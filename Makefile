NAME = pgui

#PANDORA = 1
#PSMS = 1
#PNEOPOP = 1
PGNGEO = 1

ifdef PANDORA
CC        = arm-none-linux-gnueabi-gcc
STRIP     = arm-none-linux-gnueabi-strip
else
CC        = gcc
STRIP     = strip
endif

ifdef PANDORA
CFLAGS    += -Wall -Werror `arm-none-linux-gnueabi-sdl-config --cflags` -DPANDORA
LIBS	  = -lconfig_pnd -lpnd_pnd -lSDL_image -lSDL_ttf -lSDL_gfx -lfreetype `arm-none-linux-gnueabi-sdl-config --libs` -lpng -lstdc++ -lts -ldl -lz -lm -static
else
CFLAGS    += -Wall -Werror `sdl-config --cflags`
LIBS	  = -lconfig_pc -lpnd_pc -lSDL_image -lSDL_ttf -lSDL_gfx -lfreetype `sdl-config --libs` -lstdc++
endif

CFLAGS  += -I. -Iinclude
LDFLAGS   = -Llib

ifdef PSMS
OBJECTS   += psms/emu.o
CFLAGS  += -Ipsms
endif
ifdef PNEOPOP
OBJECTS   += pneopop/emu.o
CFLAGS  += -Ipneopop
endif
ifdef PGNGEO
OBJECTS   += pgngeo/emu.o
CFLAGS  += -Ipgngeo
endif

OBJECTS   += main.o graphics.o mouse.o config.o sort.o

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
	$(STRIP) $(NAME)
ifdef PSMS
	cp psms/backg.bmp data
endif
ifdef PNEOPOP
	cp pneopop/backg.bmp data
endif
ifdef PGNGEO
	cp pgngeo/backg.bmp data
endif

.cc.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(NAME)
