#include <WiFi.h>
#include <HTTPClient.h>

char WIFI_SSID[] = "XXX";
char WIFI_PSK[] = "XXX";

// User name is <device>@<application>
char DROGUE_USER[] = "device1@example-app";
char DROGUE_PASS[] = "hey-rodney";

char DROGUE_URL[] = "https://http.sandbox.drogue.cloud/v1/pico";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  
  int status;
  Serial.println("Connecting to WiFi AP...");
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PSK);
    delay(10000);
  }
  Serial.println("Connected to WiFi!");
}

void loop() {
  HTTPClient https;
  https.setInsecure();
  Serial.println("Reporting temperature...");
  
  char payload[64];
  float temp = analogReadTemp();
  snprintf(payload, 64, "{\"temp\":%f}", temp);

  if (https.begin(DROGUE_URL)) {
    https.setAuthorization(DROGUE_USER, DROGUE_PASS);
    https.addHeader("Content-type", "application/json");
    int httpCode = https.POST(payload);
    if (httpCode > 0) {
      Serial.println("HTTP request sent");
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = https.getString();
        Serial.println("received response:");
        Serial.println(payload);
      } else {
        Serial.printf("Received HTTP code %d: %s\n", httpCode, https.errorToString(httpCode).c_str());
      }
    } else {
      Serial.printf("HTTP POST failed: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("Error starting HTTP request");
  }
  delay(5000);
}
