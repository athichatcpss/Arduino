#pragma once
//
//    FILE: AD9833.h
//  AUTHOR: Rob Tillaart
// PURPOSE: Arduino library for AD9833 function generator.
// VERSION: 0.2.0
//     URL: https://github.com/RobTillaart/AD9833


#include "Arduino.h"
#include "SPI.h"


#define AD9833_LIB_VERSION     (F("0.2.0"))


#if defined(ARDUINO_ARCH_RP2040)
#define __SPI_CLASS__   SPIClassRP2040
#else
#define __SPI_CLASS__   SPIClass
#endif


#define AD9833_MAX_FREQ       (12500000UL)  //  12.5 MHz.
#define AD9833_MAX_PHASE      (360.0)


//  MODE OPERANDI
#define AD9833_OFF            0
#define AD9833_SINE           1
#define AD9833_SQUARE1        2
#define AD9833_SQUARE2        3
#define AD9833_TRIANGLE       4


class AD9833
{
public:
  //       slaveSelect == selectPin == fsyncPin
  //  HARDWARE SPI
  AD9833(uint8_t slaveSelect, __SPI_CLASS__ * mySPI = &SPI);
  //  SOFTWARE SPI
  AD9833(uint8_t slaveSelect, uint8_t spiData, uint8_t spiClock);

  void     begin();

  void     reset();
  void     hardwareReset();
  //       mode = 0..3 (datasheet)
  bool     setPowerMode(uint8_t mode = 0);


  void     setWave(uint8_t waveform = AD9833_OFF);
  uint8_t  getWave();


  //       returns frequency set
  //       0 .. 12.5 MHz == AD9833_MAX_FREQ
  float    setFrequency(float freq, uint8_t channel = 0);
  float    getFrequency(uint8_t channel = 0);
  float    getMaxFrequency();
  void     setFrequencyChannel(uint8_t channel);

  //       returns phase set (degrees)
  //       [0 .. 360>
  float    setPhase(float phase, uint8_t channel = 0);
  float    getPhase(uint8_t channel = 0);
  float    getMaxPhase();
  void     setPhaseChannel(uint8_t channel);


  //       Hardware SPI settings
  void     setSPIspeed(uint32_t speed);  //  speed in Hz
  uint32_t getSPIspeed();
  bool     usesHWSPI();


  //  LOW LEVEL API - Expert users only
  void     writeControlRegister(uint16_t value);
  void     writeFreqRegister(uint8_t reg, uint32_t freq);
  void     writePhaseRegister(uint8_t reg, uint16_t value);


private:
  void     writeData(uint16_t data);

  bool     _hwSPI    = true;
  uint32_t _SPIspeed = 8000000;

  __SPI_CLASS__ * _mySPI;
  SPISettings     _spi_settings;

  //  PINS
  uint8_t  _dataPin   = 0;
  uint8_t  _clockPin  = 0;
  uint8_t  _selectPin = 0;
  bool     _useSelect = false;

  //  SIGNAL
  uint16_t _control   = 0;
  uint8_t  _waveform  = AD9833_OFF;

  float    _freq[2]   = { 0, 0 };  //  Hz
  float    _phase[2]  = { 0, 0 };  //  angle 0..360
};


//  --  END OF FILE --

