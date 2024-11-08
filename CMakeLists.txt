# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(myproj)

# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/mosquitto/client.crt" TEXT)
# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/mosquitto/client.key" TEXT)
# target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/mosquitto/broker.crt" TEXT)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/emqx/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/emqx/client.key" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/emqx/broker.crt" TEXT)