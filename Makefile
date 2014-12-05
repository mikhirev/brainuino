BOARD_TAG = nano328
MONITOR_PORTS = /dev/ttyUSB*
ARDUINO_LIBS = LiquidCrystal
ARDUINO_MK = /usr/share/arduino/Arduino.mk

include $(ARDUINO_MK)

tags: *.cpp *.h *.ino
	ctags --language-force=C++ $^
