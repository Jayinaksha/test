/* * I2C_Master.ino
 * Master (Transmitter) Sketch
 * Upload this to the FIRST Arduino
 */

#include <Wire.h>

void setup() {
  Wire.begin(); // Join the I2C bus (as MASTER)
  Serial.begin(9600); // For printing to its own monitor
  
  // Define the data. 
  // B10101 is just the number 21
  // B1001101 is just the number 77
  byte seq[] = {B10101, B1001101};

  // Wait for the slave to be ready
  delay(1000); 

  for (byte i = 0; i < 2; i++) {
    // Begin transmission to the I2C slave at address 8
    Wire.beginTransmission(8); 
    
    // Wire.write() ALWAYS sends a full 8-bit byte.
    // B10101 will be sent as B00010101
    // B1001101 will be sent as B01001101
    Wire.write(seq[i]);
    
    // Send the data
    Wire.endTransmission();

    // Print what was sent to this board's local monitor
    Serial.print("TX I2C Bits: ");
    for (int b = 7; b >= 0; b--) {
      Serial.print((seq[i] >> b) & 1);
    }
    Serial.println();
    
    delay(500); // Wait between transmissions
  }
}

void loop() {
  // The work is all done in setup() for this demo.
}



/* * I2C_Slave.ino
 * Slave (Receiver) Sketch
 * Upload this to the SECOND Arduino
 */

#include <Wire.h>

// This function is an "event handler"
// It runs automatically whenever data is received
// from a Master.
void recv(int n) {
  // Loop as long as there are bytes in the receive buffer
  while (Wire.available()) {
    byte val = Wire.read(); // Read the byte
    
    // Print the received 8-bit byte
    Serial.print("RX I2C Bits: ");
    for (int b = 7; b >= 0; b--) {
      Serial.print((val >> b) & 1);
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600); // For printing to its own monitor
  
  // Join the I2C bus as a SLAVE with address 8
  Wire.begin(8);
  
  // Register the "recv" function as the event handler
  // to run when data is received.
  Wire.onReceive(recv);
}

void loop() {
  // No code needed here, the onReceive event
  // handles everything automatically!
}