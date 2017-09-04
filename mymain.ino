#include <Adafruit_GFX.h>//Display
#include <Adafruit_PCD8544.h>//Display
#include <DHT.h>//Temp&Umidity DHT11
#include <RTClib.h>//Clock
#include <SPI.h>//Display
#include <Wire.h>//Clock I2C

#define DHTTYPE DHT11   // DHT 11 
#define DHTPIN 2     // what pin we're connected to
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
#define fotoresistenza A0
#define DAY 7
#define NIGHT 21


Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);//display
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
DHT dht(DHTPIN, DHTTYPE);//temp&umidity sensor
RTC_DS1307 RTC;//clock
float h = 0;//humidity
float t = 0;//temp
int led = 13;//status led
int ledPow = 12;//power led
int lux = 0;
char buffer1[20];//display buffer
DateTime now;//dateTime


void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {//if date isn't set
    RTC.adjust(DateTime(__DATE__, __TIME__));//set the date-time
  }

  //Init leds and photoresistence
  pinMode(led, OUTPUT);
  pinMode(fotoresistenza, INPUT);
  pinMode(ledPow, OUTPUT);
  
  //set leds
  digitalWrite(ledPow, LOW);
  digitalWrite(led, HIGH);
  
  //turn off status led
  delay(500);
  digitalWrite(led, LOW);

  //try to init dht
  delay(500);
  digitalWrite(led, HIGH);
  dht.begin();
  delay(500);
  digitalWrite(led, LOW);
  
  //try to init display
  delay(500);
  digitalWrite(led, HIGH);
  display.begin();
  delay(500);
  digitalWrite(led, LOW);
  
  // clean display
  delay(500);
  display.clearDisplay();
  
  //display.println("Hello world");
  //display.display();
  digitalWrite(led, HIGH);
  delay(500);
}

void loop() {
  now = RTC.now();
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lux = (int)analogRead(fotoresistenza);
  //Serial.println(lux, DEC);
      digitalWrite(ledPow, HIGH);

  /*if ((lux <= 60) && (DAY < now.hour()) && (now.hour() < NIGHT)) {
    digitalWrite(ledPow, HIGH);
  } else {
    digitalWrite(ledPow, LOW);
  }
  */
  
  sprintf(buffer1,  "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  //Serial.println("temperature:");
  //Serial.println(t);
  //Serial.println("\nhumidity");
  //Serial.println(h);
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);

  display.println(buffer1);
  display.print("Umid:\t"); display.println(h);


  display.print("Temp:\t"); display.println(t);
  display.print("Lux:\t"); display.println(lux);

  //display.setTextColor(WHITE, BLACK); // 'inverted' text
  //display.println(3.141592);
  //display.setTextSize(2);
  //display.setTextColor(BLACK);
  //display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  display.clearDisplay();
  //write temp on sd
  //envalue temp -> light on/of
  //read light
  //write light
  //envalue light -> light on/off
  //show temp & light on display
  delay(1000);
}
