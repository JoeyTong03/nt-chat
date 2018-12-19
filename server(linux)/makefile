#test1-1 test1-2 makefile

compiler=gcc

SRCS :=$(wildcard *.c)
TARGET := server
OBJDIR = .

.c:
	gcc $(SRCS) -o $(TARGET) -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
#	$(compiler) -o $@ $<

all:$(TARGET) cleano

cleano:
	rm -rf *.o

.PHONY:clean
clean:
	rm -rf $(TARGET)
