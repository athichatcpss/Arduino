
[![Arduino CI](https://github.com/RobTillaart/MCP23S17/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/MCP23S17/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/MCP23S17/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/MCP23S17/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/MCP23S17/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/MCP23S17.svg)](https://github.com/RobTillaart/MCP23S17/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/MCP23S17/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/MCP23S17.svg?maxAge=3600)](https://github.com/RobTillaart/MCP23S17/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/MCP23S17.svg)](https://registry.platformio.org/libraries/robtillaart/MCP23S17)


# MCP23S17

Arduino library for MCP23S17 16 channel SPI port expander.


## Description

This experimental library gives easy control over the 16 pins of a (SPI) MCP23S17 chip.

This IC is strongly related to the MCP23017 I2C port expander - https://github.com/RobTillaart/MCP23017_RT
Programming Interface is kept the same as much as possible.


#### 0.3.0 Breaking change

The version 0.3.0 has breaking changes in the interface. 
The essence is removal of ESP32 specific code from the library. 
This makes it possible to support the ESP32-S3 and other processors in the future. 
Also it makes the library a bit simpler to maintain.


#### Related

16 bit port expanders

- https://github.com/RobTillaart/MCP23017_RT
- https://github.com/RobTillaart/MCP23S17
- https://github.com/RobTillaart/PCF8575


8 bit port expanders

- https://github.com/RobTillaart/MCP23008
- https://github.com/RobTillaart/MCP23S08
- https://github.com/RobTillaart/PCF8574


## Interface

```cpp
#include "MCP23S17.h"
```

### Constructor

- **MCP23S17(uint8_t select, uint8_t dataIn, uint8_t dataOut, uint8_t clock, uint8_t address = 0x00)** constructor SOFTWARE SPI.
- **MCP23S17(uint8_t select, SPIClassRP2040\* spi)** constructor HARDWARE SPI with explicit SPI interface selected.
- **MCP23S17(uint8_t select, SPIClass\* spi)** constructor HARDWARE SPI with explicit SPI interface selected.
- **MCP23S17(uint8_t select, uint8_t address = 0x00, SPIClassRP2040\* spi = &SPI)** constructor HARDWARE SPI with optional address pins and SPI interface.
- **MCP23S17(uint8_t select, uint8_t address = 0x00, SPIClass\* spi = &SPI)** constructor HARDWARE SPI with optional address pins and SPI interface.
- **bool begin()** returns true if successful.
- **bool isConnected()** returns true if connected, false otherwise. (dummy for compatibility reasons)
- **uint8_t getAddress()** returns the address set in the constructor. 
Default = 0, range = 0..7.

The two hardware constructors allow to call 4 different constructors.

```cpp
- MCP23S17(10);            // select pin only
- MCP23S17(10, 7);         // select pin + address pins
- MCP23S17(10, 7, &SPI2);  // select pin + address pins + SPI port
- MCP23S17(10, &SPI2);     // select pin + SPI port
```


#### Sharing SELECT lines

(verified in #19)  
Technically two chips could use the same SELECT pin and a different address. 
Since 0.2.5 the constructors allow to setup such a configuration.
The added value is that one can use up to 8 devices (= 128 IO lines) with only 
four lines (MISO, MOSI, CLOCK, SELECT).

I assume that this configuration is less used and IMHO not recommended.
NB it is more difficult to detect which device is selected when debugging.

To use the hardware addresses the Hardware Address ENable register must be set.
See datasheet 3.3.2 ADDRESSING SPI DEVICES, need to set IOCON.HAEN.

The library supports two ways:
```cpp
MCP.enableControlRegister(MCP23S17_IOCR_HAEN);  //  or 0x08
or
MCP.enableHardwareAddress();  //  0.2.5 version and up
```

See also **IO Control Register** section below.


### Single pin interface

- **bool pinMode(uint8_t pin, uint8_t mode)** pin = 0..15, mode = INPUT, OUTPUT. Returns true if successful.
- **bool digitalWrite(uint8_t pin, uint8_t value)** pin = 0..15, value = LOW(0) HIGH (!0). Returns true if successful.
- **uint8_t digitalRead(uint8_t pin)** pin = 0..15, returns LOW or HIGH, might set the lastError();
- **bool setPolarity(uint8_t pin, bool reversed)** pin = 0..15, set reversed flag. Returns true if successful.
- **bool getPolarity(uint8_t pin, bool &reversed)** pin = 0..15, reads reversed flag. Returns true if successful.
- **bool setPullup(uint8_t pin, bool pullup)** pin = 0..15, set pull-up flag. Returns true if successful.
- **bool getPullup(uint8_t pin, bool &pullup)** pin = 0..15, reads pull-up flag. Returns true if successful.


### 8 pins interface

- **bool pinMode8(uint8_t port, uint8_t value)** port = 0..1, value = 0..255. Returns true if successful.
- **bool write8(uint8_t port, uint8_t value)** port = 0..1, value = 0..255. Returns true if successful.
- **uint8_t read8(uint8_t port)** port = 0..1, reads 8 pins into one byte.
- **bool setPolarity8(uint8_t port, uint8_t mask)** port = 0..1, sets polarity for 8 channels at once.
Returns true if successful.
- **bool getPolarity8(uint8_t port, uint8_t &mask)** port = 0..1, reads polarity of 8 channels at once.
Returns true if successful.
- **bool setPullup8(uint8_t port, uint8_t mask)** port = 0..1, sets pull-up for 8 channels at once.
Returns true if successful.
- **bool getPullup8(uint8_t port, uint8_t &mask)** port = 0..1, reads pull-up for 8 channels at once.
Returns true if successful.


### 16 pins interface

- **bool pinMode16(uint16_t value)** value = 0..0xFFFF, returns true if successful.
Returns true if successful.
- **bool write16(uint16_t value)** value = 0..0xFFFF, returns true if successful.
Returns true if successful.
- **uint16_t read16()** reads 16 pins into an uint16_t.
- **bool setPolarity16(uint16_t mask)** sets polarity for 16 channels.
Returns true if successful.
- **bool getPolarity16(uint16_t &mask)** reads polarity of 16 channels.
Returns true if successful.
- **bool setPullup16(uint16_t mask)** sets pull-up for 16 channels.
Returns true if successful.
- **bool getPullup16(uint16_t &mask)** reads pull-up for 16 channels.
Returns true if successful.

Since 0.2.6 the reading and writing to registers have been performance optimized for the 16 bit interface.
If there are problems please open an issue.


### IO Control Register

Since 0.2.3 the library supports setting bit fields in the IO control register.
Read the datasheet carefully!

- **void enableControlRegister(uint8_t mask)**
- **void disableControlRegister(uint8_t mask)**


|  constant              |  mask  |  description  |
|:-----------------------|:------:|:--------------|
|  MCP23S17_IOCR_BANK    |  0x80  |  Controls how the registers are addressed.
|  MCP23S17_IOCR_MIRROR  |  0x40  |  INT Pins Mirror bit.
|  MCP23S17_IOCR_SEQOP   |  0x20  |  Sequential Operation mode bit.
|  MCP23S17_IOCR_DISSLW  |  0x10  |  Slew Rate control bit for SDA output.
|  MCP23S17_IOCR_HAEN    |  0x08  |  Hardware Address Enable bit (MCP23S17 only).
|  MCP23S17_IOCR_ODR     |  0x04  |  Configures the INT pin as an open-drain output.
|  MCP23S17_IOCR_INTPOL  |  0x02  |  This bit sets the polarity of the INT output pin.
|  MCP23S17_IOCR_NI      |  0x01  |  Not implemented. 


Two dedicated functions are added since 0.2.5.

- **void enableHardwareAddress()** set IOCR_HAEN  bit.
- **void disableHardwareAddress()** clear IOCR_HAEN bit.


### Error codes

If one of the above functions return false, there might be an error.

- **int lastError()** Above functions set an error flag that can be read with this function.  
Reading it will reset the flag to **MCP23S17_OK**.

|  name                     |  value  |  description  |
|:--------------------------|:-------:|:--------------|
|  MCP23S17_OK              |  0x00   |  No error     |
|  MCP23S17_PIN_ERROR       |  0x81   |
|  MCP23S17_I2C_ERROR       |  0x82   |  (compatibility)
|  MCP23S17_VALUE_ERROR     |  0x83   |
|  MCP23S17_PORT_ERROR      |  0x84   |
|  MCP23S17_REGISTER_ERROR  |  0xFF   |  low level.


## Operation

See examples.


## Future

#### Must

- improve documentation

#### Should

- buy additional hardware
- keep functional in sync with MCP23017_RT
- test with multiple devices.
  - multi SELECT lines
- add example with interrupts
  - test 
- IOCON.HAEN, Hardware Address ENable.
  - should this be enabled in **begin()** by default?  0.3.0
  - check address range in constructor.

#### Could 

- check need for writing in all functions (Polarity / Pull-up)
  - check if bit mask changes.
  - what is performance gain vs footprint?
- investigate and reimplement the INPUT_PULLUP for pinMode() ?
- RP2040 support for SPI, setGPIOpins() etc
  - See MCP_DAC
- AVR software SPI optimize 0.3.0
  - dao and clock - see fastShiftOut.

#### Wont


## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,


