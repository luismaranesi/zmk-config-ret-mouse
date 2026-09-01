cmake_minimum_required(VERSION 3.20.0)

if(CONFIG_ZMK_BLE AND ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
  # Custom behaviors use ZMK's public-in-practice behavior driver interface,
  # which is kept in the ZMK application's include directory in v0.3.
  target_include_directories(app PRIVATE ${ZEPHYR_ZMK_MODULE_DIR}/app/include)
  target_sources(app PRIVATE src/behavior_portable_accent.c)
endif()
