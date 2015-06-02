	.file	"main.c"
	.text
	.type	_mm_malloc, @function
_mm_malloc:
.LFB129:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	cmpq	$1, -32(%rbp)
	jne	.L2
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	malloc
	jmp	.L7
.L2:
	cmpq	$2, -32(%rbp)
	je	.L4
	cmpq	$4, -32(%rbp)
	jne	.L5
.L4:
	movq	$8, -32(%rbp)
.L5:
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	leaq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	posix_memalign
	testl	%eax, %eax
	jne	.L6
	movq	-8(%rbp), %rax
	jmp	.L7
.L6:
	movl	$0, %eax
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE129:
	.size	_mm_malloc, .-_mm_malloc
	.type	_mm_free, @function
_mm_free:
.LFB130:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE130:
	.size	_mm_free, .-_mm_free
	.type	timestamp_us, @function
timestamp_us:
.LFB880:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	leaq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday
	movq	-16(%rbp), %rax
	imulq	$1000000, %rax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE880:
	.size	timestamp_us, .-timestamp_us
	.globl	prefetch_matrices
	.type	prefetch_matrices, @function
prefetch_matrices:
.LFB881:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE881:
	.size	prefetch_matrices, .-prefetch_matrices
	.section	.rodata
.LC3:
	.string	"naive:"
.LC4:
	.string	"%-60s%.6f\n"
.LC5:
	.string	"openmp:"
.LC6:
	.string	"%-60s%.6f speedup: %.6f\n"
.LC7:
	.string	"simd:"
.LC8:
	.string	"cache blocking:"
.LC9:
	.string	"loop unrolling:"
.LC10:
	.string	"register blocking:"
.LC11:
	.string	"openmp+simd:"
.LC12:
	.string	"openmp+simd+cache blocking:"
	.align 8
.LC13:
	.string	"openmp+simd+cache blocking+loop unrolling:"
	.align 8
.LC14:
	.string	"openmp+simd+cache blocking+loop unrolling+register blocking:"
.LC15:
	.string	"The result of openmp is wrong"
.LC16:
	.string	"The result of simd is wrong"
	.align 8
.LC17:
	.string	"The result of cache blocking is wrong"
	.align 8
.LC18:
	.string	"The result of loop unrolling is wrong"
	.align 8
.LC19:
	.string	"The result of register blocking is wrong"
	.align 8
.LC20:
	.string	"The result of openmp simd is wrong"
	.align 8
.LC21:
	.string	"The result of openmp+simd+cache blocking is wrong"
	.align 8
.LC22:
	.string	"The result of openmp+simd+cache blocking+loop unrolling is wrong"
	.align 8
.LC23:
	.string	"The result of openmp+simd+cache blocking+loop unrolling+register blocking is wrong"
	.text
	.globl	main
	.type	main, @function
main:
.LFB882:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$360, %rsp
	.cfi_offset 3, -24
	movl	%edi, -340(%rbp)
	movq	%rsi, -352(%rbp)
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	movl	$8, %edi
	call	omp_set_num_threads
	movl	$0, %eax
	movq	%rax, -200(%rbp)
	movl	$0, %eax
	movq	%rax, -192(%rbp)
	movl	$0, %eax
	movq	%rax, -184(%rbp)
	movl	$0, %eax
	movq	%rax, -176(%rbp)
	movl	$0, %eax
	movq	%rax, -168(%rbp)
	movl	$0, %eax
	movq	%rax, -160(%rbp)
	movl	$0, %eax
	movq	%rax, -152(%rbp)
	movl	$0, %eax
	movq	%rax, -144(%rbp)
	movl	$0, %eax
	movq	%rax, -136(%rbp)
	movl	$0, %eax
	movq	%rax, -128(%rbp)
	movl	$0, -328(%rbp)
	movl	$0, -324(%rbp)
	movl	$0, -320(%rbp)
	movl	$0, -316(%rbp)
	movl	$0, -312(%rbp)
	movl	$0, -308(%rbp)
	movl	$0, -304(%rbp)
	movl	$0, -300(%rbp)
	movl	$0, -244(%rbp)
	movl	$0, -240(%rbp)
	movl	$0, -236(%rbp)
	movl	$0, -232(%rbp)
	movl	$0, -228(%rbp)
	movl	$0, -224(%rbp)
	movl	$0, -220(%rbp)
	movl	$0, -216(%rbp)
	movl	$0, -212(%rbp)
	movl	$1, -208(%rbp)
	movl	$1, -204(%rbp)
	movl	$0, -296(%rbp)
	jmp	.L13
.L76:
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -48(%rbp)
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -40(%rbp)
	movl	$0, -292(%rbp)
	jmp	.L14
.L17:
	movl	$0, -288(%rbp)
	jmp	.L15
