	.text
	addu $s1, $v0, $0	# $s1 = $v0, load int value into register
	addu $t0, $s0, $0	# $t0 = $s0
	addu $t1, $s1, $0	# $t1 = $s1
	
	addi $s1, $v0, 5	
	addu $t0, $s0, $0	
	addi $t1, $s1, 7	
	
	addu $t2, $t0, $t1	# $t2 = $t1 + $t0
	
	addu $t3, $t1, $t2
	
	addu $t4, $t2, $t3
	
	addu $t5, $t3, $t4
	
	addu $t6, $t4, $t5
	
	addu $t7, $t5, $t6
	li $v0, 1		# load print int service
	addu $a0, $t7, $0	# $a0 = $t0, $a0 is the register from which print service will get the int to print
	syscall			# printf( "%d", $a0 )
	
	li $v0, 10
	syscall
	
