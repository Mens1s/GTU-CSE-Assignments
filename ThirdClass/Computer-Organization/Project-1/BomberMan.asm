.data
		
GetRowString:	.asciiz "\nEnter the number of rows: "
GetColumnString:.asciiz "\nEnter the number of columns: "
GetStepString:	.asciiz "\nEnter the number of step: "
GridInfoString: .asciiz "\nThe grid size is : "	
GridEnterString:.asciiz "\nEnter the value for grid x-y : "

.data 

# bomb: 	  	 s0          
# oldBombLocationsSize:  s1
# row:                   s2      
# column:                s3
# step:                	 s4
# grid:                	 s5
# oldBombLocations:      s6
	 
.text

Main:	
 		jal GetSizes		# getSizes() to get row, column and step numbers
		jal FillGrid		# fillGrid() to get bomb locations
		jal PrintGrid		# printGrid() printInitialStep
		
		
		li $t1, 1		# t1 = 1
		beq $s4, $t1, doNotBomb #(if step != 1) do below if not goTo doNotBomb because it does not need to plant or exculude bomb
		
		div $s4, $s4, 4		# s4 /= 4	and this operator loads remaining number to mfhi
		mfhi $s4		# step %= 4	get mfhi value to s4

            	
		li $t1, -1		# t1 = -1
		add $s4, $s4, $t1 	# step --
		
		div $s4, $s4, 4		# s4 /= 4	and this operator loads remaining number to mfhi
		mfhi $s4		# step %= 4	get mfhi value to s4
	
		li $t1, 4		# t1 = -1
		add $s4, $s4, $t1 	# step += 4
		
		loopForBomberMan:
			beqz $s4, endOfLoop	# if s4 == 0 finish Loop
		
			jal GetBombOlderLocation #getBombOlderLocation()
		
			# bomberman places bomb second second
			jal FillGridWithBombs	# fillGridWithBombs()
		
			li $t2, -1		# t2 = -1
			add $s4, $s4, $t2	# step -= 1
				
			beqz $s4, endOfLoop 	# if (step == 0) break;
		
			jal ExecuteOlderBombs 	#executeOlderBombs()
		
			li $t2, -1		# t2 = -1
			add $s4, $s4, $t2	# step -= 1
			beqz $s4, endOfLoop 	# if (step == 0) break;
			
			j loopForBomberMan	# goTo loopForBomberMan()
		
		endOfLoop: 
		doNotBomb: #(if step == 1)
			jal PrintGrid		# printGrid() which is the result
			
		j Exit
GetSizes: 
		la $a0, GetRowString		# load string to a0
		li $v0, 4			# printf("Enter the number of rows: ");
		syscall
		li $v0, 5			# scanf("%d", &row);
		syscall	
		move $s2, $v0   		# s2 = row saving row 
		

		la $a0, GetColumnString		# load string to a0
		li $v0, 4			# printf("Enter the number of columns: ");
		syscall
		li $v0, 5			# scanf("%d", &column);
		syscall
		move $s3, $v0   		# s3 = column saving column 
		
		
		la $a0, GetStepString	 	# load string to a0
		li $v0, 4			# printf("Enter the number of step: ");
		syscall
		li $v0, 5			# scanf("%d", &column);
		syscall
		move $s4, $v0   		# s4 = step saving step 
		
		jr $ra				# goTo who call this subroutines
	
FillGrid: 	
		# Allocate memory for the grid
		mul $t0, $s2, $s3
		 
		li $v0, 9 		# allocate heap memory
		move $a0, $t0 		# (char *)malloc(row * column * sizeof(char));
		syscall
		
		move $t2, $v0 		# t2 =  (char *)malloc(row * column * sizeof(char));
		move $s5, $v0 		# grid = (char *)malloc(row * column * sizeof(char));

		
    		add $t0, $zero, $zero 	# int i = 0
    		add $t1, $zero, $zero 	# int j = 0
            	
    		loop_for_fill_grid:
      			beq $s2, $t0, fill_grid_done  	# Check if row counter == 'row'
			add $t1, $zero, $zero
			
        		loopInner_for_fill_grid:
            			beq $s3, $t1, row_done  # Check if column counter == 'column'

           			la $a0, GridEnterString # load string to a0
            			li $v0, 4		# printf("Enter the value for grid x-y :");
            			syscall

            			move $a0, $t0   	# Print the row number
            			li $v0, 1		# printf(row);
            			syscall
				
				li $a0, '-'		
				li $v0, 11    		# print_character printf("-");
				syscall
				
            			move $a0, $t1   	# Print the column number
            			li $v0, 1		# printf(column);
            			syscall
				
				li $a0, ':'
				li $v0, 11    		# print_character printf(":");
				syscall			# it likes printf("Enter the value for grid %d-%d: ", i, j);
				
            			li $v0, 12       	# Get input
            			syscall			# scanf(" %c", &v0);
            			
            			sb $v0, 0($t2)  	# grid[i * column + j] = t2 = v0

            			addi $t2, $t2, 1  	# t2++ 			Move to the next grid element
            			addi $t1, $t1, 1  	# t1++ same as j++ 	Increment column counter   
            			j loopInner_for_fill_grid

        		row_done:
            			addi $t0, $t0, 1  	# t0++ same as i++ Increment row counter
            			j loop_for_fill_grid

    		fill_grid_done:
   			jr $ra


