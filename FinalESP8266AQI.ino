#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Deadpool";
const char* password = "123456789";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check for incoming clients
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait for a client to connect
   while (!client.available()) {
    delay(1);
  }
  // Read data from Arduino
  if (Serial.available() > 0) {
    String data = Serial.readString();    
    // Split the data into variables
    int index = data.indexOf(",");
    float resistance = data.substring(0, index).toFloat();
    data = data.substring(index + 1);
    index = data.indexOf(",");
    float ppm = data.substring(0, index).toFloat();
    data = data.substring(index + 1);
    index = data.indexOf(",");
    float temperature = data.substring(0, index).toFloat();
    data = data.substring(index + 1);
    float humidity = data.toFloat();

    // Send the web page to the client
    String webPage = "<html><body>";
    webPage += "<h1>MQ135 Sensor Data</h1>";
    webPage += "<p>Resistance: " + String(resistance) + " Ohms</p>";
    webPage += "<p>PPM: " + String(ppm) + "</p>";
    webPage += "<h2>DHT Sensor Data</h2>";
    webPage += "<p>Temperature: " + String(temperature) + " &#8451;</p>";
    webPage += "<p>Humidity: " + String(humidity) + " %</p>";
    webPage += "</body></html>";

    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n\r\n");
    client.print(webPage);
    client.stop();
    Serial.println("Client disconnected");
  }
}
