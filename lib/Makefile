C_SRC = $(wildcard *.c)

all:
	gcc -c -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding $(C_SRC) 

clean:
	rm -f $(O_OBJ)

.PHONY: clean all