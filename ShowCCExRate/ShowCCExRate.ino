#include <Wire.h>
#include "SSD1306.h"
#include <WiFiClientSecure.h>

SSD1306  display(0x3c, 5, 4);

const char* ssid     = "affenstunk";
const char* password = "Schluss3ndlich";

const char* host = "api.kraken.com";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

    delay(10);
      Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


}

void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void loop() {
  delay(5000);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/0/public/Ticker";
    url += "?pair=xbteur";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server

    // Make a HTTP request:
    client.println("GET https://api.kraken.com/0/public/Ticker?pair=xbteur HTTP/1.0");
    client.println("Host: api.kraken.com");
    client.println("Connection: close");
    client.println();




    
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");


  // clear the display
  display.clear();

  // draw the current demo method
  drawTextAlignmentDemo();

  // write the buffer to the display
  display.display();

  delay(10);
}
