TARGET = Solum.iso
KELF = kernel.elf
LINKSCR = linker.ld
BOOT_OBJ = boot/boot.o
KERN_OBJ = kernel/kernel.o
INFO_OBJ = boot/info.o
BOOT_S = boot/boot.s
KERN_C = kernel/kernel.c
INFO_C = boot/info.c
LIB_C = $(shell find lib/ -name "*.c")
LIB_OBJ = $(patsubst %.c, %.o, $(LIB_C))

$(TARGET): $(KELF) grub.cfg
	cp grub.cfg ISODir/boot/grub/grub.cfg
	cp $(KELF) ISODir/SolumOS/$(KELF)
	grub-mkrescue -o Solum.iso ISODir/

$(KELF): $(BOOT_OBJ) $(KERN_OBJ) $(INFO_OBJ) $(LIB_OBJ) $(LINKSCR)
	ld -n -T $(LINKSCR) -o $(KELF) $(BOOT_OBJ) $(KERN_OBJ) $(INFO_OBJ) $(LIB_OBJ)

$(BOOT_OBJ): $(BOOT_S)
	make -C boot BOOT_O

$(INFO_OBJ): $(INFO_C)
	make -C boot INFO_O

$(KERN_OBJ): $(KERN_C)
	make -C kernel KERN_O

$(LIB_OBJ): $(LIB_C)
	make -C lib all

clean: 
	make -C boot clean
	make -C kernel clean
	make -C lib clean
	rm -f $(TARGET)
	rm -f $(KELF)

debug_B:
	make
	qemu-system-x86_64 -cdrom $(TARGET) -m 1G -serial stdio

debug_U:
	make
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -cdrom $(TARGET) -m 1G -serial stdio

.PHONY: run debug_B debug_U