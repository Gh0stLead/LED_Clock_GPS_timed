#include <DS3232RTC.h>
#include <RTClib.h> //
#include <Time.h> 
#include <TimeLib.h>
#include <FastLED.h> //

RTC_DS3231 rtc;


//#define NUM_LEDS 86 // 4+((7*4)*4)+2  Number of LEDs in the project
#define NUM_LEDS 165 // 4+((7*4)*4)+2  Number of LEDs in the project

#define COLOR_ORDER RGB  // Define color order for your strip (RGB, GBR, BRG, etc)
#define DATA_PIN 6  // Data pin for led comunication
#define DST_PIN 2  // Define DST adjust button pin
#define MIN_PIN 4  // Define Minutes adjust button pin
#define HUR_PIN 5  // Define Hours adjust button pin
#define BRI_PIN 3  // Define Light sensor pin

CRGB leds[NUM_LEDS]; // Define LEDs strip
                    // 0,0,0,0
                    // 1,1,1,1
                    //  1 2 3 4 5 6 7 8 9 10111213141516171819202122232425262728    one digit led number

byte digits[12][27] = {{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},  // Digit 0
                       {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},   // Digit 1
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0},   // Digit 2
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1},   // Digit 3
                       {1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1},   // Digit 4
                       {1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1},   // Digit 5
                       {1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},   // Digit 6
                       {0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1},   // Digit 7
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},   // Digit 8
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1},   // Digit 9 | 2D Array for numbers on 7 segment
                       {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},   // Digit *0
                       {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0}};  // Digit C

bool Dot = true;  //Dot state Doppelpunkt !!!
bool DST = false; //DST state
int last_digit = 0;

//int ledColor = 0x0000FF; // Color used - Green (in hex)
//int ledColor = 0x4682B4; // lightBlue
//int ledColor = 0xF0F8FF; //middleBlue
int ledColor = 0x0000FF;
//int ledColor = 0x000F00; // halbe Helligkeit Blau
//int ledColor = 0xFF0000; // AUS
//int ledColor = 0xFFFF00; // hellBlau
//int ledColor = 0x00000F; // Color used - halbe helligkeit Green (in hex)
//long ledColor = CRGB::Blue;
//int ledColor = 0x228B22;
//int ledColor = 0x00FF00; // Color used (in hex)
//long ledColor = CRGB::DarkOrchid; // Color used (in hex)
//long ledColor = CRGB::MediumVioletRed;
//Random colors i picked up
long ColorTable[16] = {
  CRGB::Amethyst,
  CRGB::Aqua,
  CRGB::Blue,
  CRGB::Chartreuse,
  CRGB::DarkGreen,
  CRGB::DarkMagenta,
  CRGB::DarkOrange,
  CRGB::DeepPink,
  CRGB::Fuchsia,
  CRGB::Gold,
  CRGB::GreenYellow,
  CRGB::LightCoral,
  CRGB::Tomato,
  CRGB::Salmon,
  CRGB::Red,
  CRGB::Orchid
};


void setup(){ 
  Serial.begin(9600); 
  LEDS.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Set LED strip type
  //LEDS.setBrightness(75); // Set initial brightness
  LEDS.setBrightness(0); // Set initial brightness
 
  pinMode(DST_PIN, INPUT_PULLUP); // Define DST adjust button pin
  pinMode(MIN_PIN, INPUT_PULLUP); // Define Minutes adjust button pin
  pinMode(HUR_PIN, INPUT_PULLUP); // Define Hours adjust button pin
}

int GetTime(){
  tmElements_t Now;
  RTC.read(Now);
  int hour=Now.Hour;
  int minutes=Now.Minute;

//int hour=Now.Minute;
//int minutes=Now.Second;

  int second =Now.Second;
  if (second % 2==0) {Dot = false;}
    else {Dot = true;};
  return (hour*100+minutes);
  };


// Check Light sensor and set brightness accordingly
void BrightnessCheck(){
const byte sensorPin = 3; // light sensor pin
const byte brightnessLow = 25; // Low brightness value
const byte brightnessHigh = 170; // High brightness value
int sensorValue = digitalRead(sensorPin); // Read sensor
if (sensorValue == 0) {
Serial.println("Brightness High");
LEDS.setBrightness(brightnessHigh);
}
else {
Serial.println("Brightness Low");
LEDS.setBrightness(brightnessLow);}
};




