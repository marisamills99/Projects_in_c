#############################################################
# NOTE: this is the provided TEMPLATE as your required 
#		starting point of HW3 MIPS programming part.
#		This is the only file you should change and submit.
#
# CS465-DL1 S2021
# HW3 
#############################################################

#############################################################
# PUT YOUR TEAM INFO HERE
# NAME Marisa Mills
# G# 01075052
# NAME 2
# G# 2
#############################################################

#############################################################
# DESCRIPTION  
# for stall cycles keep a count of what registers are being updated (only keep track of last two)
# save last two registers in a1 and a2 (init to 0)
# every instruct move a1 to a2 and new register to a1 (if nothing move 0 to a1)  
#if its is a dependancy between a1 then stall 2 cycles if it is at a2 only stall once
#create two arrays one to store all the inputed instructions and one to store [int dest register, int instruct_that_uses__registrer]
# now we can pull src and dest information from our registers and store the dest information and instruction information 
# To get the dependancies just check if the current dest or src registers are being used by anything in our list
# if they are in the list print the (register number, instruct number, current instruction)
# dont forget to do , next dependancy after the first one prints
# PUT YOUR ALGORITHM DESCRIPTION HERE
#############################################################

#############################################################
# Data segment
#############################################################

.data # Start of Data Items
	array: .space 40 
	dependarray: .space 80
	registers: .space 31
	INIT_INPUT: .asciiz "How many instructions to process? "
	INSTR_SEQUENCE: .asciiz "Please input instruction sequence (one per line):"
	NEWLINE: .asciiz "\nNext instruction:"
	INVALID: .asciiz "Invalid"
	NOTHING: .asciiz " None"
	DEPEND: .asciiz "\nDependences:"
	STALL: .asciiz "\nStall cycles:"
	OPEN: .asciiz "("
	COMMA: .asciiz ", "
	CLOSE: .asciiz ")"
	FINISH: .asciiz "\n-------------------------------------------"
	COLON: .asciiz ":"
	SPACE: .asciiz " "
	INSTRUCT: .asciiz "\nI"
	INSTRUCTION: .asciiz " I"
	NEWLINES: .asciiz "\n"

	.align 4
	INPUT: .space 9
	
	

.text
main:
	la $a0, INIT_INPUT
	li $v0, 4
	syscall # Print out message asking for N (number of instructions to process)
	
	li $v0, 5
	syscall # read in Int 
	addi $t1, $v0, 0 
	
	
	la $a0, INSTR_SEQUENCE
	li $v0, 4
	syscall 
	
	li $t0, 0 # loop counter	
	Loop: # Read in N strings
		li $t4,0 #counter for bit loop
		li $t3,0
		
		la $a0, NEWLINE # Print out prompt for next instruction
		li $v0, 4
		syscall 												

		la $a0, INPUT
		li $a1, 9
		li $v0, 8
		syscall # read in one string and store in INPUT
		
		li $a1,0
	check_num:
		add $t7, $a0, $t4
		lb $t6, ($t7)
		li $v0, 1
		#first byte is in t1 AND it with 1111
		blt $t6, 48, error
		bgt $t6,57, check_letter
		#this will change ascii to normal num 45ascii =0 
		addi $t2, $t6,-48
		j sum
	
	check_letter:
		#this will change ascii to normal num 65ascii =A 70 = F
		bgt $t6,70, error
		addi $t2, $t6,-55
		j sum
	sum:
		#sum is in t3 char is in t2
		add $t3,$t3,$t2
		# increment counter
		addi $t4, $t4,1
		beq $t4, 8, report_value
		sll $t3 $t3 4
		j check_num
	error:
		li $v0, 4
		la $a0, INVALID
		syscall	# print out MSG with error
		#jr $ra
	report_value:
		#T3 HOLDS THE VALUE add to the array  
		# t7 is offset (i*4)
		li $t5, 4
		mult $t5,$t0
		mflo $t5
		sw $t3, array($t5)																												
		addi $t0, $t0, 1
		blt $t0, $t1, Loop
		#initilize array and counter 
		la $s1, array 
		li $t5, 0
		move $s0,$t1
	restore:
		#get value from the array
		lw $a0, 0($s1)
    		addi $s1, $s1, 4
    		
    		
    	get_insn_code:
	
		#lw   $t0, ($a0)  # load the value
		srl $t7, $a0,26  # shift right 26 bits
		#check opcode
		#if 0 check func
		beq $t7,0,check_funct
		beq $t7, 8, addi_branch
		beq $t7, 35, lw_branch
		beq $t7, 5, bne_branch
		beq $t7, 3, jal_branch
		#check 2b
		beq $t7, 43, sw_branch
		beq $t7, 2, j_branch
		#otherwise invalid
		add $v0,$zero,33
		add $t7,$zero,33
		j finish
