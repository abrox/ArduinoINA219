cmake_minimum_required (VERSION 2.6)
project (ina219)
set(SMQTT_DIR "/home/odroid/src/simplemqtt")

add_executable(ina219 ina219_test.cpp ../ina219.cpp)


FIND_LIBRARY(SMQTT_LIBRARY smqtt ${SMQTT_DIR}/lib)

target_link_libraries (ina219 LINK_PUBLIC ${SMQTT_LIBRARY} paho-mqtt3c)

target_include_directories (ina219 PUBLIC ${SMQTT_DIR}/src)

