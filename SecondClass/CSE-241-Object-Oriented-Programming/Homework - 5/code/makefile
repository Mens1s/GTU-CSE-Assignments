all: clean compile run

compile: Automatic.cpp Manual.cpp 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ Automatic.cpp -o test -std=c++11


compileAuto: Automatic.cpp 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ Automatic.cpp -o test -std=c++11
	./test

compileManual: Manual.cpp 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ Manual.cpp -o test -std=c++11
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
	@g++ Manual.cpp -o test -std=c++11
	./test
	

clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.o
	@rm -f test

