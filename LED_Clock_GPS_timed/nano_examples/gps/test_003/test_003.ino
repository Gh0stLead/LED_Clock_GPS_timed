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
gps.f_get_position(&latitude, &longitude);
Serial.print("Latitud/Longitud-Breite/Länge: "); 
Serial.print(latitude,5); 
Serial.print(", "); 
Serial.println(longitude,5);
//gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
gps.crack_datetime(&year,&month,&day,&hour,&minute,&second);
Serial.print("Fecha/Datum: "); Serial.print(day, DEC); Serial.print("/"); 
Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
Serial.print(" Hora/Stunde: "); Serial.print(hour, DEC); Serial.print(":"); 
Serial.print(minute, DEC); Serial.print(":"); Serial.println(second, DEC); 
//Serial.print("."); Serial.println(hundredths, DEC);
Serial.print("Altitud (metros): "); Serial.println(gps.f_altitude()); 
Serial.print("Rumbo/Kurs (grados/Grad): "); Serial.println(gps.f_course()); 
Serial.print("Velocidad/Geschwindigkeit(kmph): "); Serial.println(gps.f_speed_kmph());
Serial.print("Satelites/Satelliten: "); Serial.println(gps.satellites());
Serial.println();

gps.stats(&chars, &sentences, &failed_checksum);
}
}
}
