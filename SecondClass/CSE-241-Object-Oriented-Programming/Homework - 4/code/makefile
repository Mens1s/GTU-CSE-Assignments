all: clean compile run

compile: main.cpp
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@g++ main.cpp -o test -std=c++11



run:
	@echo "-------------------------------------------"
	@echo "Running the autonomous test...."
	@echo "================================================================================="
	./test
	@echo "================================================================================="
	@echo "Completed tests...."


clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.o
	@rm -f test
