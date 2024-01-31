SHELL := /bin/bash

ifndef HOST_CC
    HOST_CC = gcc
    HOST_CFLAGS = -Wall -Wno-incompatible-library-redeclaration -I../common -O2
endif

ifndef CROSS

    # ARCH = rv32e
		ARCH = rv32i
    
    # ABI = ilp32e
		ABI = ilp32
    
    #TODO: better way to switch little/big-endian:
    #ENDIAN = _le # gcc for little-endian
    #ENDIAN = _be # gcc for big-endian

    # CROSS = riscv32-embedded$(ENDIAN)-elf
    #CROSS = riscv-elf
		CROSS = riscv32-unknown-elf
    #CROSS = riscv32-embedded-elf
    
		CCPATH = /opt/riscv/bin
    # CCPATH = /usr/local/share/gcc-$(CROSS)/bin/
    #CCPATH = /usr/local/share/toolchain-$(CROSS)/bin
		HARVARD = 1
endif

ifndef BUILD
    BUILD = $(shell date -R)
endif

    CC = $(CCPATH)/$(CROSS)-gcc
    AS = $(CCPATH)/$(CROSS)-as
    LD = $(CCPATH)/$(CROSS)-ld
    AR = $(CCPATH)/$(CROSS)-ar
    RL = $(CCPATH)/$(CROSS)-ranlib
    OC = $(CCPATH)/$(CROSS)-objcopy
    OD = $(CCPATH)/$(CROSS)-objdump
   CPP = $(CCPATH)/$(CROSS)-cpp

CCFLAGS = -Wall -fcommon -O2 #-ffreestanding 
CCFLAGS += -march=$(ARCH) -mabi=$(ABI)
CCFLAGS += -D__RISCV__ -DBUILD="\"$(BUILD)\"" -DARCH="\"$(ARCH)\""
#CCFLAGS += -DSMALL # enable this to make FW smaller!
ASFLAGS = -march=$(ARCH)
ARFLAGS = -rcs
LDFLAGS = -Map=$(PROJ).map -m elf32lriscv -static -gc-sections# --entry=_start -Ttext=0 -T$(PROJ).ld
CPFLAGS = -P 

CCFLAGS += -mcmodel=medany -mexplicit-relocs # relocable clode

ifdef HARVARD
	CPFLAGS +=  -DHARVARD=1
endif

ifdef SMALL
    CCFLAGS += -DSMALL
endif

#OCFLAGS = -O binary # --reverse-bytes=4 # workaround for darkriscv big-endian blockrams
#ODFLAGS = -D

PROJ = nakedshell
LIBS = 
OBJS = main.o
ASMS = main.s
SRCS = main.c


DEPS = $(SRCS) $(ASMS) $(OBJS) $(LIBS) 
TARGETS = $(PROJ).a

.PHONY: all

all: $(TARGETS) $(DEPS)
	@echo sources ok for $(TARGETS).

clean:
	-rm -f $(ASMS) $(OBJS) $(PROJ).{S,bin,lst,map,ram,rom,x86,text,data,bin,ld,o,mem,rom.mem,ram.mem,a} $(PROJ)_uart.bin

%.o: %.s Makefile
	$(AS) $(ASFLAGS) -c $< -o $@

%.s: %.c Makefile
	$(CC) $(CCFLAGS) -S $< -o $@ -lc -v

$(PROJ).a: $(OBJS) Makefile
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RL) $@
	
$(PROJ).x86: $(SRCS)
	$(HOST_CC) $(HOST_CFLAGS) $(SRCS) -o $(PROJ).x86 -DBUILD="\"$(BUILD)\"" -DARCH="\"x86\""
	@echo x86 ok.
