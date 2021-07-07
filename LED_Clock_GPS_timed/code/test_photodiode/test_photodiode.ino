#define LIGHT_RESISTOR_PIN 7 //Define light resistor digital input
#define LIGHT_RESISTOR_ARRAY_LENGTH 20
int light_resistor_history[LIGHT_RESISTOR_ARRAY_LENGTH] {0};
int light_resistor_cursor=0;

void setup() {
  pinMode(LIGHT_RESISTOR_PIN, INPUT);
  Serial.begin(9600); //initialise serial monitor

}

void loop() 
{
  int temp=digitalRead(LIGHT_RESISTOR_PIN);      //assign value of LDR sensor to a temporary variable
  Serial.print("Intensity= "); //print on serial monitor using ""
  Serial.println(temp);         //display output on serial monitor
  delay(250);
  // calculate average brightness
  calculate_brightness();
  //decide what happens when dark or bright
}

void calculate_brightness()
{
  // get brightness either 1 or 0
  int brightness=digitalRead(LIGHT_RESISTOR_PIN);
  Serial.println(brightness);
  // add it to history
  light_resistor_history[light_resistor_cursor]= brightness;
  // move cursor
  light_resistor_cursor++;
  Serial.println(light_resistor_cursor);
  if (light_resistor_cursor >= 20)
  {
    light_resistor_cursor = 0;
  }
  //sum up array
  float sum = 0;
  for (int i = 0; i < LIGHT_RESISTOR_ARRAY_LENGTH; i++)
  {
    sum += light_resistor_history[i];
  }
  Serial.println(sum);
  float average = sum/LIGHT_RESISTOR_ARRAY_LENGTH;
  Serial.println(average);
}
