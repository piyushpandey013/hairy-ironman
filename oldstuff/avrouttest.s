	.file	"avrouttest.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.text
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-1)
	out 49-32,r24
	out 50-32,__zero_reg__
	ldi r25,lo8(0)
.L2:
	mov r30,r25
	ldi r31,lo8(0)
	subi r30,lo8(-(array.1250))
	sbci r31,hi8(-(array.1250))
	ld r24,Z
	out 50-32,r24
	subi r25,lo8(-(1))
	rjmp .L2
	.size	main, .-main
	.data
	.type	array.1250, @object
	.size	array.1250, 16
array.1250:
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