.L16:
	movl	-292(%rbp), %eax
	sall	$10, %eax
	movl	%eax, %edx
	movl	-288(%rbp), %eax
	addl	%edx, %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	leaq	(%rdx,%rax), %rbx
	call	rand
	vcvtsi2sd	%eax, %xmm0, %xmm0
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	%xmm0, (%rbx)
	movl	-292(%rbp), %eax
	sall	$10, %eax
	movl	%eax, %edx
	movl	-288(%rbp), %eax
	addl	%edx, %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	leaq	(%rdx,%rax), %rbx
	call	rand
	vcvtsi2sd	%eax, %xmm0, %xmm0
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	%xmm0, (%rbx)
	addl	$1, -288(%rbp)
.L15:
	cmpl	$1023, -288(%rbp)
	jle	.L16
	addl	$1, -292(%rbp)
.L14:
	cmpl	$1023, -292(%rbp)
	jle	.L17
	movl	$0, -284(%rbp)
	jmp	.L18
.L19:
	movl	-284(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-284(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -284(%rbp)
.L18:
	cmpl	$1048575, -284(%rbp)
	jle	.L19
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -32(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L20
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L21
.L20:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L21:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-200(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -200(%rbp)
	cmpl	$0, -240(%rbp)
	je	.L22
	movl	$0, -280(%rbp)
	jmp	.L23
.L24:
	movl	-280(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-280(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -280(%rbp)
.L23:
	cmpl	$1048575, -280(%rbp)
	jle	.L24
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -120(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-120(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_openmp
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L25
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L26
.L25:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L26:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-192(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -192(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L22
	movq	-32(%rbp), %rdx
	movq	-120(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -328(%rbp)
.L22:
	cmpl	$0, -236(%rbp)
	je	.L27
	movl	$0, -276(%rbp)
	jmp	.L28
.L29:
	movl	-276(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-276(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -276(%rbp)
.L28:
	cmpl	$1048575, -276(%rbp)
	jle	.L29
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -112(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-112(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_simd
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L30
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L31
.L30:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L31:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-184(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -184(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L27
	movq	-32(%rbp), %rdx
	movq	-112(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -324(%rbp)
.L27:
	cmpl	$0, -232(%rbp)
	je	.L32
	movl	$0, -272(%rbp)
	jmp	.L33
.L34:
	movl	-272(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-272(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -272(%rbp)
.L33:
	cmpl	$1048575, -272(%rbp)
	jle	.L34
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -104(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-104(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_cache_blocking
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L35
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L36
.L35:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L36:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-176(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -176(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L32
	movq	-32(%rbp), %rdx
	movq	-104(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -320(%rbp)
.L32:
	cmpl	$0, -228(%rbp)
	je	.L37
	movl	$0, -268(%rbp)
	jmp	.L38
.L39:
	movl	-268(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-268(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -268(%rbp)
.L38:
	cmpl	$1048575, -268(%rbp)
	jle	.L39
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -96(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-96(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_loop_unrolling
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L40
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L41
.L40:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L41:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-168(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -168(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L37
	movq	-32(%rbp), %rdx
	movq	-96(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -316(%rbp)
.L37:
	cmpl	$0, -224(%rbp)
	je	.L42
	movl	$0, -264(%rbp)
	jmp	.L43
.L44:
	movl	-264(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-264(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -264(%rbp)
.L43:
	cmpl	$1048575, -264(%rbp)
	jle	.L44
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -88(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-88(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_register_blocking
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L45
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L46
.L45:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L46:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-160(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -160(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L42
	movq	-32(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -312(%rbp)
.L42:
	cmpl	$0, -220(%rbp)
	je	.L47
	movl	$0, -260(%rbp)
	jmp	.L48
.L49:
	movl	-260(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-260(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -260(%rbp)
.L48:
	cmpl	$1048575, -260(%rbp)
	jle	.L49
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -80(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-80(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_openmp_simd
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L50
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L51
.L50:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L51:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-152(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -152(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L47
	movq	-32(%rbp), %rdx
	movq	-80(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -308(%rbp)
.L47:
	cmpl	$0, -216(%rbp)
	je	.L52
	movl	$0, -256(%rbp)
	jmp	.L53
.L54:
	movl	-256(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-256(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -256(%rbp)
.L53:
	cmpl	$1048575, -256(%rbp)
	jle	.L54
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -72(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-72(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_openmp_simd_cache_blocking
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L55
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L56
.L55:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L56:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-144(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -144(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L52
	movq	-32(%rbp), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -304(%rbp)
.L52:
	cmpl	$0, -212(%rbp)
	je	.L57
	movl	$0, -252(%rbp)
	jmp	.L58
.L59:
	movl	-252(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-252(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -252(%rbp)
.L58:
	cmpl	$1048575, -252(%rbp)
	jle	.L59
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -64(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-64(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_openmp_simd_cache_blocking_loop_unrolling
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L60
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L61
.L60:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L61:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-136(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -136(%rbp)
	cmpl	$0, -296(%rbp)
	jne	.L57
	movq	-32(%rbp), %rdx
	movq	-64(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	compare_matrix
	movl	%eax, -300(%rbp)
.L57:
	cmpl	$0, -208(%rbp)
	je	.L62
	movl	$0, -248(%rbp)
	jmp	.L63
.L64:
	movl	-248(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	movl	-248(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	prefetcht2	(%rax)
	addl	$8, -248(%rbp)
.L63:
	cmpl	$1048575, -248(%rbp)
	jle	.L64
	movl	$64, %esi
	movl	$8388608, %edi
	call	_mm_malloc
	movq	%rax, -56(%rbp)
	movl	$0, %eax
	call	timestamp_us
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rdx
	movq	-48(%rbp), %rcx
	movq	-56(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	optimization_openmp_simd_cache_register_blocking_loop_unrolling
	movl	$0, %eax
	call	timestamp_us
	subq	-24(%rbp), %rax
	testq	%rax, %rax
	js	.L65
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L66
.L65:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L66:
	vmovsd	.LC2(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	vmovsd	-128(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -128(%rbp)
.L62:
	cmpl	$0, -240(%rbp)
	je	.L67
	movq	-120(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L67:
	cmpl	$0, -236(%rbp)
	je	.L68
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L68:
	cmpl	$0, -232(%rbp)
	je	.L69
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L69:
	cmpl	$0, -228(%rbp)
	je	.L70
	movq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L70:
	cmpl	$0, -224(%rbp)
	je	.L71
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L71:
	cmpl	$0, -220(%rbp)
	je	.L72
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L72:
	cmpl	$0, -216(%rbp)
	je	.L73
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L73:
	cmpl	$0, -212(%rbp)
	je	.L74
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L74:
	cmpl	$0, -208(%rbp)
	je	.L75
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
.L75:
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_mm_free
	addl	$1, -296(%rbp)
.L13:
	movl	-296(%rbp), %eax
	cmpl	-204(%rbp), %eax
	jl	.L76
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-200(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -200(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-192(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -192(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-184(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -184(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-176(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -176(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-168(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -168(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-160(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -160(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-152(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -152(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-144(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -144(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-136(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -136(%rbp)
	vcvtsi2sd	-204(%rbp), %xmm0, %xmm0
	vmovsd	-128(%rbp), %xmm1
	vdivsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -128(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC3, %esi
	movl	$.LC4, %edi
	movl	$1, %eax
	call	printf
	cmpl	$0, -240(%rbp)
	je	.L77
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-192(%rbp), %xmm0, %xmm0
	movq	-192(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC5, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L77:
	cmpl	$0, -236(%rbp)
	je	.L78
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-184(%rbp), %xmm0, %xmm0
	movq	-184(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC7, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L78:
	cmpl	$0, -232(%rbp)
	je	.L79
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-176(%rbp), %xmm0, %xmm0
	movq	-176(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC8, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L79:
	cmpl	$0, -228(%rbp)
	je	.L80
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-168(%rbp), %xmm0, %xmm0
	movq	-168(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC9, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L80:
	cmpl	$0, -224(%rbp)
	je	.L81
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-160(%rbp), %xmm0, %xmm0
	movq	-160(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC10, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L81:
	cmpl	$0, -220(%rbp)
	je	.L82
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-152(%rbp), %xmm0, %xmm0
	movq	-152(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC11, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L82:
	cmpl	$0, -216(%rbp)
	je	.L83
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-144(%rbp), %xmm0, %xmm0
	movq	-144(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC12, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L83:
	cmpl	$0, -212(%rbp)
	je	.L84
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-136(%rbp), %xmm0, %xmm0
	movq	-136(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC13, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L84:
	cmpl	$0, -208(%rbp)
	je	.L85
	vmovsd	-200(%rbp), %xmm0
	vdivsd	-128(%rbp), %xmm0, %xmm0
	movq	-128(%rbp), %rax
	vmovapd	%xmm0, %xmm1
	movq	%rax, -360(%rbp)
	vmovsd	-360(%rbp), %xmm0
	movl	$.LC14, %esi
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
.L85:
	cmpl	$0, -328(%rbp)
	je	.L86
	movl	$.LC15, %edi
	call	puts
.L86:
	cmpl	$0, -324(%rbp)
	je	.L87
	movl	$.LC16, %edi
	call	puts
.L87:
	cmpl	$0, -320(%rbp)
	je	.L88
	movl	$.LC17, %edi
	call	puts
.L88:
	cmpl	$0, -316(%rbp)
	je	.L89
	movl	$.LC18, %edi
	call	puts
.L89:
	cmpl	$0, -312(%rbp)
	je	.L90
	movl	$.LC19, %edi
	call	puts
.L90:
	cmpl	$0, -308(%rbp)
	je	.L91
	movl	$.LC20, %edi
	call	puts
.L91:
	cmpl	$0, -304(%rbp)
	je	.L92
	movl	$.LC21, %edi
	call	puts
.L92:
	cmpl	$0, -300(%rbp)
	je	.L93
	movl	$.LC22, %edi
	call	puts
.L93:
	cmpl	$0, -244(%rbp)
	je	.L94
	movl	$.LC23, %edi
	call	puts
.L94:
	movl	$0, %eax
	addq	$360, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE882:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	4290772992
	.long	1105199103
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
