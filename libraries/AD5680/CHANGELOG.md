# Change Log AD5680

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).


## [0.2.0] - 2023-11-26
- refactor constructor interface - breaking changes.
  - minimize conditional code. -- create SPI_CLASS macro to solve it.
- reordered parameters software SPI constructor.
- update readme.md
- update examples

----

## [0.1.1] - 2023-09-22
- fix SPI_MODE to SPI_MODE0 for hardware SPI.
- fix **updateDevice()**
- add **AD5680_triangle.ino** example
- add **AD5680_sawtooth.ino** example


## [0.1.0] - 2023-09-19
- initial version

