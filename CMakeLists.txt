cmake_minimum_required(VERSION 3.20.0)

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  zephyr_library()
  zephyr_library_sources(src/behavior_portable_accent.c)
endif()
