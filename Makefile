# Makefile to build libtft
#
# Simplified version from: https://github.com/hexagon5un/AVR-Programming

MAIN = libtft.c
SRC = font.c hack.c unifont.c

CC = avr-gcc
AR = avr-ar

CFLAGS += -O2 -I.
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums 
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -g -ggdb
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections -mrelax
CFLAGS += -std=gnu99
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105523
# CFLAGS += --param=min-pagesize=0
CFLAGS += -c

ARFLAGS = rcs

MAKEFLAGS += -r

TARGET = $(strip $(basename $(MAIN)))
SRC += $(TARGET).c

OBJ = $(SRC:.c=.o) 
OBJ = $(SRC:.S=.o)

$(TARGET).o: libtft.h font.h hack.h unifont.h types.h utils.h Makefile

all: $(TARGET).a
	
%.a: %.o
	$(AR) $(ARFLAGS) $(TARGET).a $(TARGET).o font.o hack.o unifont.o

%.o: $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -f $(TARGET).a $(TARGET).hex $(TARGET).obj \
	$(TARGET).o $(TARGET).d $(TARGET).eep $(TARGET).lst \
	$(TARGET).lss $(TARGET).sym $(TARGET).map $(TARGET)~ \
	$(TARGET).eeprom
