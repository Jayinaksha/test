#include <Wire.h>

void setup() {
  Wire.begin(); 
  Serial.begin(9600);
  Serial.println("I2C Master Ready");
}

void loop() {
  Wire.beginTransmission(0x08);
  Wire.write("Hello");         
  Wire.endTransmission();      
  Serial.println("Data sent to slave");
  delay(1000);
}

#include <Wire.h>
#include <Wire.h>

void setup() {
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
  Serial.println("I2C Slave Ready");
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read(); 
    Serial.print(c);
  }
  Serial.println();
}
