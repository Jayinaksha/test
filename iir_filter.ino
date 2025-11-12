// IIR Filter: 
float xn1 = 0; 
float yn1 = 0;
const int analogInputPin = A0;

void setup() { Serial.begin(9600); }

void loop() { // float xn = analogRead(analogInputPin) / 1023.0; // Normalize analog input to range [0, 1]

float t = micros() / 1.0e6;

// creating the noisy signal
  float xn = sin(4 * PI * t) + 0.2 * sin(2 * 50 * PI * t); 
  float yn = 0.969 * yn1 + 0.0155 * xn + 0.0155 * xn1;

// Normalize signal by their maximum values 
  float max_xn = 1.2; // Maximum value of xn 
  xn = xn / max_xn;

// Print normalized values Serial.print(xn); Serial.print(","); Serial.println(yn);

xn1 = xn; 
  yn1 = yn;

delay(1);
}
