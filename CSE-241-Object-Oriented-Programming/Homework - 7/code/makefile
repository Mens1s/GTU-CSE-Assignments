all: clean compile run

compile: Driver1.java Driver2.java Tetromino.java Tetris.java 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@javac Driver1.java


compileManual: Driver2.java Tetromino.java Tetris.java 
	@echo "-------------------------------------------"
	@echo "Compiling Driver2 for Automatic Play and Testing Functions..."
	@javac Driver2.java
	@java Driver2

compileAuto: Driver1.java Tetromino.java Tetris.java 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@javac Driver1.java
	@java Driver1

run:
	@echo "-------------------------------------------"
	@echo "Running the autonomous test...."
	@echo "================================================================================="
	@java Driver1
	@echo "================================================================================="
	@echo "Completed tests...."
	@echo "-------------------------------------------"
	@echo "-------------------------------------------"
	@echo "Running the manual test...."
	@echo "================================================================================="
	@javac Driver2.java
	@java Driver2
	

clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f Driver1
	@rm -f Driver2
