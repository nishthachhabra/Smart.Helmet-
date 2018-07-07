/*=========================================================================================
 *                                        LDR
  =========================================================================================
*/
// These constants won't change:
const int sensorPin = A0;    // pin that the sensor is attached to
const int ledPin = 8;        // pin that the LED is attached to

// variables:
int sensorValue = 0;         // the sensor value

/*=========================================================================================
 *                                        MPU6050
  =========================================================================================
*/

#include <Wire.h>
#include "MPU6050.h"

MPU6050 mpu;

boolean ledState = false;
boolean freefallDetected = false;
int freefallBlinkCount = 0;

/*=========================================================================================
 *                                        MQ6
  =========================================================================================
*/

const int buzzer= 7;        // Buzzer pin to Arduino pin 9
const int gasSensor = A1;    // Gas sensor MQ-2 signal pin to nodeMCU analog A0 pin

//Variables
int gasValue;

/*=========================================================================================
 *                                        MQ3
  =========================================================================================
*/

const int AOUTpin=A2;//the AOUT pin of the alcohol sensor goes into digital pin A2 of the arduino
const int mq3Out=13;//the anode of the LED connects to digital pin D5 of the arduino

int value;

/*=========================================================================================
 *                                        DHT22
  =========================================================================================
*/

#include <dht.h>

#define dataPin A3 // Defines pin number to which the sensor is connected
dht DHT; // Creates a DHT object

/*=========================================================================================
 *                                        LCD
  =========================================================================================
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte nosmiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b00000
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte noheart[8] = {
  0b01010,
  0b10101,
  0b10001,
  0b10101,
  0b10101,
  0b10001,
  0b01010,
  0b00100
};

byte nolight[8] = {
  0b01110,
  0b11011,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};

byte light[8] = {
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

/*=========================================================================================
 *                                        SETUP
  =========================================================================================
*/

void setup() {
  Serial.begin(115200);
  
  /*=========================================================================================
 *                                        LDR
  =========================================================================================
*/
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  /*=========================================================================================
 *                                        MPU6050
  =========================================================================================
*/
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  mpu.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  mpu.setIntFreeFallEnabled(true);
  mpu.setIntZeroMotionEnabled(false);
  mpu.setIntMotionEnabled(false);
  
  mpu.setDHPFMode(MPU6050_DHPF_5HZ);

  mpu.setFreeFallDetectionThreshold(17);
  mpu.setFreeFallDetectionDuration(2);  
  
  checkSettings();

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  
  attachInterrupt(0, doInt, RISING);

  /*=========================================================================================
 *                                        MQ6
  =========================================================================================
*/
  pinMode(buzzer,OUTPUT);

  /*=========================================================================================
 *                                        MQ3
  =========================================================================================
*/

  pinMode(AOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(mq3Out, OUTPUT);//sets the pin as an output of the arduino

  /*=========================================================================================
 *                                        DHT22
  =========================================================================================
*/

  pinMode(dataPin, INPUT);
  delay(2000);

  /*=========================================================================================
 *                                        LCD
  =========================================================================================
*/
  lcd.begin(16, 2);
  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, nosmiley);
  lcd.createChar(3, noheart);
  lcd.createChar(4, light);
  lcd.createChar(5, nolight);
}

/*=========================================================================================
 *                                        MPU6050
  =========================================================================================
*/

