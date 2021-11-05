#############################################################
# NOTE: this is the provided TEMPLATE as your required 
#	starting point of HW1 MIPS programming part.
#
# CS465 S2021 HW1  
#############################################################
#############################################################
# PUT YOUR TEAM INFO HERE
# NAME Marisa Mills
# G#01075052
# NAME 2
# G# 2
#############################################################

#############################################################
# DESCRIPTION OF ALGORITHMS 
#
# PUT A BRIEF ALGORITHM DESCRIPTION HERE
# 1. hexdecimal string to integer value
# we check each character and make sure it is in right range for ascii number and letter
# then we subtract either 48 (if num) or 55 if letter (65-55= 10 =a) to convert ascii to int
# then as long as we havent hit the 8th char we add our int to the sum and shift keft by 4 to fit next
# then we add 8th int and get our number and be print it as a hex and unsigned decimal 
# 2. extract n bits between high and low indexes (inclusively)
# The easiest way to do this is by shifting. first we take our int and left shift it by (31- high order )
# next we right shift it back in place by doing (# of left shifts + low order)
# given 23 = high 15 =low
#00000001001000110100010101101011
#00100011010001010110101100000000
#00000000000000000000000001000110    = 0x46 or 70 

#############################################################

#############################################################
# Data segment
# 
# Feel free to add more data items
#############################################################
.data
	INPUTMSG: .asciiz "Enter a hexadecimal number: "
	INPUTHIGHMSG: .asciiz "Specify the high bit index to extract (0-LSB, 31-MSB): "
	INPUTLOWMSG: .asciiz "Specify the low bit index to extract (0-LSB, 31-MSB, low<=high): "
	OUTPUTMSG: .asciiz "Input: "
	BITSMSG: .asciiz "Extracted bits: "
	ERROR: .asciiz "Error: Input has invalid digits!"
	INDEXERROR: .asciiz "Error: Input has incorrect index(es)!"
	EQUALS: .asciiz " = "
	NEWLINE: .asciiz "\n"
	ZERO: .asciiz "0"
	TEN: .asciiz "A"
	
	.align 4
	INPUT: .space 8

#############################################################
# Code segment
#############################################################
.text

#############################################################
# Provided entry of program execution
# DO NOT MODIFY this part
#############################################################
		
main:
	li $v0, 4
	la $a0, INPUTMSG
	syscall	# print out MSG asking for a hexadecimal
	
	li $v0, 8
	la $a0, INPUT
	li $a1, 9 # one more than the number of allowed characters
	syscall # read in one string of 8 chars and store in INPUT

#############################################################
# END of provided code that you CANNOT modify 
#############################################################
				
	#li $v0, 4
	#la $a0, INPUT
	#syscall # print out string that read in 
	#li $v0, 4
	#la $a0, NEWLINE
	#syscall	
	# set counter =0 
	move $t4, $zero
	#set sum = 0
	move $t3, $zero

##############################################################
# Add your code here to calculate the numeric value from INPUT 
##############################################################
convert_to_decimal:
	la $t0, INPUT
	# t4 = counter
	add $t0, $t0, $t4
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
	j convert_to_decimal
error:
	li $v0, 4
	la $a0, ERROR
	syscall	# print out MSG with error
	j exit
report_value:
#############################################################
# Add your code here to print the numeric value
# Hint: syscall 34: print integer as hexadecimal
#	syscall 36: print integer as unsigned
#############################################################
	li $v0, 4
	la $a0, OUTPUTMSG
	syscall	
	li $v0, 34
	move $a0, $t3
	syscall	
	li $v0, 4
	la $a0, EQUALS
	syscall	
	li $v0, 36
	move $a0, $t3
	syscall	


#############################################################
# Add your code here to get two integers: high and low
#############################################################
high_low:
	li $v0, 4
	la $a0, NEWLINE
	syscall
	li $v0, 4
	la $a0, INPUTHIGHMSG
	syscall	# print out MSG asking for high index
	
	li $v0, 5
	syscall 
	#the high bit is in t0
	move $t0, $v0
	bgt $t0, 31, error_index
	blt $t0, 0, error_index
	#check for low
	li $v0, 4
	la $a0, NEWLINE
	syscall
	li $v0, 4
	la $a0, INPUTLOWMSG
	syscall	# print out MSG asking for high index
	
	li $v0, 5
	syscall 
	#the low bit is in t1
	move $t1, $v0
	bgt $t1, 31, error_index
	blt $t1, 0, error_index
	
	# check that low t1 is less than high t0
	bgt $t1, $t0 error_index
	j extract
error_index:
	li $v0, 4
	la $a0, INDEXERROR
	syscall	# print out MSG with error
	j exit
#############################################################
# Add your code here to extract bits and print extracted value
#############################################################
extract:
	# t3 still has the num
	# shift left by 31- high order bit (t0)
	li $t5,31
	subu $t0, $t5, $t0
	sllv $t4, $t3, $t0
	
	# shift right by low orderbit(t1) plus left shifts and store in t4
	add $t1,$t1,$t0
	srlv $t4, $t4, $t1
	
	#report bit value
	li $v0, 4
	la $a0, NEWLINE
	syscall
	li $v0, 4
	la $a0, BITSMSG
	syscall	# print out MSG for bits
	#print hex
	li $v0, 34
	move $a0, $t4
	syscall	
	#print " = "
	li $v0, 4
	la $a0, EQUALS
	syscall	
	#print unsigned
	li $v0, 36
	move $a0, $t4
	syscall	
		


#############################################################
# Optional exit 
#############################################################
exit:
	li $v0, 10
	syscall

# Example input	
# H: 0x 0   1    2    3    4    5    6    A
# B: 0000 0001 0010 0011 0100 0101 0110 1010
#    31   27   23   19   15   11   7    3  0 (index)
		

