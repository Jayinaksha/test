/* * SPI_TX.ino
 * Master (Transmitter) Sketch
 * Upload this to the FIRST Arduino
 */

#include <SPI.h>

void setup() {
  SPI.begin(); // Initializes SPI as MASTER
  Serial.begin(9600); // For printing to its own monitor
  
  // Define the data. Note that B10101 is just the number 21.
  byte seq[] = {B10101, B1001101}; 

  // Wait a moment for the slave to be ready
  delay(2000); 

  for (byte i = 0; i < 2; i++) {
    // SPI.transfer() ALWAYS sends a full 8-bit byte.
    // B10101 will be sent as B00010101
    // B1001101 will be sent as B01001101
    SPI.transfer(seq[i]);

    // Print what was sent to this board's local monitor
    Serial.print("TX SPI Bits: ");
    for (int b = 7; b >= 0; b--) {
      Serial.print((seq[i] >> b) & 1);
    }
    Serial.println();
    delay(500);
  }
}

void loop() {
  // The work is all done in setup() for this demo.
}






/* * SPI_RX.ino
 * Slave (Receiver) Sketch
 * Upload this to the SECOND Arduino
 */

#include <SPI.h>

// These variables are "volatile" because they are
// changed by an interrupt (ISR)
volatile byte val = 0;
volatile byte flag = 0;

// === SPI Interrupt Service Routine ===
// This function runs automatically *every time*
// a full byte is received via SPI.
ISR(SPI_STC_vect) {
  val = SPDR; // Read the byte from the SPI Data Register
  flag = 1;   // Set the flag to notify the main loop
}
// ======================================

void setup() {
  Serial.begin(9600); // For printing to its own monitor

  // Set the MISO pin (Master In, Slave Out) as an OUTPUT
  // This is required for the slave to send data back (though
  // this demo doesn't send anything back, it's good practice).
  pinMode(MISO, OUTPUT);

  // Turn on SPI in SLAVE mode
  SPCR |= _BV(SPE); // Set SPI Enable bit

  // Attach the Interrupt
  // This tells the Arduino to run the ISR(SPI_STC_vect)
  // function whenever an SPI transfer is complete.
  SPI.attachInterrupt();
}

void loop() {
  // Check if the interrupt has set the flag
  if (flag) {
    // Print the 8-bit byte we received
    Serial.print("RX SPI Bits: ");
    for (int b = 7; b >= 0; b--) {
      Serial.print((val >> b) & 1);
    }
    Serial.println();
    
    flag = 0; // Reset the flag to wait for the next byte
  }
}



// Wiring:

//     Connect GND of the Master to GND of the Slave.

//     Connect SCK (Pin 13 on Uno) of the Master to SCK (Pin 13) of the Slave.

//     Connect MOSI (Pin 11 on Uno) of the Master to MOSI (Pin 11) of the Slave.

//     Connect MISO (Pin 12 on Uno) of the Master to MISO (Pin 12) of the Slave.

//     Slave Select: Connect the SS pin (Pin 10 on Uno) of the Slave Arduino to its own GND pin. This permanently "selects" the slave so it's always l