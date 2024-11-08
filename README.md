# myProj

ESP-IDF project to test the use of **Espressif framework** that can be seen [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

## Cloning the project

This project is in INESC TEC's GitLab: [https://gitlab.inesctec.pt/helio.mendonca/myproj](https://gitlab.inesctec.pt/helio.mendonca/myproj)

After cloning the project execute the follwing ESP-IDF commands:

* `ESP-IDF: Add vscode configuration folder` to create the .vscode folder
with its contents, and add the following line to c_cpp_properties.json file:

```text
            "compileCommands": "${workspaceFolder}/build/compile_commands.json",
```

* `ESP-IDF: SDK Configuration editor (menuconfig)` (icon: gear wheel)
create sdkconfig using `sdkconfig.defaults` with `partitions.csv`

* `ESP-IDF Set Espressif device target` (icon near the esp32 text)

* `ESP-IDF Build project` (icon <img src="docs/database-outline.svg" width="15"
style="background-color:white;"/> )

* `ESP-IDF Select flash method` (icon: star) and choose `UART`

* `ESP-IDF: Select port to use` (icon: plug) and choose the `COMx` (in Windows)
or `\dev\ttyUSBx` (in Linux)

## Add Component

In [ESPRegistry](https://components.espressif.com/) we can search components
to be used in our project.

In VSCode we can run command `ESP-IDF: Show Component Registry`
and search a component. For instance:

* [espressif/button](https://components.espressif.com/components/espressif/button)

* [espressif/led_indicator](https://components.espressif.com/components/espressif/led_indicator)

* [espressif/arduino-esp32](https://components.espressif.com/components/espressif/arduino-esp32)

Then hit Install button that add a `managed_components` folder to the project
and also a `idf_component.yml` file with list of components in the project.

## Other

Other subjects to solve:

* [Wi-Fi Provisioning](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/provisioning/wifi_provisioning.html)

* [ESP-MQTT](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html)
(w/ SSL)

* [Over The Air Updates (OTA)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/ota.html)

## Get IP

```text
wget -qO- http://icanhazip.com
```
