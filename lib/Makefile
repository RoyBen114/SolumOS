C_SRC = $(shell find . -name "*.c")
O_OBJ = $(wildcard *.o)

all:
	gcc -c -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding $(C_SRC) 

clean:
	rm -f $(O_OBJ)

.PHONY: clean all