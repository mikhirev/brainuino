BOARD_TAG = nano328
MONITOR_PORTS = /dev/ttyUSB*
ARDUINO_LIBS = LiquidCrystal

include /usr/share/arduino/Arduino.mk

tags: *.cpp *.h *.ino
	ctags --language-force=C++ $^
