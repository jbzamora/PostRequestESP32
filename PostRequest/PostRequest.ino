#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// {1->Open, 0->Close}


//const char* ssid = "Inplastic";
//const char* password = "qwertyasdfg1234";
const char* ssid = "SPD";
const char* password = "SOLOPARADIOSES";

AsyncWebServer server(80);

//String nameSensor = "678CC52A-BA26-4150-8A83-43110E885DE0";   //Porton 1 Galpon PLANTA INPLASTIC
//String nameSensor = "00BFADD5-D7B7-4C48-A743-9BB3A4426FDB";   //Porton 1 Galpon BODEGA RESINAS INPLASTIC 
//String nameSensor = "48AC196C-207D-4436-A104-58B3496BFAC5";   //Porton 1 Galpon PLANTA INPAPELSA
String nameSensor = "3EEB6B9E-B43A-4402-9F93-2DBC68B516D7";   //Porton Bodega PLANTA INPLASTIC
//String nameSensor = "6FFD8550-73A3-4180-B37F-D4103478CF37";   //*** Porton 2 Galpon PLANTA INPLASTIC
//String nameSensor = "7B6826FB-9C2A-4792-8F61-1BE1E267D2DB";   //*** Puerta de Emergencia INPLASTIC 
String code = "";

unsigned long timer_start = 0;
unsigned long timer_refresh = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(12, OUTPUT);

  WiFi.begin(ssid, password);

  int cont = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    cont++;
    if (cont >= 10) {
      ESP.restart();
    }
  }

  Serial.print("Conectado a: ");
  Serial.print(ssid);
  Serial.print(" Con IP: ");
  Serial.println(WiFi.localIP());

  server.on("/estadosensor", HTTP_POST, [](AsyncWebServerRequest * request) {

    int estadox = analogRead(A0);

    if (estadox < 700) {
      request->send(200, "application/json", "{\"Estado\":\"ABIERTO\"}");
    }
    else {
      request->send(200, "application/json", "{\"Estado\":\"CERRADO\"}");
    }

  });
  server.begin();

  timer_start = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    int k = 0;
    for (k = 0; k < 10; k++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
      Serial.println("Signal Out");
      delay(500);
    }
    if (k >= 9) {
      ESP.restart();
    }
  }

  int estado = analogRead(A0);

  Serial.print("Estado A0: ");
  Serial.println(estado);

  if (estado < 700) {
    code = "true";    //ABIERTO
    digitalWrite(12, HIGH);
  }
  else {
    code = "false";   //CERRADO
    digitalWrite(12, LOW);
  }

  delay(50);

  timer_refresh = millis();
  if (timer_refresh >= (timer_start + 4000)) {
    timer_start = millis();
    
    String json = "{\"varGuid\":\"" + nameSensor + "\",\"Estado\":" + code + "}";
    Serial.println(json);

    WiFiClient client;
    HTTPClient http;

    http.begin(client, "http://192.168.100.11/VkeyDev/rest/PrcSensorService");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");        //Specify content-type header

    int httpResponseCode = http.POST(json);   //Send actual POST request

    if (httpResponseCode > 0) {
      String response = http.getString();   //Get the response to the request
      Serial.println(httpResponseCode);     //Print return code
      Serial.println(response);             //Print request answer
      code = "";
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
  }
}
