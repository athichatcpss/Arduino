//
//    FILE: AM2315C_lcd.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: Demo for AM2315C I2C humidity & temperature sensor
//

//  Always check datasheet
//
//                    +-----------------+
//    RED    -------- | VDD             |
//    YELLOW -------- | SDA    AM2315C  |
//    BLACK  -------- | GND             |
//    WHITE  -------- | SCL             |
//                    +-----------------+


#include "AM2315C.h"

AM2315C DHT;  //  default Wire

uint8_t count = 0;

uint32_t stop, start;


///////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7

#define  BL_OFF         0
#define  BL_ON          1

/*
  #define BACKLIGHT_PIN  7
  #define En_pin  4
  #define Rw_pin  5
  #define Rs_pin  6
  #define D4_pin  0
  #define D5_pin  1
  #define D6_pin  2
  #define D7_pin  3

  #define LED_OFF 0
  #define LED_ON  1
*/


int displayAddress = 0x27;

// lcd object is created as a placeholder as the actual address is determined in setupDisplay()
// constructor needs an I2C address. Better solution would be a function to set the address
// runtime in the class.
LiquidCrystal_I2C  lcd(displayAddress);


void setupDisplay()
{
  lcd = LiquidCrystal_I2C(displayAddress, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
  lcd.begin(20, 4);
  // lcd.setBacklightPin(BACKLIGHT_PIN, NEGATIVE);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(BL_ON);
}


void display()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.setCursor(6, 0);
  lcd.print(DHT.getTemperature());

  lcd.setCursor(0, 1);
  lcd.print("HUM:");
  lcd.setCursor(6, 1);
  lcd.print(DHT.getHumidity());

  lcd.setCursor(0, 3);
  lcd.print("TIME:");
  lcd.setCursor(6, 3);
  lcd.print(stop - start);
}


/////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AM2315C LIBRARY VERSION: ");
  Serial.println(AM2315C_LIB_VERSION);
  Serial.println();

  Wire.begin();
  DHT.begin();

  setupDisplay();

  delay(1000);
}


void loop()
{
  if (millis() - DHT.lastRead() >= 1000)
  {
    //  READ DATA
    start = micros();
    int status = DHT.read();
    stop = micros();

    display();

    if ((count % 10) == 0)
    {
      count = 0;
      Serial.println();
      Serial.println("Type\tHumidity (%)\tTemp (°C)\tTime (µs)\tStatus");
    }
    count++;

    Serial.print("AM2315C \t");
    //  DISPLAY DATA, sensor has only one decimal.
    Serial.print(DHT.getHumidity(), 1);
    Serial.print("\t\t");
    Serial.print(DHT.getTemperature(), 1);
    Serial.print("\t\t");
    Serial.print(stop - start);
    Serial.print("\t\t");
    switch (status)
    {
      case AM2315C_OK:
        Serial.print("OK");
        break;
      case AM2315C_ERROR_CHECKSUM:
        Serial.print("Checksum error");
        break;
      case AM2315C_ERROR_CONNECT:
        Serial.print("Connect error");
        break;
      case AM2315C_MISSING_BYTES:
        Serial.print("Missing bytes");
        break;
      case AM2315C_ERROR_BYTES_ALL_ZERO:
        Serial.print("All bytes read zero");
        break;
      case AM2315C_ERROR_READ_TIMEOUT:
        Serial.print("Read time out");
        break;
      case AM2315C_ERROR_LASTREAD:
        Serial.print("Error read too fast");
        break;
      default:
        Serial.print("Unknown error");
        break;
    }
    Serial.print("\n");
  }
}


//  -- END OF FILE --