#set corresponding return vals 
check_funct:
	sll $t0, $a0,26
	srl $t0,$t0,26
	beq $t0, 32, add_branch
	beq $t0, 0,sll_branch
	beq $t0, 42, slt_branch
	beq $t0, 8, jr_branch
add_branch:
	addi $t7, $zero, 0
	j finish
sll_branch:
	addi $t7, $zero, 1
	j finish
slt_branch:
	addi $t7, $zero, 6
	j finish
jr_branch:
	addi $t7, $zero, 9
	j finish
addi_branch:
	addi $t7, $zero, 5
	j finish
lw_branch:
	addi $t7, $zero, 2
	j finish
bne_branch:
	addi $t7, $zero, 3
	j finish
jal_branch:
	addi $t7, $zero, 4
	j finish
sw_branch:
	addi $t7, $zero, 7
	j finish
j_branch:
	addi $t7, $zero, 8
	j finish
finish:
	move $v0,$t7
	

#############################################################
# get_dest_reg
#############################################################
#############################################################
# DESCRIPTION  
#we use the given opcode to determine what type of instruction we have and where to look for the destination register
#j type has no destination register. r type has the 5 bit destination register preceded by  6 bit opcode 5 bit rs 5 bit rt
# and followed by 5 bit shamt and 6 bit funct. I type has it in the rs feild if there is a register updated.
# It is preceaded by 6 bit opcode and followed by 5 bit rt field and the 16 bit constant or address. 
# we again can shift left and right the appropriate number of bits (based on instruction) to reveal the destination
#registers that are updated. If nothing is updated we can return 32 and return 33 if there was and invalid input. 
# PUT YOUR ALGORITHM DESCRIPTION HERE
#############################################################
	

get_dest_reg:


	
	# shift left to remove opcode
	#shift right to get remaining 5
	#if its a jump  automatically print n/a
	
	#branch based on what the instruct code is 
	
	beq $t7,0,r_format
	beq $t7, 1, r_format
	beq $t7, 2, i_format
	beq $t7, 3, j_format
	beq $t7, 4, j_format
	beq $t7, 5, i_format
	beq $t7, 6, r_format
	beq $t7, 7, j_format
	beq $t7, 8, j_format
	beq $t7, 9, j_format
	#otherwise invalid
	add $t6,$zero,33
	j get_src_regs
j_format:
	add $t6,$zero, 32
	j get_src_regs
r_format:
	sll $t1, $a0,16
	srl $t1,$t1,27
	# the remaing five are the register
	move $t6,$t1
	j get_src_regs
i_format:
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $t6,$t1
	j get_src_regs
#############################################################
# get_src_regs
#############################################################
#############################################################
# DESCRIPTION 
#we use the given opcode to determine what type of instruction we have and where to look for the source register
#j type has no source register. r type has 2 registers to look at, the 5 bit rs and 5 bit rt preceded by 6 bit opcode 
# and followed by 5 bit rd  5 bit shamt and 6 bit funct. I type has 5 bit rt preceaded by the rs feild 
#and 6 bit opcode and it is followed by the 16 bit constant or address. Occasionally there is no destinition register, but 
# there is a source register we again can shift left and right the appropriate number of bits to reveal the source
#register(s) that are updated. If nothing is updated we can return 32 and return 33 if there was and invalid input. 
# PUT YOUR ALGORITHM DESCRIPTION HERE
#############################################################


get_src_regs:
	
	#branch based on format type 
	
	beq $t7,0,r_format2
	beq $t7, 1, sll_format2
	beq $t7, 2, i_format2
	beq $t7, 3, r_format2
	beq $t7, 4, j_format2
	beq $t7, 5, i_format2
	beq $t7, 6, r_format2
	beq $t7, 7, r_format2
	beq $t7, 8, j_format2
	beq $t7, 9, i_format2
	#otherwise invalid
	add $v0,$zero,33
	#jr $ra
	j end_here
j_format2:
	add $v0,$zero, 32
	add $v1,$zero, 32
	#jr $ra
	j end_here 
r_format2:
	sll $t1, $a0,6
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $v1,$t1
	#jr $ra
	j end_here
sll_format2:
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	add $v1,$zero,32
	#jr $ra
	j end_here
i_format2:
	sll $t1, $a0,6
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	add $v1,$zero, 32
	#jr $ra
	j end_here
end_here:
	li $t1,0
	move $s2,$v0
	move $s3, $v1
	bne $t6,$a1, add_to_stall 
	addi $t1, $t1,2
add_to_stall:
	bne $t6,$a2, add_to_stall2
	addi $t1, $t1,1
	j stall
add_to_stall2:
	bne $v0,$a1, add_to_stall3
	addi $t1, $t1,2
	j stall 
