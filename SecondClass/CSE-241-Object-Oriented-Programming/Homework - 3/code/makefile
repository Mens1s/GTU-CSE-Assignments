all: clean compile run

compile: driverAutomatic.cpp driverManual.cpp tetris.hpp tetris.cpp tetromino.hpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ driverAutomatic.cpp -o test -std=c++11


compileAuto: driverAutomatic.cpp tetris.hpp tetris.cpp tetromino.hpp 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ driverAutomatic.cpp -o test -std=c++11
	./test

compileManual: driverManual.cpp tetris.hpp tetris.cpp tetromino.hpp 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ driverManual.cpp -o test -std=c++11
	./test

run:
	@echo "-------------------------------------------"
	@echo "Running the autonomous test...."
	@echo "================================================================================="
	./test
	@echo "================================================================================="
	@echo "Completed tests...."
	@echo "-------------------------------------------"
	@echo "-------------------------------------------"
	@echo "Running the manual test...."
	@echo "================================================================================="
	@g++ driverManual.cpp -o test -std=c++11
	./test
	

clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.o
	@rm -f test
