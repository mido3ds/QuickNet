# auto-generated makefile

.PHONY: all cmake build run

all: cmake build run

cmake:
	cmake -H. -B"/mnt/c/Users/Mahmoud/Practice/QuickNet/build" || echo ">>>> cmake initialization failed! <<<<<"

build:
	cmake --build "/mnt/c/Users/Mahmoud/Practice/QuickNet/build" || echo ">>>> build failed! <<<<<"

run:
	"/mnt/c/Users/Mahmoud/Practice/QuickNet/build/QuickNet/QuickNet" 5000

clean:
	cmake --build "/mnt/c/Users/Mahmoud/Practice/QuickNet/build" --target clean || echo ">>>> clean failed! <<<<<"

cleanAll:
		del Makefile && rd /s /q build && rd /s /q CMakeFiles || rm -rf build CMakeFiles Makefile
