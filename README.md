# Fancy Updatable Configureable Wifi IoT Thingy (mk2)
A horrifying Frankenstein-esque mishmash of half a dozen Arduino example sketches.
Intended primarily for the [Witty Cloud MCU](http://www.icstation.com/esp8266-serial-wifi-witty-cloud-development-board-module-mini-nodemcu-p-8154.html).


Things this should do:
1. Automatically connect to wifi networks, as defined in the 'Wifi.ino' file.
2. Scan the resulting wifi network using mDNS to find an [MQTT server](http://www.instructables.com/id/Installing-MQTT-BrokerMosquitto-on-Raspberry-Pi/).
3. Accept commands on an MQTT channel, and put the responses on a different channel.
4. Allow OTA updates via a built-in HTTP server, so you don't have to keep unplugging it.


# Future plans:
1. Make it automatically reconnect to Wifi if it loses signal.
2. Add a command to read the on-board LDR.
3. Instead of having oddly specialised commands for two random GPIO pins, just allow any of them to be set unless they're otherwise engages (I2C, the RGB LED pins, etc).
4. Nick the smooth colour-changing code out of my kitchen lights.
5. EEPROM-save for the current state, and any settings.
6. Web interface rather than just plain old MQTT.
7. Have the debugging serial interface accept the same commands as the MQTT interface.
8. Make the Button work even if MQTT/Wifi is disconnected.
