S_SRC = boot.s
C_SRC = info.c
BOOT_OBJ = boot.o
INFO_OBJ = info.o

all:
	make BOOT_O
	make INFO_O

BOOT_O:
	nasm -f elf64 $(S_SRC) -o $(BOOT_OBJ)

INFO_O:
	gcc -c -I../include -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding $(C_SRC) $(INFO_OBJ)

clean:
	rm -f $(BOOT_OBJ)
	rm -f $(INFO_OBJ)

.PHONY: clean BOOT_O INFO_O all