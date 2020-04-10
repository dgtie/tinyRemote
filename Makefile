BIN = /home/sylam/local/avr/bin
GPP = $(BIN)/avr-g++
OBJCOPY = $(BIN)/avr-objcopy
OBJDUMP = $(BIN)/avr-objdump
SIZE = $(BIN)/avr-size
UPDI = /home/sylam/pyupdi/pyupdi.py

SRC = main.cpp code.cpp kernel.cpp usart.cpp \
      nec.cpp sharp.cpp sony.cpp remote.cpp rc.cpp key.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = out

MCU = attiny414
CFLAGS = -mmcu=$(MCU) -Os
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

all: $(TARGET).hex

flash: $(TARGET).hex
	$(UPDI) -b115200 -c/dev/ttyUSB0 -dtiny414 -f$<

clean:
	rm -f *.o $(TARGET).hex $(TARGET).map $(TARGET).lst

%.hex: %.elf
	$(OBJDUMP) -S $< > $(TARGET).lst
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
	$(SIZE) $<

%.elf: $(OBJ)
	$(GPP) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(GPP) $(CFLAGS) -o $@ -c $<

.SECONDARY: $(OBJ)
