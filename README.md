# Embedded Systems Lab Experiments

A collection of embedded systems lab experiments covering GPIO, communication protocols (UART, SPI, I²C, USB), digital signal filtering (FIR/IIR), and more — implemented on Arduino, PIC microcontrollers (MikroC/Proteus), and STM32 Nucleo boards.

---

## Table of Contents

1. [Blinking LED — STM32 Nucleo](#exp-1-blinking-led--stm32-nucleo)
2. [Blinking LED — PIC (Proteus + MikroC)](#exp-2-blinking-led--pic-proteus--mikroc)
3. [7-Segment Display](#exp-3-7-segment-display)
4. [Stepper Motor](#exp-4-stepper-motor)
5. [Keypad Input](#exp-5-keypad-input)
6. [UART Communication](#exp-6-uart-communication)
7. [SPI Communication](#exp-7-spi-communication)
8. [I²C Communication](#exp-8-i2c-communication)
9. [USB (CDC Serial)](#exp-9-usb-cdc-serial)
10. [Bitstream Demos (UART / SPI / I²C / USB)](#exp-10-bitstream-demos)
11. [FIR & IIR Filters](#exp-11-fir--iir-filters)
12. [DAC / ADC — STM32](#exp-12-dac--adc-stm32)

---

## Exp 1: Blinking LED — STM32 Nucleo

**Hardware:** STM32 Nucleo-F103RB, USB cable  
**Tool:** STM32CubeIDE

Blinks the onboard LED (PA5) once per second using the STM32 HAL library.

```c
while (1) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
  HAL_Delay(1000);
}
```

> **Setup:** In STM32CubeIDE, use the Board Selector and search for `STM32 Nucleo-F103RB`.

---

## Exp 2: Blinking LED — PIC (Proteus + MikroC)

**Hardware (simulated):** PIC microcontroller, LEDs, 220 Ω resistors (on pins RB6–RB13)  
**Tools:** Proteus 8, MikroC for PIC

### Single LED blink

```c
void main() {
  TRISB = 0x00;
  PORTB = 0x00;
  while (1) {
    PORTB = 0b00000001;
    Delay_ms(250);
    PORTB = 0b00000000;
    Delay_ms(250);
  }
}
```

### 8 LED sequential blink (using loop)

```c
void main() {
  TRISB = 0x00;
  PORTB = 0x00;
  while (1) {
    unsigned int x = 0;
    for (x = 0; x < 8; x++) {
      PORTB = (1 << x);
      Delay_ms(100);
    }
  }
}
```

> **Wiring:** Connect pins RB6–RB13 through 220 Ω resistors to the anodes of the LEDs; cathodes to GND.

---

## Exp 3: 7-Segment Display

**Hardware (simulated):** PIC microcontroller, 7-segment display, 220 Ω resistors (pins RB6–RB12)  
**Tools:** Proteus 8, MikroC for PIC

Cycles digits 0–9 on a common-cathode 7-segment display by writing segment patterns to PORTB.

```c
void main() {
  TRISB = 0x00;
  PORTB = 0x00;
  while (1) {
    PORTB = 0b00111111; delay_ms(500); // 0
    PORTB = 0b00000110; delay_ms(500); // 1
    PORTB = 0b01011011; delay_ms(500); // 2
    PORTB = 0b01001111; delay_ms(500); // 3
    PORTB = 0b01100110; delay_ms(500); // 4
    PORTB = 0b01101101; delay_ms(500); // 5
    PORTB = 0b01111101; delay_ms(500); // 6
    PORTB = 0b00000111; delay_ms(500); // 7
    PORTB = 0b01111111; delay_ms(500); // 8
    PORTB = 0b01101111; delay_ms(500); // 9
  }
}
```

> **Wiring:** Connect PORTB pins (RB6–RB12) through 220 Ω resistors to the 7-segment display inputs.

---

## Exp 4: Stepper Motor

**Hardware:** PIC16F84A, ULN2003A driver IC, stepper motor  
**Tool:** MikroC for PIC

The PIC generates a 4-step wave-drive sequence. The ULN2003A (Darlington transistor array) amplifies the PIC's output current to drive the motor coils.

```c
void main() {
  TRISB = 0x00;
  PORTB = 0x00;
  while (1) {
    PORTB = 0b0001; Delay_ms(200);
    PORTB = 0b0010; Delay_ms(200);
    PORTB = 0b0100; Delay_ms(200);
    PORTB = 0b1000; Delay_ms(200);
  }
}
```

> **Wiring:** RB0–RB3 (PIC pins 6–9) → ULN2003A inputs 1B–4B → outputs 1C–4C → stepper motor coils. ULN2003A COM pin → motor supply voltage.

---

## Exp 5: Keypad Input

**Hardware:** PIC16F877A, 4×4 matrix keypad, 74LS47 BCD-to-7-segment decoder, 7-segment display  
**Tool:** MikroC for PIC

Reads a key press from the keypad and displays the corresponding value on a 7-segment display via the 74LS47 decoder.

```c
unsigned short KP = 0;
char keypadPort at PORTB;

void main() {
  TRISA = 0x00;
  PORTA = 0x00;
  Keypad_Init();

  while (1) {
    do { KP = Keypad_Key_Press(); } while (!KP);

    if (KP <= 3)             PORTA = KP;
    else if (KP <= 7)        PORTA = KP - 1;
    else if (KP <= 11)       PORTA = KP - 2;
  }
}
```

> **Wiring:** PIC RA2–RA5 → 74LS47 pins 7, 1, 2, 6. RB0–RB3 → keypad rows 1–3; RB4–RB7 → keypad columns A–D. 74LS47 outputs → 7-segment display inputs.

---

## Exp 6: UART Communication

**Hardware:** Two Arduino boards connected via TX/RX crossover  
**Baud rate:** 9600

| Signal | Board A | Board B |
|--------|---------|---------|
| TX     | TX      | RX      |
| RX     | RX      | TX      |
| GND    | GND     | GND     |

**Transmitter**

```c
void setup() { Serial.begin(9600); }
void loop() {
  Serial.println("Hello from A");
  delay(1000);
}
```

**Receiver**

```c
void setup() { Serial.begin(9600); }
void loop() {
  if (Serial.available()) {
    Serial.println(Serial.readString());
  }
}
```

---

## Exp 7: SPI Communication

**Hardware:** Two Arduino boards  
**Library:** `SPI.h`

| Signal | Master (Board A) | Slave (Board B) |
|--------|-----------------|----------------|
| MOSI   | 11              | 11             |
| MISO   | 12              | 12             |
| SCK    | 13              | 13             |
| SS     | 10              | 10             |
| GND    | GND             | GND            |

**Master**

```c
#include <SPI.h>
void setup() {
  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
}
void loop() {
  digitalWrite(SS, LOW);
  SPI.transfer('A');
  digitalWrite(SS, HIGH);
  delay(1000);
}
```

**Slave**

```c
#include <SPI.h>
volatile byte receivedData;
volatile boolean received = false;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
}
ISR(SPI_STC_vect) {
  receivedData = SPDR;
  received = true;
}
void loop() {
  if (received) {
    Serial.print("Received: ");
    Serial.println((char)receivedData);
    received = false;
  }
}
```

---

## Exp 8: I²C Communication

**Hardware:** Two Arduino boards  
**Library:** `Wire.h`

| Signal | Master (Board A) | Slave (Board B) |
|--------|-----------------|----------------|
| SDA    | A4              | A4             |
| SCL    | A5              | A5             |
| GND    | GND             | GND            |

**Master**

```c
#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
}
void loop() {
  Wire.beginTransmission(0x08);
  Wire.write("Hello");
  Wire.endTransmission();
  Serial.println("Data sent to slave");
  delay(1000);
}
```

**Slave** (address `0x08`)

```c
#include <Wire.h>
void setup() {
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}
void loop() { delay(100); }
void receiveEvent(int howMany) {
  while (Wire.available()) Serial.print((char)Wire.read());
  Serial.println();
}
```

---

## Exp 9: USB (CDC Serial)

**Hardware:** Single Arduino (uses built-in USB-to-serial)

Sends a hello message and prints uptime (ms) over USB every second.

```c
void setup() {
  Serial.begin(9600);
  Serial.println("Hello PC via USB!");
}
void loop() {
  Serial.println(millis());
  delay(1000);
}
```

---

## Exp 10: Bitstream Demos

These demos transmit raw binary sequences and display each bit on the Serial Monitor/Plotter for analysis.

### UART Bitstream

**Transmitter** — sends two binary sequences (`10101`, `1001101`) with a length prefix byte.

```c
void setup() { Serial.begin(9600); }
void loop() {
  byte seq[] = {B10101, B1001101};
  byte len[] = {5, 7};
  for (byte i = 0; i < 2; i++) {
    Serial.write(len[i]);
    Serial.write(seq[i]);
    Serial.print("TX UART Bits: ");
    for (byte b = 0; b < len[i]; b++) Serial.print((seq[i] >> b) & 1);
    Serial.println();
    delay(500);
  }
  delay(2000);
}
```

**Receiver** — reconstructs bits, computes parity, and prints a tabular view.

```c
void setup() {
  Serial.begin(9600);
  Serial.println("Start\tB0\tB1\tB2\tB3\tB4\tB5\tB6\tB7\tParity\tStop");
}
void loop() {
  if (Serial.available() >= 2) {
    byte len = Serial.read(), data = Serial.read(), p = 0, bits[9];
    for (byte i = 0; i < len; i++) p ^= bits[i] = (data >> i) & 1;
    Serial.print("RX UART Bits: ");
    for (byte i = 0; i < len; i++) Serial.print(bits[i]);
    Serial.print(" | Parity: "); Serial.println(p);
  }
}
```

---

### SPI Bitstream

**Transmitter**

```c
#include <SPI.h>
void setup() {
  SPI.begin(); Serial.begin(9600);
  byte seq[] = {B10101, B1001101};
  for (byte i = 0; i < 2; i++) {
    SPI.transfer(seq[i]);
    Serial.print("TX SPI Bits: ");
    for (int b = 7; b >= 0; b--) Serial.print((seq[i] >> b) & 1);
    Serial.println();
    delay(500);
  }
}
void loop() {}
```

**Receiver**

```c
#include <SPI.h>
volatile byte val = 0, flag = 0;
ISR(SPI_STC_vect) { val = SPDR; flag = 1; }
void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();
}
void loop() {
  if (flag) {
    Serial.print("RX SPI Bits: ");
    for (int b = 7; b >= 0; b--) Serial.print((val >> b) & 1);
    Serial.println();
    flag = 0;
  }
}
```

---

### I²C Bitstream

**Master (transmitter)**

```c
#include <Wire.h>
void setup() {
  Wire.begin(); Serial.begin(9600);
  byte seq[] = {B10101, B1001101};
  for (byte i = 0; i < 2; i++) {
    Wire.beginTransmission(8);
    Wire.write(seq[i]);
    Wire.endTransmission();
    Serial.print("TX I2C Bits: ");
    for (int b = 7; b >= 0; b--) Serial.print((seq[i] >> b) & 1);
    Serial.println();
    delay(500);
  }
}
void loop() {}
```

**Slave (receiver)**

```c
#include <Wire.h>
void recv(int n) {
  while (Wire.available()) {
    byte val = Wire.read();
    Serial.print("RX I2C Bits: ");
    for (int b = 7; b >= 0; b--) Serial.print((val >> b) & 1);
    Serial.println();
  }
}
void setup() { Serial.begin(9600); Wire.begin(8); Wire.onReceive(recv); }
void loop() {}
```

---

### USB Bitstream

```c
void setup() { Serial.begin(9600); }
void loop() {
  byte val = B1010110;
  Serial.write(val);
  Serial.print("TX USB Bits: ");
  for (int b = 7; b >= 0; b--) Serial.print((val >> b) & 1);
  Serial.println();
  delay(1000);
}
```

---

## Exp 11: FIR & IIR Filters

Both filters are applied to a noisy sine-wave input signal (2 Hz + 50 Hz noise) and output the filtered result for the Serial Plotter.

| Feature        | FIR                         | IIR                         |
|----------------|-----------------------------|-----------------------------|
| Depends on     | Current & past **inputs**   | Inputs **and** past outputs |
| Feedback       | None                        | Yes                         |
| Stability      | Always stable               | Can become unstable         |
| Efficiency     | More coefficients needed    | Fewer coefficients needed   |

### FIR (3-tap moving average)

```c
#define N 3
float x[N];
float coeffs[N] = {1.0/N, 1.0/N, 1.0/N};

float fir(float newSample) {
  for (int i = N - 1; i > 0; i--) x[i] = x[i - 1];
  x[0] = newSample;
  float y = 0;
  for (int i = 0; i < N; i++) y += coeffs[i] * x[i];
  return y;
}

void setup() { Serial.begin(9600); }
void loop() {
  static int n = 0;
  float t = n * 0.1;
  float input = sin(t) * 100 + random(-10, 10);
  Serial.print(input); Serial.print(","); Serial.println(fir(input));
  n++;
  delay(50);
}
```

### IIR (exponential moving average, α = 0.1)

```c
float alpha = 0.1;
float y = 0;

void setup() { Serial.begin(9600); }
void loop() {
  static int n = 0;
  float t = n * 0.1;
  float input = sin(t) * 100 + random(-10, 10);
  y = y + alpha * (input - y);
  Serial.print(input); Serial.print(","); Serial.println(y);
  n++;
  delay(50);
}
```

### TA Reference — IIR (first-order recursive, analog-style)

Uses a real input signal on pin A0 with pre-designed coefficients.

```c
float xn1 = 0, yn1 = 0;

void setup() { Serial.begin(9600); }
void loop() {
  float t = micros() / 1.0e6;
  float xn = sin(2 * 2 * PI * t) + 0.2 * sin(2 * 50 * PI * t);
  xn = xn / 1.2;  // normalize
  float yn = 0.959 * yn1 + 0.0155 * xn1 + 0.0155 * xn;
  Serial.print(xn); Serial.print(" "); Serial.println(yn);
  xn1 = xn; yn1 = yn;
  delay(1);
}
```

### TA Reference — FIR (2-tap, analog-style)

```c
const int N = 2;
float buffer_xn[N] = {0, 0};
float b[N] = {0.0155, 0.0155};

void setup() { Serial.begin(9600); }
void loop() {
  float t = micros() / 1.0e6;
  float xn = sin(2.0 * 4.0 * PI * t) + 0.2 * sin(2.0 * 50.0 * PI * t);
  xn = xn / 1.2;

  for (int i = N - 1; i > 0; i--) buffer_xn[i] = buffer_xn[i - 1];
  buffer_xn[0] = xn;

  float yn = 0.0;
  for (int i = 0; i < N; i++) yn += b[i] * buffer_xn[i];

  Serial.print(xn); Serial.print(" "); Serial.println(yn);
  delay(1);
}
```

---

## Exp 12: DAC / ADC — STM32

**Hardware:** STM32 Nucleo-F401RE (or compatible), potentiometer on PA0  
**Tool:** STM32CubeIDE (HAL-generated code in `dac/`)

Reads a 12-bit ADC value from channel 0 (PA0) every second and transmits it as a decimal string over USART2 at 9600 baud — viewable in any serial terminal.

**Key loop (from `dac/`):**

```c
while (1) {
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
  char msg[10];
  sprintf(msg, "%hu\r\n", adc_value);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
  HAL_Delay(1000);
}
```

> **ADC config:** 12-bit resolution, single conversion, software trigger, 3-cycle sample time (configured via `MX_ADC1_Init` in `dac/`).

---

## Repository Structure

```
├── BLINK_LED            # PIC LED blinking code (MikroC)
├── SPI                  # Arduino SPI master/slave code
├── dac/                 # STM32 ADC-to-UART project (HAL/CubeIDE)
├── fir_filter.ino       # Arduino FIR filter demo
├── iir_filter.ino       # Arduino IIR filter demo
├── i2c.ino              # Arduino I²C basic demo
├── i2c_bitstream_1.ino  # I²C bitstream demo
├── spi_bitstream.ino    # SPI bitstream demo
├── uart_bit_stream.ino  # UART bitstream demo
├── usb_bitstream.ino    # USB (CDC) bitstream demo
└── README.md
```

---

## Tools & Platforms

| Platform | Tool |
|----------|------|
| STM32 Nucleo | STM32CubeIDE + HAL |
| PIC microcontrollers | MikroC for PIC + Proteus 8 (simulation) |
| Arduino (Uno/Nano/etc.) | Arduino IDE |
