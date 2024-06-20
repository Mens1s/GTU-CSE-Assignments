all: clean compile run

compile: main.c 
	@echo "-------------------------------------------"
	@echo "Compiling..."
	@gcc -o student_grades main.c
run:
	@echo "-------------------------------------------"
	@echo "Running the tests...."
	@echo "================================================================================="
	./student_grades
	@echo "================================================================================="
	@echo "Completed tests...."

clean:
	@echo "-------------------------------------------"
	@echo "Removing compiled files..."
	@rm -f *.o
	@rm -f student_grades
	@rm -f *.txt