#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#define DHTTYPE DHT11     // DHT 11

char ssid[] = "cahaya";
char password[] = "cahayailahi";

ESP8266WebServer server (80);

int DHTPin = D5; 
DHT dht(DHTPin, DHTTYPE); 

float Temperature;
float Humidity;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  
  pinMode(DHTPin, INPUT);
  dht.begin();  

    WiFi.begin(ssid, password);
  Serial.println("Koneksi ke jaringan dilakukan . . .");
  Serial.println("SSID: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print (". ");
  }
  server.begin();
  IPAddress ip = WiFi.localIP();
  Serial.print ("\nAlamat IP: ");
  Serial.println (ip);
  Serial.print ("Siap Melayani Anda");

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
}

void handle_OnConnect() {

 Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Laporan Suhu dan Kelembaban</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Laporan Suhu dan Kelembaban</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
