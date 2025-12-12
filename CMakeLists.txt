# LoRaWAN Helper Library for Zephyr
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_LORAWAN_HELPER)

zephyr_library()

zephyr_library_sources(
  src/LoRaWAN.cpp
)

# Make headers available to application code
zephyr_include_directories(include)

endif()
