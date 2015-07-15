	.file	"optimizations.c"
 #APP
	.align 3
__riscv_simd:
  vld vv0, va0
  vld vv1, va1
  vfmadd.d vv1, vs1, vv0, vv1
  vsd vv1, va1
  vstop

 #NO_APP
	.text
	.align	2
	.globl	naive
	.type	naive, @function
naive:
	add	sp,sp,-48
	sd	s1,24(sp)
	li	s1,8388608
	sd	s0,32(sp)
	sd	s2,16(sp)
	mv	s0,a1
	mv	s2,a2
	mv	a1,zero
	mv	a2,s1
	sd	s3,8(sp)
	sd	ra,40(sp)
	mv	s3,a0
	call	memset
	li	a7,8192
	mv	a6,s3
	add	a1,s0,a7
	add	s1,s3,s1
	li	t0,-8192
.L2:
	add	a2,a1,t0
	mv	a0,s2
	add	a3,a6,a7
.L4:
	fld	ft2,0(a2)
	mv	a4,a0
	mv	a5,a6
.L3:
	fld	ft0,0(a4)
	fld	ft1,0(a5)
	add	a5,a5,8
	fmul.d	ft0,ft2,ft0
	add	a4,a4,8
	fadd.d	ft0,ft1,ft0
	fsd	ft0,-8(a5)
	bne	a3,a5,.L3
	add	a2,a2,8
	add	a0,a0,a7
	bne	a2,a1,.L4
	li	a1,8192
	add	a1,a2,a1
	mv	a6,a3
	bne	a3,s1,.L2
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
	.size	naive, .-naive
	.align	2
	.globl	openmp
	.type	openmp, @function
openmp:
	ret
	.size	openmp, .-openmp
	.align	2
	.globl	simd
	.type	simd, @function
simd:
	add	sp,sp,-48
	sd	s0,32(sp)
	sd	s1,24(sp)
	sd	s2,16(sp)
	sd	ra,40(sp)
	mv	s1,a0
	mv	s2,a1
	mv	s0,a2
 #APP
# 105 "src/optimizations.c" 1
	vsetcfg 16, 1

# 0 "" 2
 #NO_APP
	mv	a1,zero
	li	a2,8388608
	call	memset
	lui	t0,%hi(__riscv_simd)
	mv	a1,s2
	mv	a7,zero
	add	t0,t0,%lo(__riscv_simd)
	li	a0,8192
	li	t2,1048576
	li	t3,8192
.L12:
	mv	t1,a1
	mv	a6,zero
.L14:
	ld	a5,0(t1)
 #APP
# 113 "src/optimizations.c" 1
	  vmss vs1, a5
# 0 "" 2
 #NO_APP
	mv	a4,zero
.L13:
	sraw	a5,a4,31
	srlw	a5,a5,29
	addw	a5,a5,a4
	sraw	a5,a5,3
	subw	a2,a0,a4
 #APP
# 118 "src/optimizations.c" 1
	  vsetvl a2, a2

# 0 "" 2
 #NO_APP
	add	a3,a5,a6
	sll	a3,a3,3
	sd	a2,8(sp)
	add	a3,s0,a3
 #APP
# 119 "src/optimizations.c" 1
	  vmsa va0, a3
# 0 "" 2
 #NO_APP
	add	a5,a5,a7
	sll	a5,a5,3
	add	a5,s1,a5
 #APP
# 120 "src/optimizations.c" 1
	  vmsa va1, a5
# 0 "" 2
# 122 "src/optimizations.c" 1
	vf 0(t0)

# 0 "" 2
 #NO_APP
	ld	a5,8(sp)
	addw	a4,a5,a4
	blt	a4,a0,.L13
	add	a6,a6,1024
	add	t1,t1,8
	bne	a6,t2,.L14
	add	a7,a7,1024
	add	a1,a1,t3
	bne	a7,a6,.L12
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	add	sp,sp,48
	jr	ra
	.size	simd, .-simd
	.align	2
	.globl	optimize
	.type	optimize, @function
optimize:
	tail	simd
	.size	optimize, .-optimize
	.align	2
	.globl	cacheBlock
	.type	cacheBlock, @function
cacheBlock:
	add	sp,sp,-32
	sd	s0,16(sp)
	sd	s2,0(sp)
	mv	s0,a1
	mv	s2,a2
	mv	a1,zero
	li	a2,8388608
	sd	s1,8(sp)
	sd	ra,24(sp)
	mv	s1,a0
	call	memset
	li	a5,4096
	li	t3,8388608
	add	a1,s0,a5
	mv	t2,zero
	add	t6,s1,t3
	li	t5,-4096
	li	t0,8192
	li	t4,4194304
	mv	t1,a5
.L21:
	mv	a7,s1
	mv	a6,a1
	add	s0,s2,t2
