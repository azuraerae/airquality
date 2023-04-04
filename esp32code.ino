#include <SparkFun_SCD30_Arduino_Library.h>
SCD30 sensor;
int pins[] = {16, 17, 5};

void setup() {
  // Setup des Pins écriture
  for(int i = 0; i <= sizeOfArray(pins); i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 0);
  }

  Serial.begin(115200);
  println("Starting up ESP");

  Wire.begin();
  if(!sensor.begin()){
    println("Air Sensor not detected. Press BP?");
    while(1);  
  }
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  turnOffAllLEDs();
  delay(1500);
  turnOnLED(pins[1]);
  delay(1500);
}



//Helper Functions
void turnOffAllLEDs(){
  for(int i = 0; i <= sizeOfArray(pins); i++){
    String a = "Turning off ";
    String n = String(i);
    print(a);
    print(n);
    println("");
    digitalWrite(pins[i], 0);
  }
}

void turnOnLED(int pin){
  digitalWrite(pin, 1);
}

int sizeOfArray(int i[]){
  int sizeq = sizeof(i) / sizeof(i[0]);
  println(sizeq); //Change to string
  return sizeq;
}

void print(String message){
  Serial.print(message);
}

void println(String message){
  Serial.println(message);
}
