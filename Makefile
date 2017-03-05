.PHONY: clean
CC=gcc
target=minish

$(target): mini-sh.c mini-sh.h main.c
	@echo "compiling..."
	$(CC) -o $@ mini-sh.c main.c

clean:
	rm $(target)
