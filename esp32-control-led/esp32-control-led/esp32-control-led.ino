#include <WiFi.h>

// WiFi credentials
const char* ssid = "milad";
const char* password = "123456789";

// LED pin
const int ledPin = 2;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // HTML + CSS
            client.println("<!DOCTYPE html><html><head><style>");

            client.println("body {");
            client.println("position: relative;");
            client.println("height: 100vh;");
            client.println("margin: 0;");
            client.println("display: flex;");
            client.println("flex-direction: column;");
            client.println("align-items: center;");
            client.println("justify-content: center;");
            client.println("font-family: Arial, sans-serif;");
            client.println("color: white;");
            client.println("overflow: hidden;");
            client.println("}");

            client.println("body::before {");
            client.println("content: \"\";");
            client.println("position: absolute;");
            client.println("top: 0; left: 0;");
            client.println("width: 100%; height: 100%;");
            client.println("background-image: url('https://i.ibb.co/rK8JnJMH/back.png');");
            client.println("background-size: contain;");
            client.println("background-repeat: no-repeat;");
            client.println("background-position: center;");
            client.println("background-color: black;");
            client.println("opacity: 0.7;");
            client.println("z-index: -1;");
            client.println("}");

            client.println(".title {");
            client.println("position: absolute;");
            client.println("top: 20px;");
            client.println("width: 100%;");
            client.println("text-align: center;");
            client.println("}");

            client.println("button {");
            client.println("font-size: 24px;");
            client.println("padding: 15px 30px;");
            client.println("margin: 15px;");
            client.println("border: none;");
            client.println("border-radius: 10px;");
            client.println("background-color: rgba(0, 0, 0, 0.6);");
            client.println("color: white;");
            client.println("cursor: pointer;");
            client.println("}");

            client.println("button:hover {");
            client.println("background-color: rgba(255, 255, 255, 0.2);");
            client.println("}");

            client.println("</style></head><body>");
            client.println("<div class='title'><h1>LED Control</h1></div>");
            client.println("<a href=\"/on\"><button>Turn ON</button></a>");
            client.println("<a href=\"/off\"><button>Turn OFF</button></a>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            if (currentLine.indexOf("GET /on") >= 0) {
              digitalWrite(ledPin, HIGH);
            }
            if (currentLine.indexOf("GET /off") >= 0) {
              digitalWrite(ledPin, LOW);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
