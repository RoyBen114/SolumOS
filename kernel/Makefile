C_SRC = kernel.c
KERN_OBJ = kernel.o

KERN_O:
	gcc -c -I../include -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding $(C_SRC) -o $(KERN_OBJ)

clean:
	rm -f $(KERN_OBJ)

.PHONY: clean KERN_O