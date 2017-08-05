CC = g++
CPP_VER = 11
CC_FLAGS = -g
CC_FLAGS += -std=c++$(CPP_VER)
CC_FLAGS += -c

SOURCE = src/*.cpp src/http/*.cpp
EXEC = exec.out

compile: 
	$(CC) $(SOURCE) $(CC_FLAGS)

main:
	make compile && $(CC) *.o -o $(EXEC)

run: 
	$(BLD)/$(EXEC)

clean:
	rm -f *.o $(EXEC)