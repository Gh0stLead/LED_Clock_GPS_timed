// ArduinoIDE 1.8.5
#include <DS3232RTC.h> // DS3232RTC by Jack Christensen Version 1.1.1 Installed
#include <Time.h>
#include <Wire.h>
#include "FastLED.h" // FastLED by Daniel Garcia Version 3.1.6 Installed

//#define NUM_LEDS 29 // Number of LED controles (remember I have 3 leds / controler
#define NUM_LEDS 165
//#define COLOR_ORDER BRG  // Define color order for your strip
#define COLOR_ORDER RGB
#define DATA_PIN 6 // Data pin for led comunication

CRGB leds[NUM_LEDS]; // Define LEDs strip

byte digits[12][27] = {{1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 0
                       {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},  // Digit 1
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Digit 2
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 3
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},  // Digit 4
                       {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 5
                       {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 6
                       {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},  // Digit 7
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 8
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},  // Digit 9 | 2D Array for numbers on 7 segment
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Digit *0
                       {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}}; // Digit C

bool Dot = true;         //Dot state
bool DST = false;        //DST state
int ledColor = 0x0000FF; // Color used (in hex)

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  LEDS.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set LED strip type
  LEDS.setBrightness(0);    // Set initial brightness
  pinMode(2, INPUT_PULLUP); // Define DST adjust button pin
  pinMode(4, INPUT_PULLUP); // Define Minutes adjust button pin
  pinMode(5, INPUT_PULLUP); // Define Hours adjust button pin
}

// Get time in a single number, if hours will be a single digit then time will be displayed 155 instead of 0155
unsigned long GetTime() 
{
  tmElements_t Now;
  RTC.read(Now);
  //time_t Now = RTC.Now();// Getting the current Time and storing it into a DateTime object
  //int hour=Now.Hour;
  unsigned long hour = Now.Hour;
  //int minutes=Now.Minute;
  unsigned long minutes = Now.Minute;
  //int second =Now.Second;
  unsigned long second = Now.Second;

  if (second % 2 == 0)
  {
    Dot = false;
  }
  else
  {
    Dot = true;
  };
  return (hour * 10000 + minutes * 100 + second);
};

// Check Light sensor and set brightness accordingly
void BrightnessCheck()
{
  const byte sensorPin = 3;                 // light sensor pin
  const byte brightnessLow = 25;            // Low brightness value
  const byte brightnessHigh = 170;          // High brightness value
  int sensorValue = digitalRead(sensorPin); // Read sensor
  if (sensorValue == 0)
  {
    LEDS.setBrightness(brightnessHigh);
  }
  else
  {
    Serial.println("Brightness Low");
    LEDS.setBrightness(brightnessLow);
  }
};

// Convert time to array needet for display
void TimeToArray()
{
  unsigned long Now = GetTime();

  int cursor = 165; // last led number

  Serial.print("Time is: ");
  Serial.println(Now);

  if (DST)// if DST is true then add one hour
  { 
    Now += 100;
  };
  if (Dot)
  {
    leds[54] = ledColor;
    leds[55] = ledColor;
    leds[110] = ledColor;
  }
  else
  {
    leds[54] = 0x000000;
    leds[55] = 0x000000;
  };

  
  for (int i = 1; i <= 6; i++)
  {
    int digit = Now % 10;
    //////////////////////////// 6 digit control:
    if (i == 1)
    {
      Serial.print("Digit 6 is : ");
      Serial.print(digit);
      Serial.print(" ");
      cursor = 138;
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    //////////////////////////// 5 digit control:
    else if (i == 2)
    {
      Serial.print("Digit 5 is : ");
      Serial.print(digit);
      Serial.print(" ");
      //cursor =14;
      cursor = 111;
      //for(int k=0; k<=6;k++){
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    //////////////////////////// 4 digit control:
    else if (i == 3)
    {
      Serial.print("Digit 4 is : ");
      Serial.print(digit);
      Serial.print(" ");
      //cursor =7;
      cursor = 83;
      //for(int k=0; k<=6;k++){
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    //////////////////////////// 3 digit control:
    else if (i == 4)
    {
      Serial.print("Digit 3 is : ");
      Serial.print(digit);
      Serial.print(" ");
      cursor = 56;
      //for(int k=0; k<=6;k++){
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    //////////////////////////// 2 digit control:
    else if (i == 5)
    {
      Serial.print("Digit 2 is : ");
      Serial.print(digit);
      Serial.print(" ");
      //cursor = 32;
      //cursor = 21;
      cursor = 27;
      //      for(int k=0; k<=27;k++){
      //      for(int k=0; k<=20;k++){
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    //////////////////////////// 1 digit control:
    else if (i == 6)
    {
      Serial.print("Digit 1 is : ");
      Serial.print(digit);
      Serial.print(" ");
      //cursor = 4;
      //cursor = 3;
      cursor = 0;
      //      for(int k=0; k<=27;k++){
      //      for(int k=0; k<=20;k++){
      for (int k = 0; k <= 26; k++)
      {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1)
        {
          leds[cursor] = ledColor;
        }
        else if (digits[digit][k] == 0)
        {
          leds[cursor] = 0x000000;
        };
        cursor++;
      };
      Serial.println();
    }

    ////////////////////////////////////////////////////////////////

    Now /= 10;
  };
};

void DSTcheck()
{
  int buttonDST = digitalRead(2);
  Serial.print("DST is: ");
  Serial.println(DST);
  if (buttonDST == LOW)
  {
    if (DST)
    {
      DST = false;
      Serial.print("Switching DST to: ");
      Serial.println(DST);
    }
    else if (!DST)
    {
      DST = true;
      Serial.print("Switching DST to: ");
      Serial.println(DST);
    };
    delay(500);
  };
}

void TimeAdjust()
{
  int buttonH = digitalRead(5);
  int buttonM = digitalRead(4);
  if (buttonH == LOW || buttonM == LOW)
  {
    delay(500);
    tmElements_t Now;
    RTC.read(Now);
    int hour = Now.Hour;
    int minutes = Now.Minute;
    int second = Now.Second;
    if (buttonH == LOW)
    {
      if (Now.Hour == 23)
      {
        Now.Hour = 0;
      }
      else
      {
        Now.Hour += 1;
      };
    }
    else
    {
      if (Now.Minute == 59)
      {
        Now.Minute = 0;
      }
      else
      {
        Now.Minute += 1;
      };
    };
    RTC.write(Now);
  }
}

void loop() // Main loop
{
  BrightnessCheck(); // Check brightness
  DSTcheck();        // Check DST
  TimeAdjust();      // Check to se if time is geting modified
  TimeToArray();     // Get leds array with required configuration
  FastLED.show();    // Display leds array
}
