#if 0     // 1 -> ESP8266 | 0 -> ESP32
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#else
#include <WiFi.h>
#include <HTTPClient.h>
#endif

const char* ssid = "SPD";
const char* password = "SOLOPARADIOSES";

String timezone = "America/Guayaquil";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to WiFi");
  int cont = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(". ");
    cont++;
    if (cont >= 10) {
      ESP.restart();
    }
  }

  Serial.print("Conectado a: ");
  Serial.print(ssid);
  Serial.print(" Con IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {    
    String json = "{\"timezone\":\"" + timezone + "\"}";
//    Serial.println(json);

    WiFiClient client;
    HTTPClient http;

    http.begin(client, "https://binaryec.com/timezone/index.php");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");        //Specify content-type header

    int httpResponseCode = http.POST(json);   //Send actual POST request

    if (httpResponseCode > 0) {
      String response = http.getString();   //Get the response to the request
      Serial.println("Code: "); Serial.print(httpResponseCode);     //Print return code
      Serial.println("Server Response: "); Serial.print(response);  //Print request answer
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources

    delay(5000);
}
