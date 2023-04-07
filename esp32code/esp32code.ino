#include <DictionaryDeclarations.h>
#include <SparkFun_SCD30_Arduino_Library.h>

#define _DICT_KEYLEN 64

SCD30 sensor;

Dictionary *pins = new Dictionary();
pins("r", 5);
pins("y", 17);
pins("g", 16);

Dictionary *values = new Dictionary();
values("CO2", 0);
values("T", 0);
values("H", 0);

int state = 3;

void setup() {
  // Setup des Pins écriture
  for(int i = 0; i <= pins.count(); i++){
    pinMode(toInt(pins[i]), OUTPUT);
    digitalWrite(toInt(pins[i]), 0);
  }

  Serial.begin(115200);
  println("Starting up ESP");

  Wire.begin();
  if(!sensor.begin()){
    println("Air Sensor not detected. Press BP?");
    while(1);  
  }
  turnOffAllLEDs();
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

  updateServer();
}

void updateData(){
  values("CO2", sensor.getCO2());
  values("T", sensor.getTemperature());
  values("H", sensor.getHumidity());
}

void stateHandler(){
  int co2 = values["CO2"];
  state = floor(1200/co2);
  if(state > 3){state = 3;}
  println(String(state));
  turnOnLed(state);
}

void updateServer(){
  
}

//Helper Functions
void turnOffAllLEDs(){
  for(int i = 0; i <= pins.count(); i++){
    digitalWrite(toInt(pins[i]), 0);
  }
}

void turnOnLED(int led){
  digitalWrite(toInt(pins[led]), 1);
}

void turnOnLED(String led){
  int pin = pins[led];
  if(!pin == "") {
    pin = toInt(pin);
    digitalWrite(pin, 1);
  } else {
    String err = "[ERROR] The led ";
    String errSuite = " does not exist!";
    err.concat(led);
    err.concat(errSuite); 
    println(err)
  }
}

void print(String message){
  Serial.print(message);
}

void println(String message){
  Serial.println(message);
}
