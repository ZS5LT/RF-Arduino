 /* 
 ------------------------------------------------------------- */

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}

#include <dht11.h>
#include <Time.h>  

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    /

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
dht11 DHT11;

#define DHT11PIN 9

struct dataStruct{
  float humidity;
  float temp;
  unsigned long counter;
  long thetime;   
}myData;

byte tx_buf[sizeof(myData)] = {0};

 #define ledPin 13        //Onboard LED = digital pin 13
#define Relay 7

 void setup(){
   Serial.begin(9600);
   pinMode(ledPin, OUTPUT);   
   if (!driver.init())
       Serial.println("init failed");
 
   digitalWrite(Relay,HIGH);
   pinMode(Relay,OUTPUT);
   setTime(16,06,00,04,11,2015); //epoch
   
 }

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

 int Rel = 0;
 
 void loop(){

  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
    Serial.println("OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }

    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);
    Serial.print("Temperature (°F): ");
    Serial.println(Fahrenheit(DHT11.temperature), 2);
    digitalClockDisplay();

myData.temp=Fahrenheit(DHT11.temperature);
myData.humidity=DHT11.humidity;
myData.counter++;
myData.thetime=now();

    memcpy(tx_buf, &myData, sizeof(myData) );
    byte zize=sizeof(myData);

    driver.send((uint8_t *)tx_buf, zize);
    driver.waitPacketSent();
    delay(2000);

Serial.print("Hour is: "); Serial.println(hour());

if ((hour()>=6) && (hour()<8)){
  if (Rel==0) {
    Serial.print("Setting Relay on..");
    digitalWrite(Relay,LOW);
    Rel=1;
  } else {
    Serial.print("Relay should be on..1");
  }
} else if ((hour()>=17) && (hour()<20)) {
  if (Rel==0){
    Rel = 1; 
    Serial.print("Setting Relay on.2");
    digitalWrite(Relay,LOW);
  } else {
    Serial.print("Relay should be on..3");  
  }
} else if (Rel == 1) {
    digitalWrite(Relay,HIGH);
    Serial.println("setting relay off..4");
    Rel=0;
} else {
  Serial.println("relay is off..");
}


}


 
