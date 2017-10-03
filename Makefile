CC = g++
CPP_VER = 11
CC_FLAGS = -g
CC_FLAGS += -std=c++$(CPP_VER)
CC_FLAGS += -c

MAIN=examples/*.cpp

SOURCE = src/*.cpp src/http/*.cpp src/http/responder/*.cpp
EXEC = exec.out

compile: 
	$(CC) $(SOURCE) $(CC_FLAGS)

test:
	./test/build_tests && ./test/run_tests || echo Error

main:
	$(CC) -std=c++$(CPP_VER) *.o $(MAIN) -I src/ -o $(EXEC)

run: 
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)