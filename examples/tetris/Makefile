# Mategame sample Makefile
#     Just drop it into an Arduino sketch folder and set some variables
#     to get it going. Just give it a good read.

help:
	@echo "Hello! You may want instructions to compile."
	@echo "This is a sample Makefile that should be dropped inside a game sketch folder and configured to build it."
	@echo ""
	@echo "Read the Makefile and configure your setup."
	@echo "There'll be comments to help you."
	@echo ""
	@echo "-> To compile and run an emulation on the computer type:"
	@echo ""
	@echo "    $ make mock"
	@echo ""
	@echo "-> To compile and upload to arduino type:"
	@echo ""
	@echo "    $ make upload"

# Variables to configure
# Where you downloaded mockvision
export MOCKVISION_PATH= ../../mockvision

ifndef MOCKVISION_PATH
$(error Please dont forget to set the MOCKVISION_PATH variable)
endif

# Where is the arduino IDE located
export ARDUINO_DIR = /usr/share/arduino/

# These libs are mostly used in the TVout games, they should be placed
# inside your arduino IDE 'libraries' folder
export ARDUINO_LIBS = EEPROM Controllers TVout

# Your board's name, find it in 'boards.txt' inside the arduino folder
export BOARD_TAG = atmega328

# Where the arduino is connected
export ARDUINO_PORT = /dev/ttyUSB0

# Mine only works with this baudrate, your mileage may vary
export AVRDUDE_ARD_BAUDRATE = 57600
export AVRDUDE_ARD_PROGRAMMER = arduino

# This is the name of your skectch, for example:
# If your skecth is called 'invaders.ino' it will be 'invaders'
export TARGET = tetris

# Additional cpp or c files that should be linked together (they should have no 'main' function)
export ADDITIONAL_TARGETS = bitmaps.o stc.o

upload:
	make -f $(MOCKVISION_PATH)/arduino.mk upload

mock:
	make -f $(MOCKVISION_PATH)/mockvision.mk

clean:
	make -f $(MOCKVISION_PATH)/arduino.mk clean
	make -f $(MOCKVISION_PATH)/mockvision.mk clean