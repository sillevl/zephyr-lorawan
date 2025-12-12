# Changelog

All notable changes to the LoRaWAN Helper Library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2025-12-12

### Added
- Initial release of LoRaWAN Helper Library
- C++ wrapper class for Zephyr's LoRaWAN stack
- Compile-time hex string to byte array conversion
- Kconfig integration for credentials configuration
- OTAA (Over-The-Air Activation) support
- Automatic DevNonce generation
- Downlink callback handling
- Data rate change callback handling
- Full Zephyr module structure support

### Features
- Simple API: `connect()` and `send()` methods
- Thread-safe singleton pattern for callbacks
- Namespace-based organization (DevBit::LoRaWAN)
- Comprehensive documentation and examples
- Compatible with Zephyr 3.x and 4.x

### Documentation
- Complete README with usage examples
- GitHub publishing guide (PUBLISHING.md)
- Apache 2.0 License
- Example west.yml configuration

[Unreleased]: https://github.com/yourusername/zephyr-lorawan-helper/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/yourusername/zephyr-lorawan-helper/releases/tag/v1.0.0