.L26:
	add	a2,a6,t5
	mv	a0,s0
	add	a3,a7,t0
.L23:
	fld	ft2,0(a2)
	mv	a4,a0
	mv	a5,a7
.L22:
	fld	ft0,0(a4)
	fld	ft1,0(a5)
	add	a5,a5,8
	fmul.d	ft0,ft2,ft0
	add	a4,a4,8
	fadd.d	ft0,ft1,ft0
	fsd	ft0,-8(a5)
	bne	a3,a5,.L22
	add	a2,a2,8
	add	a0,a0,t0
	bne	a6,a2,.L23
	li	a5,8192
	add	a6,a6,a5
	mv	a7,a3
	bne	t6,a3,.L26
	add	t2,t2,t4
	add	a1,a1,t1
	bne	t2,t3,.L21
	ld	ra,24(sp)
	ld	s0,16(sp)
	ld	s1,8(sp)
	ld	s2,0(sp)
	add	sp,sp,32
	jr	ra
	.size	cacheBlock, .-cacheBlock
	.align	2
	.globl	loopUnroll
	.type	loopUnroll, @function
loopUnroll:
	add	sp,sp,-80
	sd	s1,56(sp)
	li	s1,8388608
	sd	s0,64(sp)
	sd	s2,48(sp)
	mv	s0,a1
	mv	s2,a2
	mv	a1,zero
	mv	a2,s1
	sd	s3,40(sp)
	sd	ra,72(sp)
	mv	s3,a0
	fsd	fs0,24(sp)
	fsd	fs1,16(sp)
	fsd	fs2,8(sp)
	call	memset
	li	a1,8192
	mv	t0,a1
	mv	a7,s3
	add	a1,s0,a1
	add	s1,s3,s1
	li	t2,-8192
	li	a2,1024
	mv	t1,t0
.L33:
	add	a0,a1,t2
	mv	a6,s2
.L32:
	fld	ft0,0(a0)
	mv	a4,a6
	mv	a5,a7
	mv	a3,zero