PrintGrid:	
		add $t0, $zero, $zero 			# int i = 0
    		add $t1, $zero, $zero 			# int j = 0
    		add $t2, $s5, $zero   			# t2 = grid
    		
    		li $v0, 11				
        	li $a0, '\n'				# printf("\n");
        	syscall
        	
    		loop_for_print_grid:
      			beq $s2, $t0, print_grid_done  	# Check if row counter == 'row'
			add $t1, $zero, $zero		# t1 = 0 same as j = 0
			
        		loopInner_for_print_grid:
            			beq $s3, $t1, print_row_done  # Check if column counter == 'column' go to printRowDone end of loop

				
				lb $a0, 0($t2)		# Print the grid element
				li $v0, 11    		# printf("%c", grid[i * column + j]);
				syscall

				
				li $a0, ' '
				li $v0, 11    		# printf(" ");
				syscall
				
            			addi $t1, $t1, 1  	# j++ same as t1++
            			addi $t2, $t2, 1  	# in here we do not use i*column+j i only do incerement grid value to next element : Move to the next grid element
            			j loopInner_for_print_grid

        		print_row_done:
        			li $v0, 11		
        			li $a0, '\n'		# printf("\n");
        			syscall
        			
            			addi $t0, $t0, 1  	# Increment row counter
            			j loop_for_print_grid

    		print_grid_done:
   			jr $ra

GetBombOlderLocation:
		add $t0, $zero, $zero 			# int i = 0
    		add $t1, $zero, $zero 			# int j = 0
    		add $t2, $s5, $zero  			# t2 point to grid
    		add $t3, $zero, $zero  			# oldBombLocationsSize = 0;
    		
    		loop_for_getBombCount:
      			beq $s2, $t0, getBombCount_done  	# Check if row counter == 'row'
			add $t1, $zero, $zero
			
        		loopInner_for_getBombCount_grid:
            			beq $s3, $t1, older_row_done  	# Check if column counter == 'column'

				
				li $t4, 'O'			# t4 = "O"
				lb $t8, 0($t2)			# t8 = grid[i * column + j]
				beq $t4, $t8, bomb_found	# if(t4 == t8) same as if(grid[i * column + j] == "O")
				
				
				
            			addi $t1, $t1, 1  		# j++ same as t1++
            			addi $t2, $t2, 1  		# Move to the next grid element
            			j loopInner_for_getBombCount_grid
            			
            			bomb_found:	
					addi, $t3, $t3, 1	# oldBombLocationsSize++ if bombFound we have to incerement t3 which holds oldBombLocationsSize
					addi $t1, $t1, 1  	# j++ same as t1++
            				addi $t2, $t2, 1  	# Move to the next grid element
            				
					j loopInner_for_getBombCount_grid

        		older_row_done:
            			addi $t0, $t0, 1  		# i++, t0++ Increment row counter
            			j loop_for_getBombCount

    		getBombCount_done:

    			add $s1, $t3, $zero
    			# oldBombLocations = (int *)malloc(oldBombLocationsSize * sizeof(int));
    			li $v0, 9				# allocate heap memory
    			li $t4, 4				# t4 = 4
    			mul $t3, $t3, $t4			# t3 = oldBombLocationsSize * t4 = oldBombLocationsSize * sizeof(int)
			move $a0, $t3 				# a0 = t3 = (int *)malloc(oldBombLocationsSize * sizeof(int))
			syscall
		
			move $t5, $v0 				# t5 = v0 contains address of allocated memory
			move $s6, $v0 				# s6 = v0 contains address of allocated memory
			
    			add $t0, $zero, $zero 			# int i = 0
    			add $t1, $zero, $zero 			# int j = 0
    			add $t2, $s5, $zero			# t2 = grid
				
 			add $t3, $zero, $zero  			# oldBombLocationsSize = 0;
			
			add $t7, $zero, $zero			# t7 = 0
   			loop_for_get_old_bomb_locations:
      				beq $s2, $t0, for_get_old_bomb_locations_done  # Check if row counter == 'row'
				add $t1, $zero, $zero		# t1 = j = 0 
				
        			loopInner_for_get_old_bomb_locations:
            				beq $s3, $t1, for_get_old_bomb_locations_done_col  # Check if column counter == 'column'
					
					
					# if(grid[i * column + j] == a)
					lb $t6, 0($t2)		# t6 = currentGridLocation = t2 = grid[i * column + j]
					li $t4, 'O' 		# t4 = O
					beq $t4, $t6, bomb_found_in_inner_loop		# if(t4 == t6) same as if(grid[i * column + j] == "O")
					
				
            				addi $t1, $t1, 1  	# j++ same as t1++
            				addi $t2, $t2, 1  	# Move to the next grid element
            				j loopInner_for_get_old_bomb_locations
            
            				bomb_found_in_inner_loop:
						
						mul $t8, $s3, $t0	# t8 = i * column 
						add $t8, $t8, $t1	# t8 += j 
						
				
						sb $t8, 0($t5)  	# oldBombLocations[oldBombLocationsSize] = i * column + j; Store the grid location in the oldBombLocations
						add $t7, $t7, 1		# t7 = 1
						add $t5, $t5, 4		# oldBombLocationsSize++ same as t5++
						
						addi $t1, $t1, 1  	# j++ same as t1++
            					addi $t2, $t2, 1  	# Move to the next grid element
            					j loopInner_for_get_old_bomb_locations

        			for_get_old_bomb_locations_done_col:
   
            				addi $t0, $t0, 1  		# Increment row counter
            				j loop_for_get_old_bomb_locations

    			for_get_old_bomb_locations_done:				
   				jr $ra
   				
