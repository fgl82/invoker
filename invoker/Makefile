# Software Name
PROGRAM = invoker.dge
#PLATFORM = BITTBOY
#PLATFORM = RG-300
#PLATFORM = RG-350
#PLATFORM = NPG
PLATFORM = PC

# Compiler

ifeq ($(PLATFORM), BITTBOY)
	CC = /opt/bittboy-toolchain/bin/arm-buildroot-linux-musleabi-gcc
else ifeq ($(PLATFORM), RG-300)
	CC = /opt/new-toolchain/usr/bin/mipsel-linux-gcc
else ifeq ($(PLATFORM), RG-350)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
else ifeq ($(PLATFORM), NPG)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc	
else
	PROGRAM = invoker-x86.dge
	CC = gcc
endif

# You can use Ofast too but it can be more prone to bugs, careful.
ifeq ($(PLATFORM), BITTBOY)
	CFLAGS = -DTARGET_BITTBOY -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie -static
else ifeq ($(PLATFORM), RG-300)
	CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie	
else ifeq ($(PLATFORM), RG-350)
	CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -std=gnu99
else ifeq ($(PLATFORM), NPG)
	CFLAGS = -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -std=gnu99
else
	CFLAGS = -DTARGET_PC -O2 -fdata-sections -ffunction-sections -fno-PIC -flto -Wall -Wextra -no-pie	
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