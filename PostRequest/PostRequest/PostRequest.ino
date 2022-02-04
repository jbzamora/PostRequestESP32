/*****************************************************************************
  
  This is an example for doing a simple Post Request in JSON format with
  a microcontroller [WiFi] ESP32 of Espressif Systems.

  In this example we send a region or timezone similar to 'America/Guayaquil' 
  for received a string on JSON format with the local time and other dates
  provided for a micro service of https://binaryec.com  
  
  Developer: Jhonny Zamora A.
  https://github.com/jbzamora
  Email: jzamora@binaryec.com
  
*****************************************************************************/
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";

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

  Serial.println();
  Serial.print("Conectado a: ");
  Serial.print(ssid);
  Serial.print(" Con IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {    
    String json = "{\"timezone\":\"" + timezone + "\"}";

    HTTPClient http;

    http.begin("https://binaryec.com/timezone/index.php");      //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");         //Specify content-type header

    int httpResponseCode = http.POST(json);   //Send actual POST request

    if (httpResponseCode > 0) {
      String response = http.getString();   //Get the response to the request
      Serial.print("Code: "); Serial.println(httpResponseCode);     //Print return code
      Serial.print("Server Response: "); Serial.println(response);  //Print request answer
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources

    delay(5000);
}
