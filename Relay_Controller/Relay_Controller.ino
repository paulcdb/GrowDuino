/* YourDuino Example: Relay Control 1.10
  Handles "Relay is active-low" to assure
  no relay activation from reset until
  application is ready.
   terry@yourduino.com 

Arduino Version: 1.8.1
*/

/*-----( Import needed libraries )-----*/
#include "RTClib.h"
#include "U8glib.h"

/*-----( Declare Constants )-----*/
#define RELAY_ON 0
#define RELAY_OFF 1

/*-----( Declare objects )-----*/
int Hour;
int Pump1;
int AMPM;

int ledOn = 7;
int ledOff = 19;

RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI

/*-----( Declare Variables )-----*/
#define Relay_1  6  // Arduino Digital I/O pin number
#define Relay_2  7
#define Relay_3  8
#define Relay_4  9
#define Relay_5  10
#define Relay_6  11
#define Relay_7  12
#define Relay_8  13



void setup()   /****** SETUP: RUNS ONCE ******/
{
//-------( Initialize Pins so relays are inactive at reset)----
  digitalWrite(Relay_1, RELAY_OFF);
  digitalWrite(Relay_2, RELAY_OFF);
  digitalWrite(Relay_3, RELAY_OFF);
  digitalWrite(Relay_4, RELAY_OFF);
  digitalWrite(Relay_5, RELAY_OFF); 
  digitalWrite(Relay_6, RELAY_OFF);
  digitalWrite(Relay_7, RELAY_OFF);
  digitalWrite(Relay_8, RELAY_OFF);

//---( THEN set pins as outputs )----  
  pinMode(Relay_1, OUTPUT);   
  pinMode(Relay_2, OUTPUT);  
  pinMode(Relay_3, OUTPUT);  
  pinMode(Relay_4, OUTPUT);
  pinMode(Relay_5, OUTPUT);
  pinMode(Relay_6, OUTPUT);
  pinMode(Relay_7, OUTPUT);
  pinMode(Relay_8, OUTPUT);
  delay(4000); //Check that all relays are inactive at Reset

//--(end setup )---
Serial.begin(9600);

if (! rtc.begin()) {
  Serial.println("Couldn't find RTC");
  while (1);
}

if (rtc.lostPower()) {
  Serial.println("RTC lost power, lets set the time!");
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2017 at 3am you would call:
  // rtc.adjust(DateTime(2017, 1, 21, 3, 0, 0));
}
delay(3000); // wait for console opening
}

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  DateTime now = rtc.now();
  Hour = now.hour();
  AMPM = now.hour();
  Pump1 = now.hour();
  Serial.println("Waiting until there's something to do... ");
  Serial.print("\t Hour: ");
  if (AMPM < 13) {
    Serial.print(AMPM);
    Serial.println("AM");
  }
  else if (AMPM > 12) {
    Serial.print(now.hour());
    Serial.print(" = ");
    Serial.print(AMPM - 12);
    Serial.println("PM");
  }
    Serial.print("\t Mode: ");
if (now.hour() >= ledOn && now.hour() < ledOff ) {
    Serial.println("Daytime");
  }
  else if (now.hour() >= ledOff ) { 
    Serial.println("Nighttime");
  } else {
    Serial.print("Error in Mode calculation");
  }

  Serial.print("\t Relay: 1 = ");
  Serial.print(digitalRead(Relay_1));
  Serial.print(" | 2 = ");
  Serial.print(digitalRead(Relay_2));
  Serial.print(" | 3 = ");
  Serial.print(digitalRead(Relay_3));
  Serial.print(" | 4 = ");
  Serial.print(digitalRead(Relay_4));
  Serial.print(" | 5 = ");
  Serial.print(digitalRead(Relay_5));
  Serial.print(" | 6 = ");
  Serial.print(digitalRead(Relay_6));
  Serial.print(" | 7 = ");
  Serial.print(digitalRead(Relay_7));
  Serial.print(" | 8 = ");
  Serial.println(digitalRead(Relay_8));
  
//---( Check the hour and turn on relay if its with the range)---
//---( LED Grow Lights (Seed Tray)---
if (now.hour() >= ledOn && now.hour() < ledOff & digitalRead(Relay_1) == RELAY_OFF) {
  digitalWrite(Relay_1, RELAY_ON);// set the Relay ON
  Serial.println(); Serial.println("Turn on LED Grow Lights - Seed Tray.");
  Serial.println("");
  } else if (now.hour() >= ledOff & digitalRead(Relay_1) == RELAY_ON) {
//---( Check the hour and turn off relay if it's outside the range)---  
  digitalWrite(Relay_1, RELAY_OFF);// set the Relay OFF
  Serial.println(); Serial.println("Turn off LED Grow Lights - Seed Tray.");
  Serial.println("");
  }

//---( Check the hour and turn on relay if its with the range)---
//---(Pump - Seed Tray)---
if (now.minute() >= 00 && now.minute() <= 15 & digitalRead(Relay_2) == RELAY_OFF) {
  digitalWrite(Relay_2, RELAY_ON);// set the Relay ON
  Serial.println(); Serial.println("Turn on Pump - Seed Tray.");
  Serial.println("");
} else if (now.minute() > 15 && now.minute() < 00 & digitalRead(Relay_2) == RELAY_ON) {
//---( Check the hour and turn off relay after 15 minutes)---
//---( LED Grow Lights (Seed Tray)---
  digitalWrite(Relay_2, RELAY_OFF);// set the Relay OFF
  Serial.println(); Serial.println("Turn off Pump - Seed Tray.");
  Pump1 = Pump1 + 1;
  Serial.println("");
}
  
delay(30000);
}//--(end main loop )---
