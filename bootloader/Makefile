OSNAME = CustomOS

OVMFDIR = ../OVMFbin
LDS = bootloader.ld
ASMC = nasm
CC = gcc

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone
ASMFLAGS = 
BOOTFILEFLAGS = -felf32 
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm) 
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

bootloader: $(OBJS) link

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@echo ===== COMPILING $^ =====
	@mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@echo ===== LINKING $^ =====
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/bootloader.elf $(OBJS)

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)