FillGridWithBombs:
		add $t0, $zero, $zero 		# int i = 0
    		add $t1, $zero, $zero 		# int j = 0
            	add $t2, $s5, $zero 		# t2 point to grid
            	
    		loop_for_fill_grid_with_bombs:
      			beq $s2, $t0, fill_grid_done_with_bombs  	# Check if row counter == 'row'
			add $t1, $zero, $zero
			
        		loopInner_for_fill_grid_with_bombs:
            			beq $s3, $t1, row_done_fill_bombs  	# Check if column counter == 'column'

 
            			li $t3, 'O'
            			
            			sb $t3, 0($t2)  	# Store the 'O' (t3) in the grid

            			addi $t2, $t2, 1  	# Move to the next grid element
            			addi $t1, $t1, 1  	# t1++ same as j++
            			j loopInner_for_fill_grid_with_bombs

        		row_done_fill_bombs:
            			addi $t0, $t0, 1  	# Increment row counter
            			j loop_for_fill_grid_with_bombs

    		fill_grid_done_with_bombs:
   			jr $ra
   			
ExecuteOlderBombs:
		add $t0, $zero, $zero 			# int i = 0
            	add $t1, $s5, $zero  			# t1 point to grid
            	add $t2, $s1, $zero 			# t2 = oldBombLocationSize 
            	add $t3, $zero, $zero 			# int bombLocation = 0
            	add $t4, $zero, $zero 			# int bombRow = 0
            	add $t5, $zero, $zero 			# int bombCol = 0
		add $t6, $s6, $zero  			# save pointer at bombLocations 
            	add $s7, $ra, $zero			# save ra in s7 I USE S7 BECAUSE I DO NOT HAVE ANY REGISTER TO USE, and also i do not manipulate s7 so it obeys
            				
            	loop_for_execute:
      			beq $s1, $t0, execute_loop_done  # Check if row counter == 'row'
			            	
			
			add $t3, $t6, $zero 		# int bombLocation = oldBombLocations[i];load the current bomb location into t3
			
			lw $t3, 0($t6)			# int bombLocation = oldBombLocations[i]; bomb location assigment
			
			
  			div $t3, $s3  			# Divide t3 by 'column'
  			mflo $t4  			# int BombRow = bombLocation / column; Store the quotient in $t4 (bombRow)
  			mfhi $t5  			# int bombCol = bombLocation % column; Store the remainder in $t5 (bombCol)
  			
  			add $t7, $t4, $zero  		# $t7 = bombRow
  			add $t8, $t7, $zero 		# save Bomb Row to t8
  			
  			mul $t7, $t7, $s3  		# $t7 = bombRow * column
  			
  			add $t7, $t7, $t5  		# $t7 = bombRow * column + bombCol
  			add $t8, $t1, $t7  		# $t8 = &grid[bombLocation]
  			li $t9, '.'  			# Load '.' into $t8
  			sb $t9, 0($t8)  		# grid[bombLocation] = '.'
  
  	
  			# if statement begin for 0
			add $t8, $zero, $t8 		# t8 = newRow = bombRow + 0
			li $t9, -1
			add $t9, $t9 , $t5 		# t9 = newColumn = bombCol + -1

			add $t3, $t4, $zero  		# Calculate newRow * column
			mul $t3, $t3, $s3  		# $t3 = newRow * column
			add $t3, $t3, $t9  		# $t3 = newRow * column + newCol
			
			add $t3, $t1, $t3  		# $t3 = &grid[newRow * column + newCol]
			jal ExecuteLoop
			
			# if statement begin for 1
			add $t8, $zero, $t8 		# t8 = newRow = bombRow + 0
			li $t9, 1
			add $t9, $t9 , $t5 		# t9 = newColumn = bombCol + 1

			# t3=location to grid[location] = '.'
			add $t3, $t4, $zero 	 	# Calculate newRow * column
			mul $t3, $t3, $s3  		# $t3 = newRow * column
			add $t3, $t3, $t9  		# $t3 = newRow * column + newCol
			
			add $t3, $t1, $t3  		# $t3 = &grid[newRow * column + newCol]
			jal ExecuteLoop
			
			# if statement begin for 2
  			li $t9, -1
			add $t8, $t4, $t9 		# t8 = newRow = bombRow + -1
			li $t9, 0
			add $t9, $t9 , $t5 		# t9 = newColumn = bombCol + 0

			# t3=location to grid[location] = '.'
			add $t3, $t8, $zero  		# Calculate newRow * column
			mul $t3, $t3, $s3  		# $t3 = newRow * column
			add $t3, $t3, $t9  		# $t3 = newRow * column + newCol
			
			
			add $t3, $t1, $t3  		# $t3 = &grid[newRow * column + newCol]
			jal ExecuteLoop
			
			# if statement begin for 3
  			li $t9, 1
			add $t8, $t4, $t9 		# t8 = newRow = bombRow + 1
			li $t9, 0
			add $t9, $t9 , $t5 		# t9 = newColumn = bombCol + 0

			# t3=location to grid[location] = '.'
			add $t3, $t8, $zero  		# Calculate newRow * column
			mul $t3, $t3, $s3  		# $t3 = newRow * column
			add $t3, $t3, $t9  		# $t3 = newRow * column + newCol
			
			add $t3, $t1, $t3  		# $t3 = &grid[newRow * column + newCol]
			jal ExecuteLoop
			
			addi $t0, $t0, 1  		# t0++ same as  i++ Increment i counter
			addi $t6, $t6, 4  		# increment next bombLocation
			j loop_for_execute
			
    		execute_loop_done:
   			jr $s7