void DSTcheck(){
   int buttonDST = digitalRead(2);
   // Serial.print("DST is: ");Serial.println(DST);
   if (buttonDST == LOW){
    if (DST){
      DST=false;
      // Serial.print("Switching DST to: ");Serial.println(DST);
      }
      else if (!DST){
        DST=true;
        // Serial.print("Switching DST to: ");Serial.println(DST);
      };
   delay(500);   
   };
  }

void TimeAdjust(){
  int buttonH = digitalRead(HUR_PIN);
  int buttonM = digitalRead(MIN_PIN);
  if (buttonH == LOW || buttonM == LOW){
    delay(500);
    tmElements_t Now;
    RTC.read(Now);
    int hour=Now.Hour;
    int minutes=Now.Minute;
    int second =Now.Second;
      if (buttonH == LOW){
        if (Now.Hour== 23){Now.Hour=0;}
          else {Now.Hour += 1;};
        }else {
          if (Now.Minute== 59){Now.Minute=0;}
          else {Now.Minute += 1;};
          };
          
    RTC.write(Now); 
    }
  }


/////////////////////////////////////////////////////////////////////////
void TimeToArray(){
  int Now = GetTime();  // Get time
  
//  int cursor = 118; // last led number
//  int cursor = 86; // last led number
  int cursor = 165; // last led number
    leds[0]=0x000000; // blanking dash
    leds[1]=0x000000; // blanking dash
    leds[2]=0x000000; // blanking dash
    leds[3]=0x000000; // blanking dash
    
   Serial.print("Time is: ");Serial.println(Now);
  if (DST){   // if DST is true then add one hour
   Now+=100;
   Serial.print("DST is ON, time set to : ");Serial.println(Now);
  }; 
//  if (Dot){leds[42]=ledColor;
//           leds[43]=ledColor;}   
//    else  {leds[42]=0x000000;
//           leds[43]=0x000000;
    if (Dot){leds[54]=ledColor;
           leds[55]=ledColor;;
           leds[110]=ledColor;}   
    else  {leds[54]=0x000000;
           leds[55]=0x000000;
    };
    
  for(int i=1;i<=4;i++){
    int digit = Now % 10; // get last digit in time
   //////////////////////////// 4 digit control: 
    if (i==1){
      Serial.print("Digit 4 is : ");
      Serial.print(digit);
      Serial.print(" ");
      //cursor = 90;
      //cursor =65;
      cursor =83;
    //for(int k=0; k<=27;k++){
    //for(int k=0; k<=20;k++){
    for(int k=0; k<=26;k++){
             Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
       Serial.println();

      if (digit != last_digit)
      {
//        cylon();
//        ledColor =  ColorTable[random(16)];
      }
      last_digit = digit;
      }
  //////////////////////////// 3 digit control:
    else if (i==2){
       Serial.print("Digit 3 is : ");
       Serial.print(digit);
       Serial.print(" ");
      //cursor =62;
      // cursor =44;
      cursor =56;
    //for(int k=0; k<=27;k++){ 
    //for(int k=0; k<=20;k++){
    for(int k=0; k<=26;k++){
         Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
       Serial.println();
      }
//////////////////////////// 2 digit control: 
    else if (i==3){
       Serial.print("Digit 2 is : ");
       Serial.print(digit);
       Serial.print(" ");
      //cursor =32;
      //cursor =21;
      cursor =27;
      //for(int k=0; k<=27;k++){ 
      //for(int k=0; k<=20;k++){
      for(int k=0; k<=26;k++){
         Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
       Serial.println();
      }
  //////////////////////////// 1 digit control:
    else if (i==4){
       Serial.print("Digit 1 is : ");
       Serial.print(digit);
       Serial.print(" ");
      //cursor =4;
      //cursor =3;
      cursor =0;
      //for(int k=0; k<=27;k++){ 
//      for(int k=0; k<=20;k++){
      for(int k=0; k<=26;k++){
         Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
       Serial.println();
      }
    Now /= 10;
  }; 
};

/////////////////////////////////////////////////////////////////////////


void loop()
{ 
  BrightnessCheck(); // Check brightness
  DSTcheck(); // Check DST
  TimeAdjust(); // Check to se if time is geting modified
  TimeToArray(); // Show time
  FastLED.show(); // Display leds array
}
