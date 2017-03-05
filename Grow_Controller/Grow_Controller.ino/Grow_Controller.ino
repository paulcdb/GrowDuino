/*
   Arduino Version: 1.8.1
*/

/*-----( Import needed libraries )-----*/
#include <DHT.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
#include <time.h>
#include <TimeAlarms.h>
#include <NTPClient.h>

/*
   Pin 1 -
   Pin 2 - One Wire Temp Probe
   Pin 3 - Ultrasound Sensor
   Pin 4 - Temp/Humidity Sensor ** CHANGE *
   Pin 5 - Float Sensor
   Pin 6 to 13 - Currently Unused
   Pin A0 -
   Pin A1 - pH Sensor
   Pin A2 -
*/

// Digital Pins
#define ONE_WIRE_BUS 2    // Connect to Pin 2
const int pingPin = 3;
#define DHTPIN 4          // what pin we're connected to
const byte switchPin = 5; // The pin for the Water Float Sensor
#define RELAY1  7         // LED Grow Lights
#define RELAY2  8        // Small Water Pump

// Analog Pins
#define SensorPin 1 //pH meter Analog output to Arduino Analog Input 1

#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
OneWire ourWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&ourWire); // Pass the oneWire reference to Dallas Temperature.

#define Offset 0.00 //deviation compensate
unsigned long int avgValue; //Store the average value of the sensor feedback
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x23, 0x36 }; //MAC address.
IPAddress ip(192, 168, 0, 118);
IPAddress gateway(192, 168, 0, 1);
EthernetClient client;
char server[] = "192.168.0.46"; // IP Adres (or name) of server to dump data to

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // start the Ethernet connection
  Ethernet.begin(mac, ip, gateway);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
    Serial.println("Dallas Temperature IC Control Library Demo");
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode (switchPin, INPUT);
  }

  // start clock
  // setTime(19,0,0,9,5,15); // set time to 19:00 Sep 5 2015 I want to use an RTC eventually......

  // Start up the library
  sensors.begin();
  
  dht.begin();
}



void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  // PH Sensor Code
  int buf[10]; //buffer for read analog
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        int temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 3.5 * phValue + Offset; //convert the millivolt into pH value

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("pH Level: ");
    Serial.print(phValue, 2);
    Serial.print(" \t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" \t");
    Serial.print("Room Temp: ");
    Serial.print(t);
    Serial.print(" *C");
    Serial.print(" \t");
    sensors.requestTemperatures();
    Serial.print("Probe Temp: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.print(" *C");
    Serial.print(" \t");
    sensors.requestTemperatures();
    Serial.print("Water Level: ");
    Serial.print(digitalRead (switchPin));
    Serial.println("");

    Serial.print("GET /grow/add_data.php?");
    Serial.print("controller=");
    Serial.print("Arduino01");
    Serial.print("&");
    Serial.print("ph=");
    Serial.print(phValue, 2);
    Serial.print("&");
    Serial.print("roomtemp=");
    Serial.print(t);
    Serial.print("&");
    Serial.print("watertemp=");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.print("&");
    Serial.print("humidity=");
    Serial.print(h);
    Serial.print("&");
    Serial.print("waterlevel=");
    Serial.print(digitalRead (switchPin));

    delay(10000);
  }
  // Connect to the web server
  // URL should be: add_data.php?controller=name&ph=phlevel&roomtemp=roomtemp&watertemp=watertemp&humidity=humidity&waterlevel=waterlevel)
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    client.print("GET /grow/add_data.php?");
    client.print("controller=");
    client.print("Arduino01");
    client.print("&");
    client.print("ph=");
    client.print(phValue, 2);
    client.print("&");
    client.print("roomtemp=");
    client.print(t);
    client.print("&");
    client.print("watertemp=");
    client.print(sensors.getTempCByIndex(0));
    client.print("&");
    client.print("humidity=");
    client.print(h);
    client.print("&");
    client.print("waterlevel=");
    client.print(digitalRead (switchPin));
    client.println( " HTTP/1.1");
    client.print( "Host: " );
    client.println(server);
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
  }
  // If Arduino can't connect to the server (your computer or web page)
  Serial.println("--> connection failed\n");

  // Give the server some time to recieve the data and store it. Be advised when delaying.
  // If u use a short delay, the server might not capture data because of Arduino transmitting new data too soon.
  delay(600000);
}