void doInt()
{
  freefallBlinkCount = 0;
  freefallDetected = true;  
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:                ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Motion Interrupt:     ");
  Serial.println(mpu.getIntMotionEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Zero Motion Interrupt:     ");
  Serial.println(mpu.getIntZeroMotionEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Free Fall Interrupt:       ");
  Serial.println(mpu.getIntFreeFallEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Free Fal Threshold:          ");
  Serial.println(mpu.getFreeFallDetectionThreshold());

  Serial.print(" * Free FallDuration:           ");
  Serial.println(mpu.getFreeFallDetectionDuration());
  
  Serial.print(" * Clock Source:              ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:             ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets:     ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());

  Serial.print(" * Accelerometer power delay: ");
  switch(mpu.getAccelPowerOnDelay())
  {
    case MPU6050_DELAY_3MS:            Serial.println("3ms"); break;
    case MPU6050_DELAY_2MS:            Serial.println("2ms"); break;
    case MPU6050_DELAY_1MS:            Serial.println("1ms"); break;
    case MPU6050_NO_DELAY:             Serial.println("0ms"); break;
  }  
  
  Serial.println();
}

/*=========================================================================================
 *                                        LOOP
  =========================================================================================
*/

void loop() {
  // read the sensor:
  /*=========================================================================================
 *                                        LDR
  =========================================================================================
*/
  sensorValue = analogRead(sensorPin);
  Serial.print(sensorValue);
  Serial.print("  ");

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.print(sensorValue);
  Serial.print("  ");

  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);
  Serial.println(sensorValue);

  lcd.setCursor(4, 0);

  if(sensorValue > 235){
    digitalWrite(ledPin, HIGH);
    lcd.write(byte(4));
  }
  else{
    digitalWrite(ledPin, LOW);
    lcd.write(byte(5));
  }

  /*=========================================================================================
 *                                        MPU6050
  =========================================================================================
*/
  Vector rawAccel = mpu.readRawAccel();
  Activites act = mpu.readActivites();

  Serial.print(act.isFreeFall);
  Serial.print("\n");
  
  if (freefallDetected)
  {
    lcd.setCursor(6, 0);
    lcd.print("!");
    
    ledState = !ledState;

    digitalWrite(10, ledState);

    freefallBlinkCount++;

    if (freefallBlinkCount == 20)
    {
      freefallDetected = false;
      ledState = false;
      digitalWrite(10, ledState);
    }
  }

  /*=========================================================================================
 *                                        MQ6
  =========================================================================================
*/

  // Read and store values to aditional variables 
  gasValue   = 400 + analogRead(gasSensor);
 
  Serial.println(gasValue);
  lcd.setCursor(0, 0);
  // Dangerous gas if
  if (gasValue >= 550 && gasValue < 600){
    lcd.write(byte(2));
    tone(buzzer, 1000);
    delay(150);
    tone(buzzer, 350);
    delay(150);
    tone(buzzer,0);
  }
  else if (gasValue >= 600 && gasValue < 700){
    lcd.write(byte(2));
    tone(buzzer, 1000);
    delay(100);
    tone(buzzer, 350);
    delay(100);
  }
  else if (gasValue >= 700 && gasValue < 800){
    lcd.write(byte(2));
    tone(buzzer, 1000);
    delay(75);
    tone(buzzer, 350);
    delay(75);
  }
  else if (gasValue >= 800 && gasValue < 900){
    lcd.write(byte(2));
    tone(buzzer, 1000);
    delay(50);
    tone(buzzer, 350);
    delay(50);
  }
  else if (gasValue >= 900 && gasValue < 1000){
    lcd.write(byte(2));
    tone(buzzer, 1000);
    delay(25);
    tone(buzzer, 350);
    delay(25);
  }
  else if (gasValue >= 1000){
    lcd.write(byte(2));
    tone(buzzer, 1000);
  }
  else
  {
    lcd.write(byte(1));
    noTone(buzzer);
    }

  /*=========================================================================================
 *                                        MQ3
  =========================================================================================
*/
  value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
  Serial.println(value);
  lcd.setCursor(2, 0);
 if(value<450)
  {
    lcd.write(byte(0));
      digitalWrite(mq3Out, LOW);
      Serial.println("You are sober.");
  }
  if (value>=450 && value<630)
  {
    lcd.write(byte(3));
    digitalWrite(mq3Out, HIGH);
      Serial.println("Alcohol detected");
  }
  if (value>=630 && value<700)
  {
    lcd.write(byte(3));
    digitalWrite(mq3Out, HIGH);
      Serial.println("More than one drink going on here....");
  }
  if (value>=700 && value <800)
  {
    lcd.write(byte(3));
    digitalWrite(mq3Out, HIGH);
      Serial.println("Serious Booze here! ");
  }
  if(value>800)
  {
    lcd.write(byte(3));
    digitalWrite(mq3Out, HIGH);
     Serial.println("You are drunk!");
  }

  /*=========================================================================================
 *                                        DHT22
  =========================================================================================
*/

  int readData = DHT.read22(dataPin); // Reads the data from the sensor
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity
  
  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("*C and ");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println("%");
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print((int)t);
  lcd.print((char)223);
  lcd.print("C ");
  lcd.print("H: ");
  lcd.print((int)h);
  lcd.print("%");
}
