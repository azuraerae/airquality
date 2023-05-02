#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>

// Initialisation des variables
SCD30 sensor;
int state = 2;

// Initialisation de constantes
int pins[] = {5, 17, 16};
const char *ssid = "CO2_Sensor";
const char *pw = "12345678";
AsyncWebServer server(80);

// Definire une nouvelle structure de données
typedef struct {
  uint32_t CO2;
  uint32_t temp;
  uint32_t hum;
} Data;

// Format de données envoyés au client
Data sensorData;

void setup() {
  // Setup des Pins écriture
  for(int i = 0; i <= 2; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 0);
  }

  // Setup du Serial
  Serial.begin(115200);
  println("Starting up ESP and SPIFFS.");

  // Setup de la mémoire SPIFFS
  if(!SPIFFS.begin()){
    println("An error was encountered while mounting SPIFFS...");
    return; // This cancels the start of the ESP.
  }

  // Setup du capteur
  Wire.begin();
  if(!sensor.begin()){
    println("Air Sensor not detected. Press BP?");
    while(1);  
  }

  // Extinction des LEDs
  turnOffAllLEDs();

  // Setup du point d'acces WiFi
  println("Setting up Server...");
  WiFi.softAP(ssid, pw);

  // Passing Connection information
  char buffer[100];
  sprintf(buffer, "Now listening at IP %s, on SSID %d\n", WiFi.softAPIP().toString().c_str(), ssid);
  Serial.println(buffer);

  // Setup du server web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.html", "text/html", false);
  });
  server.on("/credits", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/credits.html", "text/html", false);
  });
  server.begin();

  // Done
  println("Ready!");
}

void loop() {
  __main();
}

void __main(){
  if(sensor.dataAvailable()){
    updateData();
    stateHandler();
  }
  
  delay(2000);
}

void updateData(){
  sensorData.CO2 = sensor.getCO2();
  sensorData.temp = sensor.getTemperature();
  sensorData.hum = sensor.getHumidity();
  println(String(sensorData.CO2));
}

void stateHandler(){
  int co2 = sensorData.CO2;
  state = floor(1200/co2);
  if(state > 2){state = 2;}
  turnOffAllLEDs();
  turnOnLED(state);
}

//Helper Functions
void turnOffAllLEDs(){
  for(int i = 0; i <= 2; i++){
    digitalWrite(pins[i], 0);
  }
}

void turnOnLED(int led){
  digitalWrite(pins[led], 1);
}

void print(String message){
  Serial.print(message);
}

void println(String message){
  Serial.println(message);
}
