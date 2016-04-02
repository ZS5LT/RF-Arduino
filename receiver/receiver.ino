/* 
  RF Blink - Receiver sketch 
     Written by ScottC 17 Jun 2014
     Arduino IDE version 1.0.5
     Website: http://arduinobasics.blogspot.com
     Receiver: XY-MK-5V
     Description: A simple sketch used to test RF transmission/receiver.          
 ------------------------------------------------------------- */

#include <SPI.h>
#include <RH_ASK.h>
#include <Time.h>

RH_ASK driver;

#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 


 #define ledPin 13        //Onboard LED = digital pin 13
 unsigned int data = 0;   // variable used to store received data

struct dataStruct{
  float humidity;
  float temp;
  unsigned long counter;  
  long thetime; 
}myData;


byte tx_buf[sizeof(myData)] = {0};

char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}



void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  u8g.setFont(u8g_font_osb21);
  char a[10];
  //sprintf(a,"T: ");

  int tog=myData.counter % 2;

  if (tog == 1){  
    ftoa((char *)&a,myData.temp,1);
    u8g.drawStr( 0,28,a);
  } else {
    sprintf(a,"%d %d %d", hour(),minute(),second());
    u8g.drawStr(0,28,a);
  }
  char z[10];


  if (tog==1){ 
     sprintf(z,"%d",myData.counter);
} else {
  //sprintf(z,"H: ");
  ftoa((char *)&z,myData.humidity,1);
}
  u8g.drawStr(0,56,z);
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

 void setup(){
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);

  if (!driver.init())
    Serial.println("init failed");


     // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
 }

 void loop(){

 uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        int i;
        
        // Message with a good checksum received, dump it.
        driver.printBuffer("Got:", buf, buflen);
        memcpy(&myData, buf, sizeof(myData));
        setTime(myData.thetime);
        Serial.print("Temperature: ");
        Serial.println(myData.temp,2);
        Serial.print("Humidity: ");
        Serial.println(myData.humidity,2);
        Serial.print("Counter: ");
        Serial.println(myData.counter);
        Serial.print("Timer: ");
        Serial.println(myData.thetime);
        digitalClockDisplay();
    }

 Serial.print(".");
    // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  
 }

