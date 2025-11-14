C_SRC = kernel.c
KERN_OBJ = kernel.o

all:
	gcc -c -fno-builtin -ffreestanding -nostdlib -m32 $(C_SRC) -o $(KERN_OBJ)

clean:
	rm -f $(KERN_OBJ)

.PHONY: clean