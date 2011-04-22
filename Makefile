TARGET = assembler
CFLAGS = -Wall -g


all:$(TARGET)

assembler:assembler.c assembler.h
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)