add_to_stall3:
	bne $v0,$a2, add_to_stall4
	addi $t1, $t1,1
	j stall 
add_to_stall4:
	bne $v1,$a1, add_to_stall5
	addi $t1, $t1,2
	j stall
add_to_stall5:
	bne $v1,$a2, stall
	addi $t1, $t1,1
	j stall
stall:
	 # print the report with instruct dependancies and stalls
	
	la $a0, INSTRUCT
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t5
    	syscall
    	
    	la $a0, COLON
	li $v0, 4
	syscall
	
	
	la $a0, DEPEND
	li $v0, 4
	syscall
	# initilize counters and load the dependancy array 
	li $t2,0
	li $t8, 0
	la $s4, dependarray 
	# jump to print dependancies
	jal dependancies
	
	#t6 has dest v0 has src 1 v1 has src2
	#la $t1, dependarray
	#store dest register
	sw $t6,dependarray($t9)
	addi $t9, $t9, 4
	#store instruct number next to it 
	sw $t5,dependarray($t9)
	addi $t9, $t9, 4
	
	# print the number stalls
	la $a0, STALL
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t1
    	syscall	 
	# print a --------------- to seperate 
	la $a0, FINISH
	li $v0, 4
	syscall
	
	
	# move my dest register into a1 for comparison
	move $a2, $a1
	move $a1,$t6
	beq $a1, 32, changereg
	j continue_here
changereg:
	# if it was reg 32 change to -1 so 2 things don't match
	li $a1, -1
continue_here:
	# loop through and restore next instruct and check						
    	addi $t5, $t5, 1
    	blt $t5, $s0, restore
    	# finish 
	li $v0, 10
	syscall
dependancies:
	#t6 has dest s2 has src 1 s3 has src2
	# create a list int register int instruction
	lw $t4, 0($s4)
    	addi $s4, $s4, 8
    	#matches nothing ignore 
    	beq $t4,32, dependancies
    	beq $t4, 33, dependancies
    	
    	beq $t6,$t4, printdepend
    	beq $s2,$t4, printdepend
    	beq $s3,$t4, printdepend
    	
    	addi $t8,$t8,1
    	# if something has been printed go to depend2
    	bne $t2, $zero, dependancies2
    	bne $t8, $s0, dependancies
    	beq $t2, $zero, printnone
    	jr $ra
dependancies2:
	# this the same just calls a funct to add a comma between 
	lw $t4, 0($s4)
    	addi $s4, $s4, 8
    	
    	#matches nothing ignore 
    	beq $t4,32, dependancies2
    	beq $t4, 33, dependancies2
    	
    	beq $t6,$t4, printdepend2
    	beq $s2,$t4, printdepend2
    	beq $s3,$t4, printdepend2
    	
    	addi $t8,$t8,1
    	bne $t8, $s0, dependancies2
    	beq $t2, $zero, printnone
    	jr $ra
printnone:  
	# print nonthing  	
    	la $a0, NOTHING
	li $v0, 4
	syscall
	
	jr $ra
printdepend:
	# flag to check if it has been printed
	li $t2,1  
	# print depend in format (Register number,corresponding instruction,current instruction)	
    	la $a0, OPEN
	li $v0, 4
	syscall
    	
    	li $v0, 1
    	move $a0, $t4
    	syscall
    	
    	la $a0, COMMA
	li $v0, 4
	syscall
    	
    	la $a0, INSTRUCTION
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t8
    	syscall
    	
    	la $a0, COMMA
	li $v0, 4
	syscall
    	
    	la $a0, INSTRUCTION
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t5
    	syscall
    	
    	la $a0, CLOSE
	li $v0, 4
	syscall
    	
    	# loop and check next one
    	addi $t8,$t8,1
    	bne $t8, $s0, dependancies2
    	# hit the end so return 
	jr $ra

printdepend2:
	# flag to check if it has been printed
	li $t2,1 
	# same as above just adds a comma
	la $a0, COMMA
	li $v0, 4
	syscall
	
	 	
    	la $a0, OPEN
	li $v0, 4
	syscall
    	
    	li $v0, 1
    	move $a0, $t4
    	syscall
    	
    	la $a0, COMMA
	li $v0, 4
	syscall
    	
    	la $a0, INSTRUCTION
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t8
    	syscall
    	
    	la $a0, COMMA
	li $v0, 4
	syscall
    	
    	la $a0, INSTRUCTION
	li $v0, 4
	syscall
	
	li $v0, 1
    	move $a0, $t5
    	syscall
    	
    	la $a0, CLOSE
	li $v0, 4
	syscall
    	
    	addi $t8,$t8,1
    	bne $t8, $s0, dependancies2
	jr $ra
