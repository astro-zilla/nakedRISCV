
#APPLICATION = hello
 APPLICATION = nakedshell
# APPLICATION = coremark
# APPLICATION = badapple
# APPLICATION = mandelbrot

OS_IFACE = libc_naked

# un-select SMALL for coremark!
# export SMALL = 1# uncomment for smaller firmware

SHELL := /bin/bash

ifndef HOST_CC
    HOST_CC = gcc
    HOST_CFLAGS = -Wall -Wno-incompatible-library-redeclaration -I../common -O2
endif

ifndef CROSS

    export ARCH = rv32i
    #ARCH = rv32e
    
    export ABI = ilp32
    #ABI = ilp32e
    
    #TODO: better way to switch little/big-endian:
    #ENDIAN = _le # gcc for little-endian
    #ENDIAN = _be # gcc for big-endian

		export CROSS = riscv32-unknown-elf
    # export CROSS = riscv32-embedded$(ENDIAN)-elf
    #CROSS = riscv-elf
    #CROSS = riscv32-unknown-elf
    #CROSS = riscv32-embedded-elf
    
		export CCPATH = /opt/riscv/bin
    # export CCPATH = /usr/local/share/gcc-$(CROSS)/bin/
    #CCPATH = /usr/local/share/toolchain-$(CROSS)/bin
		HARVARD = 1
endif

ifndef BUILD
    BUILD = $(shell date -R)
endif

    CC  = $(CCPATH)/$(CROSS)-gcc
    AS  = $(CCPATH)/$(CROSS)-as
    RL = $(CCPATH)/$(CROSS)-ranlib
    LD  = $(CCPATH)/$(CROSS)-ld
    OC  = $(CCPATH)/$(CROSS)-objcopy
    OD  = $(CCPATH)/$(CROSS)-objdump
    CPP = $(CCPATH)/$(CROSS)-cpp

CCFLAGS = -Wall -fcommon -O2 #-ffreestanding 
CCFLAGS += -march=$(ARCH) -mabi=$(ABI)
CCFLAGS += -D__RISCV__ -DBUILD="\"$(BUILD)\"" -DARCH="\"$(ARCH)\"" -DMISSING_SYSCALL_NAMES
ASFLAGS = -march=$(ARCH)
LDFLAGS = -T$(PROJ).ld -Map=$(PROJ).map -m elf32lriscv -static -gc-sections --entry=_start 
LDFLAGS += -L/opt/riscv/riscv32-unknown-elf/lib -L/opt/riscv/lib/gcc/riscv32-unknown-elf/13.2.0 -L$(OS_IFACE) # -Ttext=0 
LDLIBS  = $(LIBS) 
CPFLAGS = -P 

CCFLAGS += -mcmodel=medany -mexplicit-relocs # relocable clode

ifdef HARVARD
	CPFLAGS +=  -DHARVARD=1
	export HARVARD
endif

OCFLAGS = -O binary # --reverse-bytes=4 # workaround for darkriscv big-endian blockrams
ODFLAGS = -D

PROJ = logisimv
LIBS = $(APPLICATION)/$(APPLICATION).a $(OS_IFACE)/$(OS_IFACE).a
OBJS = boot.o
ASMS = boot.s
SRCS = 

ifdef SMALL
    CPFLAGS += -DSMALL=1
endif

# uncomment for hardware mul
#export MAC = 1

ifdef MAC
    CFLAGS += -DMAC=1
endif

DEPS = $(SRCS) $(ASMS) $(OBJS) $(PROJ).ld $(PROJ).lds $(LIBS) $(APPLICATION)/$(APPLICATION).a

ifdef HARVARD
	TARGETS = $(PROJ).rom.mem $(PROJ).ram.mem
else
	TARGETS = $(PROJ).mem
endif

.PHONY: all

all: $(TARGETS) $(DEPS)
	-echo build is ok for $(TARGETS)

$(APPLICATION)/$(APPLICATION).a:
	make -C $(APPLICATION) all

$(OS_IFACE)/$(OS_IFACE).a:
	make -C $(OS_IFACE) all

clean:
	make -C $(APPLICATION) clean
	make -C $(OS_IFACE) clean
	-rm -f $(OBJS) $(PROJ).{lss,bin,lst,map,ram,rom,x86,text,data,bin,ld,o,mem,rom.mem,ram.mem} $(PROJ)_uart.bin

%.o: %.s Makefile
	$(AS) $(ASFLAGS) -c $< -o $@

%.s: %.c Makefile
	$(CC) $(CCFLAGS) -S $< -o $@ 

$(PROJ).ld: $(PROJ).lds Makefile
	$(CPP) $(CPFLAGS) $(PROJ).lds $(PROJ).ld
	
$(PROJ).o: $(OBJS) $(PROJ).ld $(LIBS)
	$(RL) $(LDLIBS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) -lg_nano -lgcc -lc_naked -lg_nano
	$(OD) $(ODFLAGS) $@ > $(PROJ).lss

$(PROJ).rom.mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).text --only-section .*text* 
	# hexdump -ve '1/4 "%08x\n"' $(PROJ).text > $@
	echo 'v3.0 hex bytes little-endian' > $@
	xxd -p -c 4 -g 4 $(PROJ).text >> $@
	rm $(PROJ).text
	wc -l $@
	@echo rom ok.

$(PROJ).ram.mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).data --only-section .*data* --only-section .*bss*
	# hexdump -ve '1/4 "%08x\n"' $(PROJ).data > $@
	@echo 'v3.0 hex bytes little-endian' > $@
	xxd -p -c 4 -g 4 $(PROJ).data >> $@
	rm $(PROJ).data
	wc -l $@
	@echo ram ok.

$(PROJ).mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).bin
	# hexdump -ve '1/4 "%08x\n"' $(PROJ).bin > $@
	# uncomment for little-endian
	@echo 'v3.0 hex bytes little-endian' > $@
	xxd -p -c 4 -g 4 $< >> $@
	dd if=$(PROJ).bin of=$(PROJ)_uart.bin skip=512 bs=1
	rm $(PROJ).bin
	# uncomment for MIF file
	#awk 'BEGIN { print "DEPTH = 1024;\nWIDTH = 32;\nADDRESS_RADIX = HEX;\nDATA_RADIX = HEX;\nCONTENT\nBEGIN" } { printf("%08x : %s\n",NR-1,$1); } END { print "END;" }' < $(PROJ).mem > $(PROJ).mif
	wc -l $@
	@echo mem ok.
	
$(PROJ).x86: $(SRCS)
	$(HOST_CC) $(HOST_CFLAGS) $(SRCS) -o $(PROJ).x86 -DBUILD="\"$(BUILD)\"" -DARCH="\"x86\""
	@echo x86 ok.
