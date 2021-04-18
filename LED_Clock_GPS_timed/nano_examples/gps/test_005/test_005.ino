#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(4,3);//Declaramos el pin 4 Tx y 3 Rx
//Declaramos la variables para la obtención de datos
int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;

void setup()
{

Serial.begin(115200);//Iniciamos el puerto serie
serialgps.begin(9600);//Iniciamos el puerto serie del gps
//Imprimimos:
Serial.println("");
Serial.println("GPS GY-GPS6MV2 Leantec");
Serial.println(" ---  Signal suchen --- ");
Serial.println("");
}

void loop()
{
while(serialgps.available()) 
{
int c = serialgps.read(); 
if(gps.encode(c)) 
{
float latitude, longitude;
//gps.f_get_position(&latitude, &longitude);
Serial.print("test_003: "); 
Serial.println();

//gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);

gps.crack_datetime(&year,&month,&day,&hour,&minute,&second);
Serial.print("Fecha/Datum: "); Serial.print(day, DEC); Serial.print("/"); 
Serial.print(month, DEC); Serial.print("/"); Serial.print(year);

Serial.print(" Hora/Stunde: "); Serial.print(hour, DEC); Serial.print(":"); 
Serial.print(minute, DEC); Serial.print(":"); Serial.println(second, DEC); 
//Serial.print("."); Serial.println(hundredths, DEC);
Serial.println();

unsigned long hhour=hour;
Serial.print(hhour);
Serial.println();
unsigned long mminutes=minute;
Serial.print(mminutes);
Serial.println();
unsigned long ssecond=second;
Serial.print(ssecond);
Serial.println();
return (hour*10000+minute*100+second);


gps.stats(&chars, &sentences, &failed_checksum);
}
}
}
