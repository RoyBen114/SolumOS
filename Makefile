TARGET = Solum.iso
OBJ = boot/boot.o kernel/kernel.o
KELF = kernel.elf

$(TARGET): $(KELF)
	cp grub.cfg ISODir/boot/grub/grub.cfg
	cp $(KELF) ISODir/SolumOS/$(KELF)
	grub-mkrescue -o Solum.iso ISODir/

$(KELF): $(OBJ)
	ld -m elf_i386 -n -T linker.ld -o $(KELF) $(OBJ)

$(OBJ):
	make -C boot all
	make -C kernel all

clean: 
	make -C boot clean
	make -C kernel clean
	rm -f $(TARGET)
	rm -f $(KELF)

run:
	make clean
	make
	qemu-system-x86_64 -cdrom $(TARGET) -m 1G

.PHONY: run clean