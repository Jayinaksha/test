/* * UART_TX.ino
 * Transmitter Sketch
 * Upload this to the FIRST Arduino
 */

void setup() {
  // Start serial communication with the PC Serial Monitor
  Serial.begin(9600); 
}

void loop() {
  // Data to send:
  // seq[0] = B10101 (decimal 21), 5 bits long
  // seq[1] = B1001101 (decimal 77), 7 bits long
  byte seq[] = {B10101, B1001101};
  byte len[] = {5, 7};

  for (byte i = 0; i < 2; i++) {
    // Send the two-byte "packet"
    // Byte 1: The length of the data
    Serial.write(len[i]);
    // Byte 2: The data itself
    Serial.write(seq[i]);

    // Print a debug message *to this Arduino's own Serial Monitor*
    Serial.print("TX UART Bits: ");
    for (byte b = 0; b < len[i]; b++) {
      Serial.print((seq[i] >> b) & 1);
    }
    Serial.println();
    delay(500);
  }
  delay(2000); // Wait 2 seconds before repeating
}





/* * UART_RX.ino
 * Receiver Sketch
 * Upload this to the SECOND Arduino
 */

void setup() {
  // Start serial communication with the PC Serial Monitor
  Serial.begin(9600);
  // Print the header for the table
  Serial.println("Start\tB0\tB1\tB2\tB3\tB4\tB5\tB6\tB7\tParity\tStop");
}

void loop() {
  // Wait until the 2-byte packet (length + data) has arrived
  if (Serial.available() >= 2) {
    
    // Read the 2-byte packet
    byte len = Serial.read();  // Read Byte 1 (the length)
    byte data = Serial.read(); // Read Byte 2 (the data)
    
    byte p = 0;      // Variable to store calculated parity
    byte bits[9];    // Array to store the extracted bits

    // Extract the 'len' number of bits from the 'data' byte
    for (byte i = 0; i < len; i++) {
      bits[i] = (data >> i) & 1; // Extract the i-th bit
      p ^= bits[i];              // Calculate parity using XOR
    }

    // --- Print the results to this Arduino's Serial Monitor ---

    // Print the raw bits received
    Serial.print("RX UART Bits: ");
    for (byte i = 0; i < len; i++) {
      Serial.print(bits[i]);
    }
    Serial.print(" | Parity: "); 
    Serial.println(p);

    // Print the formatted table row
    Serial.print(0); // Fake Start Bit
    Serial.print('\t');
    for (byte i = 0; i < 8; i++) { 
      // Print the bits we cared about, or 0 for padding
      Serial.print(i < len ? bits[i] : 0); 
      Serial.print('\t'); 
    }
    Serial.print(p); // Calculated Parity Bit
    Serial.print('\t'); 
    Serial.println(1); // Fake Stop Bit
  }
}