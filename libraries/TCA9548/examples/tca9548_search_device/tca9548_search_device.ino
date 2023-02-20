//
//    FILE: tca9548_search_device.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo TCA9548 I2C multiplexer
//     URL: https://github.com/RobTillaart/TCA9548


#include "TCA9548.h"

TCA9548 MP(0x70);

uint8_t searchAddress = 0x38;   //  dummy, adjust to your needs.


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.println();

  Wire.begin();
  if (MP.begin() == false)
  {
    Serial.println("Could not connect to TCA9548 multiplexer.");
  }

  Serial.println("\nScan the channels of the multiplexer for searchAddress.\n");
  for (int chan = 0; chan < 8; chan++)
  {
    MP.selectChannel(chan);
    bool b = MP.isConnected(searchAddress);
    Serial.print("CHAN: ");
    Serial.print(chan);
    Serial.print("\t");
    Serial.print( b ? "found!" : "x");
  }
  Serial.println();

  Serial.println("done...");
}


void loop()
{
}


//  -- END OF FILE --
