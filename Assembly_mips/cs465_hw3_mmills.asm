
.text
.globl main
.data
.align 2
main_registers: .space 48
.text
main:

li $t0, 3

li $t1, 2
mul $t0,$t0,$t1

li $t1, 3
add $t0,$t0,$t1

li $t1, 4

li $t2, 2
div $t1,$t1,$t2
sub $t0,$t0,$t1

li $t1, 2
sub $t0,$t0,$t1

li $t1, 1
sub $t0,$t0,$t1

li $t1, 1

li $t2, 3
mul $t1,$t1,$t2
add $t0,$t0,$t1

li $v0, 1 
move $a0, $t0 
syscall 

	li $v0, 10
	syscall
