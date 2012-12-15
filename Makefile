CC=gcc
CFLAGS=-Wall -g
TARGET=test
default:$(TARGET)
SOURCE=rbtree.c rb_test.c

all : $(TARGET)

%o:%c
	$(CC)  $(CFLAGS) -o $@ -c $< 

$(TARGET) : rbtree.o rb_test.o
	$(CC) -o $@  $^

.PHONY: all clean
clean:
	rm -rf *.o
	rm -rf $(TARGET)

