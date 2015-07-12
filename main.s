	.file	"main.c"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,8388608
	add	sp,sp,-64
	add	a0,a0,64
	sd	ra,56(sp)
	sd	s0,48(sp)
	sd	s1,40(sp)
	sd	s2,32(sp)
	sd	s3,24(sp)
	sd	s4,16(sp)
	call	malloc
	beqz	a0,.L12
	add	a1,a0,64
	and	a1,a1,-64
	sd	a0,-8(a1)
	mv	s3,a1
.L2:
	li	a0,8388608
	add	a0,a0,64
	call	malloc
	beqz	a0,.L13
	add	a2,a0,64
	and	a2,a2,-64
	sd	a0,-8(a2)
	mv	s2,a2
.L3:
	li	a0,8388608
	add	a0,a0,64
	call	malloc
	beqz	a0,.L14
	add	a5,a0,64
	and	a5,a5,-64
	sd	a0,-8(a5)
	mv	s1,a5
.L4:
	li	a0,8388608
	add	a0,a0,64
	call	malloc
	beqz	a0,.L15
	add	a3,a0,64
	and	a3,a3,-64
	sd	a0,-8(a3)
	mv	s0,a3
.L5:
	lui	s4,%hi(.LC0)
	lui	a3,%hi(h2)
	lui	a2,%hi(w2h1)
	lui	a1,%hi(w1)
	add	a3,a3,%lo(h2)
	add	a2,a2,%lo(w2h1)
	add	a1,a1,%lo(w1)
	add	a0,s4,%lo(.LC0)
	call	read_matrix_dimension
	mv	a3,s0
	mv	a2,s2
	mv	a1,s3
	add	a0,s4,%lo(.LC0)
	call	read_matrix
	li	a5,1
	beq	a0,a5,.L18
	li	a5,2
	beq	a0,a5,.L19
	li	a5,3
	beq	a0,a5,.L20
	bnez	a0,.L7
	mv	a1,zero
	mv	a0,sp
	call	gettimeofday
	mv	a2,s2
	mv	a1,s3
	mv	a0,s1
	ld	s2,0(sp)
	ld	s3,8(sp)
	call	optimize
	mv	a1,zero
	mv	a0,sp
	call	gettimeofday
	mv	a1,s0
	mv	a0,s1
	ld	s0,0(sp)
	ld	s1,8(sp)
	call	compare_matrix
	bnez	a0,.L21
	li	a5,999424
	add	a5,a5,576
	mul	a1,s0,a5
	lui	a0,%hi(.LC5)
	add	a0,a0,%lo(.LC5)
	mul	s2,s2,a5
	add	a1,a1,s1
	add	s2,s2,s3
	sub	a1,a1,s2
	call	printf
.L7:
	ld	ra,56(sp)
	mv	a0,zero
	ld	s0,48(sp)
	ld	s1,40(sp)
	ld	s2,32(sp)
	ld	s3,24(sp)
	ld	s4,16(sp)
	add	sp,sp,64
	jr	ra
.L21:
	lui	a0,%hi(.LC4)
	add	a0,a0,%lo(.LC4)
	call	printf
	j	.L7
.L12:
	mv	s3,zero
	j	.L2
.L13:
	mv	s2,zero
	j	.L3
.L14:
	mv	s1,zero
	j	.L4
.L15:
	mv	s0,zero
	j	.L5
.L18:
	lui	a0,%hi(.LC1)
	add	a0,a0,%lo(.LC1)
	call	puts
	j	.L7
.L19:
	lui	a0,%hi(.LC2)
	add	a0,a0,%lo(.LC2)
	call	printf
	j	.L7
.L20:
	lui	a0,%hi(.LC3)
	add	a0,a0,%lo(.LC3)
	call	printf
	j	.L7
	.size	main, .-main
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"test/test.out"
	.zero	2
.LC1:
	.string	"Cannot open test file"
	.zero	2
.LC2:
	.string	"Error while reading data from test file"
.LC3:
	.string	"Error while closing the test file"
	.zero	6
.LC4:
	.string	"incorrect"
	.zero	6
.LC5:
	.string	"%lld"
	.ident	"GCC: (GNU) 5.1.0"
