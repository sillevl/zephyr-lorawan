# LoRaWAN Helper Library for Zephyr

A reusable C++ library for Zephyr RTOS that simplifies LoRaWAN OTAA connections with compile-time configuration.

## Features

- 🎯 **Simple C++ API** - Clean object-oriented wrapper around Zephyr's LoRaWAN stack
- 🔐 **Compile-time Credentials** - Hex string to byte array conversion at compile time
- ⚙️ **Kconfig Integration** - Configuration through Zephyr's standard Kconfig system
- 📦 **Zephyr Module** - Standard Zephyr module structure for easy reuse
- 🚀 **OTAA Support** - Full Over-The-Air Activation support

## Installation

### Option 1: Local Module (Current Setup)

The module is already set up in your project at `lib/lorawan_helper/`. It's automatically included via `CMakeLists.txt`.

### Option 2: As a Git Submodule

```bash
cd your-zephyr-project
git submodule add https://github.com/yourusername/zephyr-lorawan-helper.git lib/lorawan_helper
```

### Option 3: Via West Manifest (Recommended for GitHub)

Add to your `west.yml` or create one in your project root:

```yaml
manifest:
  remotes:
    - name: your-remote
      url-base: https://github.com/yourusername

  projects:
    - name: zephyr-lorawan-helper
      remote: your-remote
      path: lib/lorawan_helper
      revision: main

  self:
    path: .
```

Then run:
```bash
west update
```

## Usage

### 1. Enable in prj.conf

```ini
# Enable the library
CONFIG_LORAWAN_HELPER=y

# Base requirements
CONFIG_LORAWAN=y
CONFIG_LORA=y
CONFIG_LORA_SX127X=y  # Or your LoRa radio driver
```

### 2. Configure Credentials

Create `prj_extra.conf` (not tracked in git) with your credentials:

```ini
CONFIG_LORAWAN_DEV_EUI="70B3D57ED005XXXX"
CONFIG_LORAWAN_JOIN_EUI="0000000000000000"
CONFIG_LORAWAN_APP_KEY="YOUR32CHARACTERHEXSTRINGHERE"
```

### 3. Use in Your Application

```cpp
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <LoRaWAN.h>
#include <lorawan_config.h>

int main(void)
{
    // Get LoRa device
    const struct device *lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
    
    // Create LoRaWAN instance
    DevBit::LoRaWAN lorawan(lora_dev);
    
    // Load credentials from Kconfig (compile-time)
    auto keys = DevBit::get_lorawan_keys();
    
    // Connect using OTAA
    if (lorawan.connect(keys) != 0) {
        return -1;
    }
    
    // Send data
    uint8_t data[] = {0x01, 0x02, 0x03};
    lorawan.send(data, sizeof(data), 2, false);
    
    return 0;
}
```

### 4. Update CMakeLists.txt

```cmake
# Add module before find_package(Zephyr)
list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/lorawan_helper
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(your_project)
```

## API Reference

### `DevBit::LoRaWAN` Class

#### Constructor
```cpp
LoRaWAN(const struct device* dev)
```
Creates a LoRaWAN instance with the specified LoRa device.

#### Methods

**`int connect(const LoRaWANKeys& keys)`**
- Connects to the LoRaWAN network using OTAA
- Returns: 0 on success, negative error code on failure

**`void send(const uint8_t* data, size_t len, uint8_t port, bool confirmed)`**
- Sends data to the network
- `port`: Application port (1-223)
- `confirmed`: Whether to request acknowledgment

### Helper Functions

**`LoRaWANKeys get_lorawan_keys()`**
- Returns credentials loaded from Kconfig
- Performs compile-time hex string to byte array conversion

## Configuration Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `CONFIG_LORAWAN_HELPER` | bool | n | Enable the library |
| `CONFIG_LORAWAN_DEV_EUI` | string | "00...00" | Device EUI (16 hex chars) |
| `CONFIG_LORAWAN_JOIN_EUI` | string | "00...00" | Join/App EUI (16 hex chars) |
| `CONFIG_LORAWAN_APP_KEY` | string | "00...00" | App Key (32 hex chars) |

## Project Structure

```
lib/lorawan_helper/
├── CMakeLists.txt          # Build configuration
├── Kconfig                 # Configuration options
├── README.md              # This file
├── zephyr/
│   └── module.yml         # Zephyr module metadata
├── include/
│   ├── LoRaWAN.h         # Main class header
│   └── lorawan_config.h  # Compile-time config helpers
└── src/
    └── LoRaWAN.cpp       # Implementation
```

## Publishing to GitHub

To make this module available publicly:

1. **Create a new repository** on GitHub (e.g., `zephyr-lorawan-helper`)

2. **Initialize git in the module directory**:
```bash
cd lib/lorawan_helper
git init
git add .
git commit -m "Initial commit of LoRaWAN helper module"
git branch -M main
git remote add origin https://github.com/yourusername/zephyr-lorawan-helper.git
git push -u origin main
```

3. **Update your main project** to use west or git submodules (see Installation options above)

4. **Add a license** (Apache-2.0 recommended for Zephyr projects)

## Dependencies

- Zephyr RTOS (tested with v3.x)
- `CONFIG_LORAWAN=y`
- `CONFIG_LORA=y`
- A LoRa radio driver (e.g., SX1276/RFM95)

## License

Apache-2.0 (or your preferred license)

## Contributing

Contributions welcome! Please submit issues and pull requests on GitHub.

## Author

DevBit - Your embedded LoRaWAN solutions
