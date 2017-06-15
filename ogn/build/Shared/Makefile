
CC = gcc
XX = g++

FGLAGS = -Wall -o -g

OBJS_PATH = ../../obj

LIB_PATH = ../../lib

INC_PATH = -I ../../src/Core/core/core/ \
		   -I ../../src/Core/core/core/math \
		   -I ../../src/Core/core/html/ \
		   -I ../../src/Core/core/json/include/json/ \
		   -I ../../src/Core/core/json/src/lib_json/ \
		   -I ../../src/Core/core/tinyxml/ \
		   -I ../../src/Core/core/zlib/ \
		   -I ../../src/Core/modules/libevent/include \
		   -I ../../src/Core/modules/libevent/WIN32-Code/nmake 

TARGET = Core.a

%.o : $(INC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
%.o : $(INC_PATH)%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
			  			  
$(TARGET):$(OBJS)
	    ar rcs $(TARGET) $^
		mv $(TARGET) $(LIB_PATH)
		
clean:
	rm -f *.o
	rm -f $(LIB_PATH)*