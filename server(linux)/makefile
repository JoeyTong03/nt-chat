#test1-1 test1-2 makefile

compiler=g++

SRCS :=$(wildcard *.c *.cpp)
TARGET := server
OBJDIR = .

.c:
	g++ $(SRCS) -o $(TARGET) -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
#	$(compiler) -o $@ $<

all:$(TARGET) cleano

cleano:
	rm -rf *.o

.PHONY:clean
clean:
	rm -rf $(TARGET)

#gcc db.c -o db  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
#gcc db.c -o getcmsg  -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient


