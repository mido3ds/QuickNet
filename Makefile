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
	echo '> Compiling Tests... Please make sure src is compiled, or run `make compile`' && cd test && g++ -std=c++$(CPP_VER) -g -O0 ./*.cpp ./http/*.cpp ./http/responder/*.cpp ../*.o -o run_tests -LCPUnit/lib -ICPUnit/src -I../src -lCPUnit $* && echo '> Running Tests...' && ./run_tests || echo '> Error Occurred'

main:
	$(CC) -std=c++$(CPP_VER) *.o $(MAIN) -I src/ -o $(EXEC)

run: 
	./$(EXEC)

clean:
	rm -f *.o $(EXEC)