ExecuteLoop:
	# control statements
	# t3 location of placed . dot
	# newRow t4
	# newCol t9
	# row s2
	# column s3
	
	# this loop checks if(newRow >= 0 && newRow < row && newCol >= 0 && newCol < column)
	# and do grid[newRow * column + newCol] = '.';
	
	# first and
	beqz $t4, SecondAnd 			# if(newRow == 0)
	bgt $t4, $zero, SecondAnd		# if(newRow > 0)
	j exitFromExecutionLoop
	
	#secondAnd
	SecondAnd:
		blt $t4, $s2, ThirdAnd		# if(newRow < row)
		j exitFromExecutionLoop
	
	#thirdAnd
	ThirdAnd:
		beqz $t9, FourthAnd 		# if(newCol == 0)
		bgt $t9, $zero, FourthAnd 	# if(newCol > 0)
		j exitFromExecutionLoop
	
	#fourthAnd
	FourthAnd:
		blt $t9, $s3, PlaceBomb		# if(newCol < col)
		j exitFromExecutionLoop
	
	#placeBomb
	PlaceBomb:
		li $t9, '.'  			# Load '.' into $t8
  		sb $t9, 0($t3) 	 		# grid[bombLocation] = '.'
  		
	exitFromExecutionLoop:
		jr $ra


Exit:


		
		
		
