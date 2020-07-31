#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTTYPE DHT11 //Can be DHT12 or DHT22 if that's what you have.
uint8_t DHTPIN = D7; //Data line for sensor.
DHT dht(DHTPIN, DHTTYPE);

//Ints, may refactor the endpoint and model in mantilogs for Floats though if needed.
int temp_int = 0; 
int hum_int = 0;
//Location of the node, for me it's on the Gecko Rack here so Rack_1 
//(I just like underscores, should be fine with spaces.)
char *node_location = "Rack_1";

//Where mantilogs is on your network or if you have it remotely you can use a domain name.
char *mantilogs_url = "http://192.168.0.160:8080"

//Setup your wifi connection here.
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
 
void setup () {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
   
    delay(1000);
    Serial.print("Connecting..");
   
  }
 
}
 
void loop() {
   
  if (WiFi.status() == WL_CONNECTED) { 
    Serial.println("Connected, sending: ");
    HTTPClient http;  
    
    delay(2000);
    
    //Sensor readings.
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);
    delay(2000);
    //Denug. Gotta check those numbers.
    Serial.println(t);
    Serial.println(h);
    
    //Cast to int, pick your measurement.
    temp_int = (int)f; //Using F for more accuracy in INT. Use C for celsius if that's what you're into.
    hum_int = (int)h;
    
    String query = mantilogs_url + "/newenvlog/" + String(temp_int) + "/" + String(hum_int) + "/" + String(node_location);
    
    Serial.println(query);
    
    http.begin(query); 
    int httpCode = http.GET();                                                                  //Send the request
     
    if (httpCode > 0) { 
     
      String payload = http.getString();   
      Serial.println(payload);                     
     
    }
     
    http.end();  
   
  }
 
delay(60000*60);    //Send a request every hour
 
}
