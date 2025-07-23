#include <WiFi.h>

const char* ssid = "HUAWEI-m5WH";
const char* password = "mjHDqt4M";
const int serverPort = 80;

WiFiServer server(serverPort);

float receivedTemperature = 0.0;
float receivedHumidity = 0.0;
float receivedGas = 0.0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    // Read data from client
    String line = client.readStringUntil('\n');
    if (line.length() > 0) {
      int firstComma = line.indexOf(',');
      int secondComma = line.indexOf(',', firstComma + 1);

      if (firstComma != -1 && secondComma != -1) {
        receivedTemperature = line.substring(0, firstComma).toFloat();
        receivedHumidity = line.substring(firstComma + 1, secondComma).toFloat();
        receivedGas = line.substring(secondComma + 1).toFloat();
        Serial.println("Data parsed successfully");
      } else {
        Serial.println("Failed to parse data");
      }
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<!DOCTYPE html><html><head><title>ESP32 Sensor Data</title></head><body>");
    client.println("<h1>Sensor Data</h1>");
    client.print("<p>Temperature: ");
    client.print(receivedTemperature);
    client.print(" &deg;C</p>");
    client.print("<p>Humidity: ");
    client.print(receivedHumidity);
    client.print(" %</p>");
    client.print("<p>Gas: ");
    client.print(receivedGas);
    client.print(" ppm</p>");
    client.println("</body></html>");

    Serial.println("Date primite:");
    Serial.println(receivedTemperature);
    Serial.println(receivedHumidity);
    Serial.println(receivedGas);

    client.stop();
    Serial.println("Client disconnected");
  }
}
