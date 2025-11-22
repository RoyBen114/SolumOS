C_SRC = $(shell find . -name "*.c")
O_OBJ = $(C_SRC:%.c=%.o)

all:
	gcc -c -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -mno-red-zone -ffreestanding $(C_SRC) -o $(O_OBJ)

clean:
	rm -f $(O_OBJ)

.PHONY: clean