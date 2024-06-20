		addi $t0, $0, 5			# i = 5 (saveToMem loop runs while i > 0)		
		addi $s0, $0, 0X10010008	# memory address 8. which means 2nd word (0 indexed)
saveToMem:		beq $t0, $0, saveToMemExit		# if i == 0, branch to loopExit
		move $a0, $t0			# $a0 = i (it will be sent to subroutine)
		jal saveCalculation 		# recieving the calculated value (it will be saved in $v0 by the subroutine and then it will be saved in memory)
		sw $v0, 0($s0)			# save the value in memory
		addi $s0, $s0, 4		# incrementing the memory by 4 to reach the next word
		subi $t0, $t0, 1		# i--
		j saveToMem			# jump to loop
saveToMemExit:		bne $s0, $0, exit		# if $s0 is not 0, branch to exit (since s0 keeps the memory address and it is a possitive number -at this point-, this branch should be taken)
saveCalculation:	ori $v0, $a0, 2049		# calculating the value and saving it in $v0. this is basically bitwise or operation with 2049.
		jr $ra			# return back
exit:		addi $s0, $0, 0X1001000c		# memory address is set to 12. which means 3rd word (0 indexed)
		lb $s1, 0($s0)			# reading the first byte of 3rd word and saving it in $s1 (first byte means the rightmost byte)
		slt $t1, $s1, $s0		# if the value we just read is smaller than its address, set $t1 to 1. Otherwise, set $t1 to 0.
