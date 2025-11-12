// FIR Filter Example 
const int N = 2; float xn_buffer[N] = {0}; float b[N] = {0.0155, 0.0155};

void setup() { Serial.begin(9600); }

void loop() { float Axn = analogRead(A0) / 1023.0; float t = micros() / 1.0e6; // time in seconds

// create a noisy signal float xn = sin(2 * 4 * PI * t) + 0.2 * sin(2 * 50 * PI * t); float max_xn = 1.2; // normalize xn /= max_xn;

// shift old samples for (int i = N - 1; i > 0; i--) { xn_buffer[i] = xn_buffer[i - 1]; }

xn_buffer[0] = xn;

// FIR filter float yn = 0; for (int i = 0; i < N; i++) { yn += b[i] * xn_buffer[i]; }

yn *= 15; // amplify for visualization

Serial.print("Input:"); Serial.print(xn); Serial.print("\tOutput:"); Serial.println(yn);

delay(1); // smooth plotting }
