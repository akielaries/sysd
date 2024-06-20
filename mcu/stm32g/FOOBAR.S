	.cpu cortex-m4
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"foobar.c"
	.text
	.align	1
	.global	wait
	.arch armv7e-m
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	wait, %function
wait:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #12
	add	r7, sp, #0
	movs	r3, #0
	str	r3, [r7, #4]
	b	.L2
.L3:
	.syntax unified
@ 4 "foobar.c" 1
	nop
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	str	r3, [r7, #4]
.L2:
	ldr	r3, [r7, #4]
	ldr	r2, .L4
	cmp	r3, r2
	bls	.L3
	nop
	nop
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r7}
	bx	lr
.L5:
	.align	2
.L4:
	.word	1999999
	.size	wait, .-wait
	.ident	"GCC: (15:10.3-2021.07-4) 10.3.1 20210621 (release)"
