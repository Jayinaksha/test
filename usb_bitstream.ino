/* * USB_TX.ino
 * Transmitter Sketch
 * Upload this to the FIRST Arduino
 * (This is a 1-byte UART transmitter)
 */

void setup() { 
  Serial.begin(9600); 
}

void loop() {
  byte val = B1010110; // This is decimal 86

  // Send the single raw byte
  // It's sent as B01010110
  Serial.write(val);

  // Print a debug message to this Arduino's own Serial Monitor
  Serial.print("TX USB/UART Bits: ");
  for (int b = 7; b >= 0; b--) {
    Serial.print((val >> b) & 1);
  }
  Serial.println();
  
  delay(1000);
}





/* * USB_RX.ino
 * Receiver Sketch
 * Upload this to the SECOND Arduino
 * (This is a 1-byte UART receiver)
 */

void setup() {
  Serial.begin(9600);
  Serial.println("Waiting for 8-bit UART data...");
}

void loop() {
  // Wait until at least 1 byte has arrived
  if (Serial.available() > 0) {
    
    // Read the 1 byte from the buffer
    byte val = Serial.read(); 
    
    // Print the 8-bit byte we received
    Serial.print("RX USB/UART Bits: ");
    for (int b = 7; b >= 0; b--) {
      Serial.print((val >> b) & 1);
    }
    Serial.println();
  }
}


