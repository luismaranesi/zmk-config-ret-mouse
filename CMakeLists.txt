cmake_minimum_required(VERSION 3.20.0)

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  zephyr_library()
  # Custom behaviors use ZMK's public-in-practice behavior driver interface,
  # which is kept in the ZMK application's include directory in v0.3.
  zephyr_library_include_directories(${ZEPHYR_ZMK_MODULE_DIR}/app/include)
  zephyr_library_sources(src/behavior_portable_accent.c)
endif()
