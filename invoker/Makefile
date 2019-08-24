# Software Name
PROGRAM = invoker.elf
PLATFORM = BITTBOY
#PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
CC = /opt/bittboy-toolchain/bin/arm-miyoo-linux-musleabi-gcc
else ifeq ($(PROFILE), RS97)
CC = /opt/rs97-toolchain/bin/mipsel-linux-gcc
else
CC = gcc
endif

# You can use Ofast too but it can be more prone to bugs, careful.
ifeq ($(PLATFORM), BITTBOY)
CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie -static
else
CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie
endif

CFLAGS +=  -I.

LIBS += 
LDFLAGS = -Wl,--start-group $(LIBS) -Wl,--end-group -Wl,--as-needed -Wl,--gc-sections -flto

ifeq ($(DEBUG), YES)
CFLAGS +=  -DDEBUG -g3
else
LDFLAGS	+=  -s
endif

CFILES =  invoker.c
SFILES = 

OFILES = $(SFILES:.S=.o) $(CFILES:.c=.o)

$(PROGRAM):	$(OFILES)
			$(CC) $(CFLAGS) $(OFILES) -o $@ $(LDFLAGS)

all: $(PROGRAM)

%.o: %.c
	 $(CC) $(CFLAGS) -c $< -o $@

clean:
	 -rm -f $(OFILES) $(MAPFILE) $(PROGRAM)