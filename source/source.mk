# List of all the board related files.
SOURCESRC = ./source/i2c_eeprom.c \
			./source/sysParam.c \
			./source/usbcfg.c \
			$(CHIBIOS)/os/various/shell.c \
			$(CHIBIOS)/os/hal/lib/streams/chprintf.c
# Required include directories
SOURCEINC = ./include