.L31:
	fld	fs1,0(a4)
	fld	ft11,8(a4)
	fld	ft9,16(a4)
	fld	fa7,24(a4)
	fld	fa5,32(a4)
	fld	fa3,40(a4)
	fld	fa1,48(a4)
	fld	ft7,56(a4)
	fld	ft5,64(a4)
	fld	ft3,72(a4)
	fld	ft1,80(a4)
	fmul.d	fs1,ft0,fs1
	fmul.d	ft11,ft0,ft11
	fmul.d	ft9,ft0,ft9
	fmul.d	fa7,ft0,fa7
	fmul.d	fa5,ft0,fa5
	fmul.d	fa3,ft0,fa3
	fmul.d	fa1,ft0,fa1
	fmul.d	ft7,ft0,ft7
	fmul.d	ft5,ft0,ft5
	fmul.d	ft3,ft0,ft3
	fmul.d	ft1,ft0,ft1
	fld	fs0,8(a5)
	fld	ft10,16(a5)
	fld	ft8,24(a5)
	fld	fa6,32(a5)
	fld	fa4,40(a5)
	fld	fa2,48(a5)
	fld	fa0,56(a5)
	fld	ft6,64(a5)
	fld	ft4,72(a5)
	fld	fs2,0(a5)
	fld	ft2,80(a5)
	fadd.d	ft11,fs0,ft11
	fadd.d	fs1,fs2,fs1
	fadd.d	ft9,ft10,ft9
	fadd.d	fa7,ft8,fa7
	fadd.d	fa5,fa6,fa5
	fadd.d	fa3,fa4,fa3
	fadd.d	fa1,fa2,fa1
	fadd.d	ft7,fa0,ft7
	fadd.d	ft5,ft6,ft5
	fadd.d	ft3,ft4,ft3
	fadd.d	ft1,ft2,ft1
	fsd	fs1,0(a5)
	fsd	ft11,8(a5)
	fsd	ft9,16(a5)
	fsd	fa7,24(a5)
	fsd	fa5,32(a5)
	fsd	fa3,40(a5)
	fsd	fa1,48(a5)
	fsd	ft7,56(a5)
	fsd	ft5,64(a5)
	fsd	ft3,72(a5)
	fsd	ft1,80(a5)
	fld	fs0,88(a4)
	fld	ft11,96(a4)
	fld	ft9,104(a4)
	fld	fa7,112(a4)
	fld	fa5,120(a4)
	fld	fa3,128(a4)
	fld	fa1,136(a4)
	fld	ft7,144(a4)
	fld	ft5,152(a4)
	fld	ft3,160(a4)
	fld	ft1,168(a4)
	fmul.d	fs0,ft0,fs0
	fmul.d	ft11,ft0,ft11
	fmul.d	ft9,ft0,ft9
	fmul.d	fa7,ft0,fa7
	fmul.d	fa5,ft0,fa5
	fmul.d	fa3,ft0,fa3
	fmul.d	fa1,ft0,fa1
	fmul.d	ft7,ft0,ft7
	fmul.d	ft5,ft0,ft5
	fmul.d	ft3,ft0,ft3
	fmul.d	ft1,ft0,ft1
	fld	ft10,104(a5)
	fld	ft8,112(a5)
	fld	fa6,120(a5)
	fld	fa4,128(a5)
	fld	fa2,136(a5)
	fld	fa0,144(a5)
	fld	ft6,152(a5)
	fld	ft4,160(a5)
	fld	ft2,168(a5)
	fld	fs1,88(a5)
	fld	fs2,96(a5)
	fadd.d	ft9,ft10,ft9
	fadd.d	fa7,ft8,fa7
	fadd.d	fa5,fa6,fa5
	fadd.d	fa3,fa4,fa3
	fadd.d	fa1,fa2,fa1
	fadd.d	ft7,fa0,ft7
	fadd.d	ft5,ft6,ft5
	fadd.d	ft3,ft4,ft3
	fadd.d	ft1,ft2,ft1
	fadd.d	fs0,fs1,fs0
	fadd.d	ft11,fs2,ft11
	fsd	ft9,104(a5)
	fsd	fa7,112(a5)
	fsd	fs0,88(a5)
	fsd	ft11,96(a5)
	fsd	fa5,120(a5)
	fsd	fa3,128(a5)
	fsd	fa1,136(a5)
	fsd	ft7,144(a5)
	fsd	ft5,152(a5)
	fsd	ft3,160(a5)
	fsd	ft1,168(a5)
	fld	ft11,176(a4)
	fld	ft9,184(a4)
	fld	fa7,192(a4)
	fld	fa5,200(a4)
	fld	fa3,208(a4)
	fld	fa1,216(a4)
	fld	ft7,224(a4)
	fld	ft5,232(a4)
	fld	ft3,240(a4)
	fld	ft1,248(a4)
	fmul.d	ft11,ft0,ft11
	fmul.d	ft9,ft0,ft9
	fmul.d	fa7,ft0,fa7
	fmul.d	fa5,ft0,fa5
	fmul.d	fa3,ft0,fa3
	fmul.d	fa1,ft0,fa1
	fmul.d	ft7,ft0,ft7
	fmul.d	ft5,ft0,ft5
	fmul.d	ft3,ft0,ft3
	fmul.d	ft1,ft0,ft1
	fld	ft10,184(a5)
	fld	ft8,192(a5)
	fld	fa6,200(a5)
	fld	fa4,208(a5)
	fld	fa2,216(a5)
	fld	fa0,224(a5)
	fld	ft6,232(a5)
	fld	ft4,240(a5)
	fld	ft2,248(a5)
	fld	fs0,176(a5)
	fadd.d	ft9,ft10,ft9
	fadd.d	fa7,ft8,fa7
	fadd.d	ft11,fs0,ft11
	fadd.d	fa5,fa6,fa5
	fadd.d	fa3,fa4,fa3
	fadd.d	fa1,fa2,fa1
	fadd.d	ft7,fa0,ft7
	fadd.d	ft5,ft6,ft5
	fadd.d	ft3,ft4,ft3
	fadd.d	ft1,ft2,ft1
	addw	a3,a3,32
	fsd	ft11,176(a5)
	fsd	ft9,184(a5)
	fsd	fa7,192(a5)
	fsd	fa5,200(a5)
	fsd	fa3,208(a5)
	fsd	fa1,216(a5)
	fsd	ft7,224(a5)
	fsd	ft5,232(a5)
	fsd	ft3,240(a5)
	fsd	ft1,248(a5)
	add	a4,a4,256
	add	a5,a5,256
	bne	a3,a2,.L31
	add	a0,a0,8
	add	a6,a6,t0
	bne	a0,a1,.L32
	add	a7,a7,t1
	add	a1,a0,t1
	bne	a7,s1,.L33
	ld	ra,72(sp)
	ld	s0,64(sp)
	ld	s1,56(sp)
	ld	s2,48(sp)
	ld	s3,40(sp)
	fld	fs0,24(sp)
	fld	fs1,16(sp)
	fld	fs2,8(sp)
	add	sp,sp,80
	jr	ra
	.size	loopUnroll, .-loopUnroll
	.align	2
	.globl	registerBlock
	.type	registerBlock, @function
registerBlock:
	add	sp,sp,-48
	sd	s1,24(sp)
	li	s1,8388608
	sd	s0,32(sp)
	sd	s2,16(sp)
	mv	s0,a1
	mv	s2,a2
	mv	a1,zero
	mv	a2,s1
	sd	s3,8(sp)
	sd	ra,40(sp)
	mv	s3,a0
	call	memset
	li	t0,8192
	mv	t1,s3
	add	a1,s0,t0
	add	s1,s3,s1
	li	t2,-8192
	li	t4,16384
	li	t3,32768
