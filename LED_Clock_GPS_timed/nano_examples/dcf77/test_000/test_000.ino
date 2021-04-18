#define DCF77PIN 2

unsigned char signal = 0;
unsigned char buffer;

void setup(void) {
 Serial.begin(9600);
 pinMode(DCF77PIN, INPUT);
}

void loop(void) {
 signal = digitalRead(DCF77PIN);
 if (buffer != signal) {
 Serial.println(signal);
 buffer = signal;
 }
}
