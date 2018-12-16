CC := gcc #编译器
TARGET1 := tcp_server1
TARGET2 := tcp_client1
SOURCE1 := server.c
SOURCE2 := client.c

.PHONY:all #多目标
all:$(TARGET1) $(TARGET2)
	
$(TARGET1):$(SOURCE1)
	$(CC) -o $(TARGET1) $(SOURCE1)
$(TARGET2):$(SOURCE2)
	$(CC) -o $(TARGET2) $(SOURCE2)

#清除可执行文件
.PHONY:clean
clean:
	rm -f $(TARGET1) $(TARGET2)