.L41:
	add	a7,a1,t2
	add	a6,s2,t0
.L40:
	fld	fa7,0(a7)
	fld	fa6,8(a7)
	fld	fa5,16(a7)
	fld	fa4,24(a7)
	add	a4,a6,t2
	add	a0,a6,t0
	add	a2,a6,t4
	mv	a3,a6
	mv	a5,t1
.L39:
	fld	ft0,0(a4)
	fld	ft6,8(a4)
	fld	ft4,16(a4)
	fld	ft2,24(a4)
	fmul.d	ft7,fa7,ft0
	fmul.d	ft5,fa7,ft6
	fmul.d	ft3,fa7,ft4
	fmul.d	ft1,fa7,ft2
	fld	ft0,0(a3)
	fld	ft6,8(a3)
	fld	ft4,16(a3)
	fld	ft2,24(a3)
	fmul.d	ft11,fa6,ft0
	fmul.d	ft10,fa6,ft6
	fmul.d	ft9,fa6,ft4
	fmul.d	ft8,fa6,ft2
	fld	ft0,0(a5)
	fld	ft6,8(a5)
	fld	ft4,16(a5)
	fld	ft2,24(a5)
	fld	fa0,0(a0)
	fld	fa3,8(a0)
	fld	fa2,16(a0)
	fld	fa1,24(a0)
	fadd.d	ft0,ft7,ft0
	fadd.d	ft6,ft5,ft6
	fadd.d	ft4,ft3,ft4
	fadd.d	ft2,ft1,ft2
	fmul.d	fa0,fa5,fa0
	fmul.d	fa3,fa5,fa3
	fmul.d	fa2,fa5,fa2
	fmul.d	fa1,fa5,fa1
	fld	ft7,8(a2)
	fld	ft5,16(a2)
	fld	ft3,24(a2)
	fld	ft1,0(a2)
	fadd.d	ft0,ft11,ft0
	fadd.d	ft6,ft10,ft6
	fadd.d	ft4,ft9,ft4
	fadd.d	ft2,ft8,ft2
	fmul.d	ft7,fa4,ft7
	fmul.d	ft5,fa4,ft5
	fmul.d	ft3,fa4,ft3
	fmul.d	ft1,fa4,ft1
	fadd.d	ft0,fa0,ft0
	fadd.d	ft6,fa3,ft6
	fadd.d	ft4,fa2,ft4
	fadd.d	ft2,fa1,ft2
	add	a4,a4,32
	fadd.d	ft6,ft7,ft6
	fadd.d	ft4,ft5,ft4
	fadd.d	ft2,ft3,ft2
	fadd.d	ft0,ft1,ft0
	fsd	ft6,8(a5)
	fsd	ft4,16(a5)
	fsd	ft2,24(a5)
	fsd	ft0,0(a5)
	add	a3,a3,32
	add	a5,a5,32
	add	a0,a0,32
	add	a2,a2,32
	bne	a6,a4,.L39
	add	a7,a7,32
	add	a6,a6,t3
	bne	a7,a1,.L40
	add	t1,t1,t0
	add	a1,a7,t0
	bne	t1,s1,.L41
	ld	ra,40(sp)
	ld	s0,32(sp)
	ld	s1,24(sp)
	ld	s2,16(sp)
	ld	s3,8(sp)
	add	sp,sp,48
	jr	ra
	.size	registerBlock, .-registerBlock
	.align	2
	.globl	openmp_simd
	.type	openmp_simd, @function
openmp_simd:
	ret
	.size	openmp_simd, .-openmp_simd
	.align	2
	.globl	openmp_simd_cacheBlock
	.type	openmp_simd_cacheBlock, @function
openmp_simd_cacheBlock:
	ret
	.size	openmp_simd_cacheBlock, .-openmp_simd_cacheBlock
	.align	2
	.globl	openmp_simd_cacheBlock_loopUnroll
	.type	openmp_simd_cacheBlock_loopUnroll, @function
openmp_simd_cacheBlock_loopUnroll:
	ret
	.size	openmp_simd_cacheBlock_loopUnroll, .-openmp_simd_cacheBlock_loopUnroll
	.align	2
	.globl	openmp_simd_cacheBlock_loopUnroll_registerBlock
	.type	openmp_simd_cacheBlock_loopUnroll_registerBlock, @function
openmp_simd_cacheBlock_loopUnroll_registerBlock:
	ret
	.size	openmp_simd_cacheBlock_loopUnroll_registerBlock, .-openmp_simd_cacheBlock_loopUnroll_registerBlock
	.ident	"GCC: (GNU) 5.1.0"
