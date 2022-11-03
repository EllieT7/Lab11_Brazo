// Importamos las librerías
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

#include "config.h"

#define SERVO1_PIN  19 
String pwmValue;
int page = 0;
AsyncWebServer server(80);

String getRSSI(){
  return String(WiFi.RSSI());
}
void setup() {
  Serial.begin(115200);   
  pinMode(SERVO1_PIN,OUTPUT);

  // Iniciamos  SPIFFS
  if (!SPIFFS.begin())
  { Serial.println("ha ocurrido un error al montar SPIFFS");
    return;
  }
  // conectamos al Wi-Fi
  WiFi.begin(ssid, password);
  // Mientras no se conecte, mantenemos un bucle con reintentos sucesivos
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Esperamos un segundo
    Serial.println("Conectando a la red WiFi..");
  }
  Serial.println();
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP:\t");
  // Imprimimos la ip que le ha dado nuestro router
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html",String(), false, datos);
  });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 0;
    request->send(SPIFFS, "/index.html", String(), false, datos);
  });
  
  server.on("/manual.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 1;
    request->send(SPIFFS, "/manual.html", String(), false, datos);
  });

  server.on("/automatico.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    page = 2;
    request->send(SPIFFS, "/automatico.html", String(), false, datos);
  });

  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
  });
  
  server.on("/RSSI", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",getRSSI().c_str());
  });

  //Tercera pestaña
  server.on("/SLIDER", HTTP_POST, [](AsyncWebServerRequest *request){
    pwmValue = request->arg("pwmValue");
  });

  server.begin();
}

String datos(const String& var) {
  if (var == "IP") {
    return WiFi.localIP().toString();
  }else if (var == "HOSTNAME") {
    return String(WiFi.SSID());
  }else if (var == "STATUS") {
    return String(WiFi.status());
  }else if (var == "PSK") {
    return String(WiFi.psk());
  }else if (var == "RSSI") {
    return String(WiFi.RSSI());
  }
  return var;
}

void loop() {
  
}
