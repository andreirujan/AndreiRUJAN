#include <Wire.h>
#include <WiFi.h>

const char* ssid = "HUAWEI-m5WH";
const char* password = "mjHDqt4M";

const char* serverIP = "192.168.100.91";  // IP-ul celei de-a doua plăcuțe ESP32 (server)
const uint16_t serverPort = 80;

WiFiClient client;

float receivedTemperature = 0.0;
float receivedHumidity = 0.0;
float receivedGas = 0.0;

void receiveEvent(int howMany) {
  if (howMany == sizeof(receivedTemperature) + sizeof(receivedHumidity) + sizeof(receivedGas)) {
    Wire.readBytes((char*)&receivedTemperature, sizeof(receivedTemperature));
    Wire.readBytes((char*)&receivedHumidity, sizeof(receivedHumidity));
    Wire.readBytes((char*)&receivedGas, sizeof(receivedGas));
    Serial.print("Received Temperature: ");
    Serial.print(receivedTemperature);
    Serial.print(" Humidity: ");
    Serial.print(receivedHumidity);
    Serial.print(" Gas: ");
    Serial.println(receivedGas);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08); // Adresa I2C a clientului
  Wire.onReceive(receiveEvent);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Send data to another ESP32 server
  if (client.connect(serverIP, serverPort)) {
    String dataToSend = String(receivedTemperature) + "," + String(receivedHumidity) + "," + String(receivedGas);
    client.println(dataToSend);
    Serial.println("Data sent to server: " + dataToSend);
    client.stop();
  } else {
    Serial.println("Connection to server failed");
  }

  delay(2000);
}
