#############################################################
# NOTE: this is the provided TEMPLATE as your required 
#		starting point of HW2 MIPS programming part.
#		This is the only file you should change and submit.
#
# CS465-DL1 S2021
# HW2  
#############################################################
#############################################################
# PUT YOUR TEAM INFO HERE
# NAME Marisa Mills
# G# 01075052
# NAME 2
# G# 2
#############################################################

#############################################################
# Data segment
#############################################################

.data

	INPUTMSG: .asciiz "Enter a MIPS machine word in hex: 0x"
	OUTPUTMSG: .asciiz "Input: "
	INSTCODE: .asciiz "Instruction Code: "
	DESTREG: .asciiz "Destination Register: "
	SRCREGS: .asciiz "Source Register(s): "
	INVALID: .asciiz "Invalid"
	NOTUSED: .asciiz "N/A"
	LOGMSG: .asciiz "========================\nStage: "
	
	EQUALS: .asciiz " = "
	COMMA: .asciiz ", "
	SPACE: .asciiz " "
	NEWLINE: .asciiz "\n"
	
	

#############################################################
# Code segment
#############################################################

.text

#############################################################
# atoi
#############################################################
#############################################################
# DESCRIPTION  
#1. hexdecimal string to integer value
# we check each character and make sure it is in right range for ascii number and letter
# then we subtract either 48 (if num) or 55 if letter (65-55= 10 =a) to convert ascii to int
# then as long as we havent hit the 8th char we add our int to the sum and shift keft by 4 to fit next
# then we add 8th int and get our number and be print it as a hex and unsigned decimal 
# 
# PUT YOUR ALGORITHM DESCRIPTION HERE
#############################################################
		
.globl atoi
atoi:
	beq $t4,0, call_log
	# t4 = counter
check_num:
	add $t0, $a0, $t4
	lb $t1, ($t0)
	li $v0, 1
	#first byte is in t1 AND it with 1111
	blt $t1, 48, error
	bgt $t1,57, check_letter
	#this will change ascii to normal num 45ascii =0 
	addi $t2, $t1,-48
	j sum

	
check_letter:
	#this will change ascii to normal num 65ascii =A 70 = F
	bgt $t1,70, error
	addi $t2, $t1,-55
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
	jr $ra
report_value:
	move $v0,$t3

	jr $ra


#############################################################
# get_insn_code
#############################################################
#############################################################
# DESCRIPTION  
# we need to look at the opcode (the first six bits of the hex number instruction )to  determine 
#the opcode this is achomplished by shifting. The easiest way to  do this is to shift the number right 
#26 bits to leave the first 6 bits preceaded by zeros. 
# now that we have the opcode we can return the coresponding instruction code.
# the only other thing to check is if the opcode is 0 we have an r instruction and we have to determine the 
# fuction code to get the instruction code. Agsin we utilize shifting to get this information.
#first we shift left 26 then we shift right 26 bits to get the 6 digit funct code. 
# PUT YOUR ALGORITHM DESCRIPTION HERE
#############################################################

	
.globl get_insn_code
get_insn_code:

	#log status
	#save argument and return address
	#la $t0, INPUT
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	
	#print a new line then call log
	la $a0, NEWLINE
	li $v0, 4
	syscall	
	
	#log status
	#t6 is funct counter increment
	addi $t6, $t6,1
	add $t0,$zero,$t6
	move $a0, $t6
	jal log
	#restore variables
	lw $a0,0($sp)
	lw $ra,4($sp)
	addi $sp,$sp,8
	
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
	jr $ra
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
	jr $ra
	

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
	
.globl get_dest_reg
get_dest_reg:


	#log status
	#save argument and return address
	#la $t0, INPUT
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	
	#print a new line then call log
	la $a0, NEWLINE
	li $v0, 4
	syscall	
	
	#log status
	#t6 is funct counter increment
	addi $t6, $t6,1
	#add $t0,$zero,$t6
	move $a0, $t6
	jal log
	#restore variables
	lw $a0,0($sp)
	lw $ra,4($sp)
	addi $sp,$sp,8
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
	add $v0,$zero,33
	jr $ra
j_format:
	add $v0,$zero, 32
	jr $ra
r_format:
	sll $t1, $a0,16
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	jr $ra
i_format:
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	jr $ra
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

.globl get_src_regs
get_src_regs:

	#log status
	#save argument and return address
	#la $t0, INPUT
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	
	#print a new line then call log
	la $a0, NEWLINE
	li $v0, 4
	syscall	
	
	#log status
	#t6 is funct counter increment
	addi $t6, $t6,1
	#add $t0,$zero,$t6
	move $a0, $t6
	jal log
	#restore variables
	lw $a0,0($sp)
	lw $ra,4($sp)
	addi $sp,$sp,8
	
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
	jr $ra
j_format2:
	add $v0,$zero, 32
	add $v1,$zero, 32
	jr $ra
r_format2:
	sll $t1, $a0,6
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $v1,$t1
	jr $ra
sll_format2:
	sll $t1, $a0,11
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	add $v1,$zero,32
	jr $ra
i_format2:
	sll $t1, $a0,6
	srl $t1,$t1,27
	# the remaing five are the register
	move $v0,$t1
	add $v1,$zero, 32
	jr $ra
	
	
#############################################################
# optional: other helper functions
#############################################################
				
