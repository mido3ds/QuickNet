# auto-generated makefile

.PHONY: all cmake build run

all: cmake build run

cmake:
	cmake -G"Visual Studio 14 2015 Win64" -H. -B"C:/Users/Mahmoud/Practice/QuickNet/build" || echo ">>>> cmake initialization failed! <<<<<"

build:
	cmake --build "C:/Users/Mahmoud/Practice/QuickNet/build" || echo ">>>> build failed! <<<<<"

run:
	"C:/Users/Mahmoud/Practice/QuickNet/build/QuickNet/Debug/QuickNet.exe"

clean:
	cmake --build "C:/Users/Mahmoud/Practice/QuickNet/build" --target clean || echo ">>>> clean failed! <<<<<"

cleanAll:
		del Makefile && rd /s /q build && rd /s /q CMakeFiles || rm -rf build CMakeFiles Makefile
