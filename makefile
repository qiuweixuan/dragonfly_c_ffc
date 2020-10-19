DEBUG = -g
EXTERN_LIB =  -L/usr/local/openssl/lib -lgmp  -lcrypto
INCLUDE = -I./include/ 
CXX = gcc
TARGET = dragonfly_main
SRC = $(wildcard ./src/*.c)
ROOT_SRC = $(wildcard ./*.c)
SRC_OBJ = $(patsubst %.c, %.o, $(SRC))
ROOT_OBJ = $(patsubst %.c, %.o, $(ROOT_SRC))

TEST_SRC = $(wildcard ./test/*.c)
TEST_TARGET= $(patsubst %.c, %, $(TEST_SRC))


 
.PHONY: all
all:  $(TARGET) 

$(TARGET):$(SRC_OBJ) $(ROOT_OBJ) 
	$(CXX) $^ -std=c11 -o $@ $(EXTERN_LIB)

#编译当前目录下的.c文件
%.o:%.c
	$(CXX) $(DEBUG) -std=c11 -c $< -o $@ $(INCLUDE)


.PHONY: test test-clean
test:  $(TEST_TARGET)

$(TEST_TARGET):%:%.c $(SRC_OBJ) 
	$(CXX) $^ -std=c11 -o $@ $(EXTERN_LIB)

test-clean:
	-rm -rf $(TEST_TARGET)


#防止外面有clean文件，阻止执行clean
.PHONY:clean

clean:
	-rm -rf $(TARGET) $(TEST_TARGET) $(SRC_OBJ) $(ROOT_OBJ)
