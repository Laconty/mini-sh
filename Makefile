CC=gcc
target=minish
SRC = $(wildcard src/*.c)

$(target): $(SRC)
	$(CC) -o $@ $^

clean: 
	rm -f $(target)

.PHONY: clean
