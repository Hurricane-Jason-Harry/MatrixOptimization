	.file	"utils.c"
	.text
	.align	2
	.globl	compare_matrix
	.type	compare_matrix, @function
compare_matrix:
	lui	a5,%hi(.LC0)
	li	a3,8388608
	fld	ft2,%lo(.LC0)(a5)
	add	a3,a0,a3
	j	.L3
.L8:
	beq	a3,a0,.L7
.L3:
	fld	ft0,0(a0)
	fld	ft1,0(a1)
	add	a0,a0,8
	add	a1,a1,8
	fsub.d	ft0,ft0,ft1
	fcvt.w.d a5,ft0,rtz
	sraw	a4,a5,31
	xor	a5,a4,a5
	subw	a5,a5,a4
	fcvt.d.w	ft0,a5
	fgt.d	a5,ft0,ft2
	beqz	a5,.L8
	li	a0,1
	ret
.L7:
	mv	a0,zero
	ret
	.size	compare_matrix, .-compare_matrix
	.align	2
	.globl	read_matrix_dimension
	.type	read_matrix_dimension, @function
read_matrix_dimension:
	add	sp,sp,-48
	sd	s1,24(sp)
	mv	s1,a1
	lui	a1,%hi(.LC1)
	add	a1,a1,%lo(.LC1)
	sd	s2,16(sp)
	sd	s3,8(sp)
	sd	ra,40(sp)
	sd	s0,32(sp)
	mv	s2,a2
	mv	s3,a3
	call	fopen
	beqz	a0,.L13
	mv	s0,a0
	mv	a3,a0
	li	a2,1
	li	a1,4
	mv	a0,s1
	call	fread
	bnez	a0,.L11
.L12:
	li	a0,2
.L10:
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
.L13:
	ld	ra,40(sp)
	li	a0,1
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
.L11:
	mv	a3,s0
	li	a2,1
	li	a1,4
	mv	a0,s2
	call	fread
	beqz	a0,.L12
	mv	a3,s0
	li	a2,1
	li	a1,4
	mv	a0,s3
	call	fread
	beqz	a0,.L12
	mv	a0,s0
	call	fclose
	beqz	a0,.L10
	li	a0,3
	j	.L10
	.size	read_matrix_dimension, .-read_matrix_dimension
	.align	2
	.globl	read_matrix
	.type	read_matrix, @function
read_matrix:
	add	sp,sp,-48
	sd	s3,8(sp)
	mv	s3,a1
	lui	a1,%hi(.LC1)
	add	a1,a1,%lo(.LC1)
	sd	s1,24(sp)
	sd	s2,16(sp)
	sd	ra,40(sp)
	sd	s0,32(sp)
	mv	s1,a2
	mv	s2,a3
	call	fopen
	beqz	a0,.L26
	mv	a3,a0
	mv	s0,a0
	li	a2,1048576
	mv	a0,s1
	li	a1,8
	call	fread
	li	s1,1048576
	bgeu	a0,s1,.L24
.L25:
	li	a0,2
.L23:
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
.L26:
	ld	ra,40(sp)
	li	a0,1
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
.L24:
	mv	a3,s0
	mv	a2,s1
	li	a1,8
	mv	a0,s2
	call	fread
	bltu	a0,s1,.L25
	mv	a3,s0
	mv	a2,s1
	li	a1,8
	mv	a0,s3
	call	fread
	bltu	a0,s1,.L25
	mv	a0,s0
	call	fclose
	beqz	a0,.L23
	li	a0,3
	j	.L23
	.size	read_matrix, .-read_matrix
	.section	.srodata.cst8,"aM",@progbits,8
	.align	3
.LC0:
	.4byte	2665960982
	.4byte	1020396463
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC1:
	.string	"rb"
	.ident	"GCC: (GNU) 5.1.0"
