
all: Teensy_ESC_Interface.eep Teensy_ESC_Interface.hex

ARDUINO_PATH := /home/cedric/bin/arduino-1.0.5
CC := $(ARDUINO_PATH)/hardware/tools/arm-none-eabi/bin/arm-none-eabi-gcc
CXX := $(ARDUINO_PATH)/hardware/tools/arm-none-eabi/bin/arm-none-eabi-g++
OBJCOPY := $(ARDUINO_PATH)/hardware/tools/arm-none-eabi/bin/arm-none-eabi-objcopy

CFLAGS := -g -Wall -ffunction-sections -fdata-sections -DF_CPU=48000000 -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -nostdlib -D__MK20DX128__ -DTIME_T=1372950027 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -Iteensy3

CPPFLAGS := -Os -mcpu=cortex-m4 -mthumb

CXXFLAGS := -g -Wall -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=48000000 -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=105 -nostdlib -D__MK20DX128__ -fno-rtti -felide-constructors -std=gnu++0x -DUSB_SERIAL -DLAYOUT_US_ENGLISH -Iteensy3

LDLIBS := -lm

LDFLAGS := -Os -mcpu=cortex-m4 -mthumb -Wl,--gc-sections -Tteensy3/mk20dx128.ld

CXX_FILES := \
    teensy3/IPAddress.cpp \
    teensy3/IPAddress.cpp \
    teensy3/Print.cpp \
    teensy3/HardwareSerial2.cpp \
    teensy3/Tone.cpp \
    teensy3/HardwareSerial1.cpp \
    teensy3/WMath.cpp \
    teensy3/usb_inst.cpp \
    teensy3/Stream.cpp \
    teensy3/main.cpp \
    teensy3/HardwareSerial3.cpp \
    teensy3/WString.cpp \
    teensy3/usb_flightsim.cpp \
    teensy3/IntervalTimer.cpp \
    Teensy_ESC_Interface.cpp

CC_FILES := \
    teensy3/usb_midi.c \
    teensy3/usb_mouse.c \
    teensy3/touch.c \
    teensy3/eeprom.c \
    teensy3/usb_dev.c \
    teensy3/yield.c \
    teensy3/keylayouts.c \
    teensy3/mk20dx128.c \
    teensy3/usb_mem.c \
    teensy3/analog.c \
    teensy3/serial2.c \
    teensy3/usb_joystick.c \
    teensy3/serial3.c \
    teensy3/serial1.c \
    teensy3/pins_teensy.c \
    teensy3/nonstd.c \
    teensy3/usb_rawhid.c \
    teensy3/usb_seremu.c \
    teensy3/usb_desc.c \
    teensy3/usb_keyboard.c \
    teensy3/usb_serial.c

Teensy_ESC_Interface.elf: $(patsubst %.c,%.o,$(CC_FILES)) $(patsubst %.cpp,%.o,$(CXX_FILES))
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

Teensy_ESC_Interface.eep: Teensy_ESC_Interface.elf
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@

Teensy_ESC_Interface.hex: Teensy_ESC_Interface.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
