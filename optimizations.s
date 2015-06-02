	.file	"optimizations.c"
	.text
	.type	optimization_openmp._omp_fn.0, @function
optimization_openmp._omp_fn.0:
.LFB1055:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	call	omp_get_num_threads
	movl	%eax, %ebp
	call	omp_get_thread_num
	movl	%eax, %ecx
	movl	$1024, %eax
	cltd
	idivl	%ebp
	movl	%eax, %r12d
	movl	%edx, %esi
	cmpl	%edx, %ecx
	jl	.L2
.L10:
	imull	%r12d, %ecx
	leal	(%rcx,%rsi), %ebp
	addl	%ebp, %r12d
	cmpl	%r12d, %ebp
	jge	.L1
	movl	%ebp, %edi
	sall	$10, %edi
	vxorpd	%xmm2, %xmm2, %xmm2
.L5:
	movl	$0, %r10d
.L7:
	movq	8(%rbx), %r9
	movq	16(%rbx), %r8
	movl	%r10d, %r11d
	movl	%r10d, %edx
	movl	$0, %eax
	vmovapd	%xmm2, %xmm0
	jmp	.L4
.L6:
	addl	$1, %ebp
	addl	$1024, %edi
	cmpl	%r12d, %ebp
	jne	.L5
	jmp	.L1
.L11:
	leal	(%r11,%rdi), %edx
	movslq	%edx, %rdx
	movq	(%rbx), %rax
	vmovsd	%xmm0, (%rax,%rdx,8)
	addl	$1, %r10d
	cmpl	$1024, %r10d
	jne	.L7
	jmp	.L6
.L4:
	leal	(%rax,%rdi), %esi
	movslq	%esi, %rsi
	movslq	%edx, %rcx
	vmovsd	(%r9,%rsi,8), %xmm1
	vmulsd	(%r8,%rcx,8), %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	addl	$1, %eax
	addl	$1024, %edx
	cmpl	$1024, %eax
	jne	.L4
	jmp	.L11
.L2:
	leal	1(%rax), %r12d
	movl	$0, %esi
	jmp	.L10
.L1:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1055:
	.size	optimization_openmp._omp_fn.0, .-optimization_openmp._omp_fn.0
	.type	optimization_openmp_simd._omp_fn.1, @function
optimization_openmp_simd._omp_fn.1:
.LFB1056:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	call	omp_get_num_threads
	movl	%eax, %r12d
	call	omp_get_thread_num
	movl	%eax, %esi
	movl	$1024, %eax
	cltd
	idivl	%r12d
	movl	%eax, %ecx
	movl	%edx, %edi
	cmpl	%edx, %esi
	jl	.L14
.L22:
	imull	%ecx, %esi
	leal	(%rsi,%rdi), %r9d
	leal	(%r9,%rcx), %r11d
	cmpl	%r11d, %r9d
	jge	.L13
	movl	%r9d, %r10d
	sall	$10, %r10d
	vxorpd	%xmm2, %xmm2, %xmm2
.L17:
	movslq	%r10d, %r8
	salq	$3, %r8
	leaq	8192(%r8), %rsi
	movl	$0, %edi
.L19:
	movq	%rdi, %rdx
	movq	%r8, %rax
	vmovapd	%ymm2, %ymm0
	jmp	.L16
.L18:
	addl	$1, %r9d
	addl	$1024, %r10d
	cmpl	%r9d, %r11d
	jg	.L17
	jmp	.L13
.L23:
	movq	%rdi, %rax
	addq	(%rbx), %rax
	vmovupd	%ymm0, (%rax,%r8)
	addq	$32, %rdi
	cmpq	$8192, %rdi
	jne	.L19
	jmp	.L18
.L16:
	movq	%rax, %rcx
	addq	8(%rbx), %rcx
	vbroadcastsd	(%rcx), %ymm1
	movq	16(%rbx), %rcx
	vfmadd231pd	(%rcx,%rdx), %ymm1, %ymm0
	addq	$8, %rax
	addq	$8192, %rdx
	cmpq	%rsi, %rax
	jne	.L16
	jmp	.L23
.L14:
	leal	1(%rax), %ecx
	movl	$0, %edi
	jmp	.L22
.L13:
	leaq	-16(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1056:
	.size	optimization_openmp_simd._omp_fn.1, .-optimization_openmp_simd._omp_fn.1
	.type	optimization_openmp_simd_cache_blocking._omp_fn.2, @function
optimization_openmp_simd_cache_blocking._omp_fn.2:
.LFB1057:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$16, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	call	omp_get_num_threads
	movl	%eax, %r12d
	call	omp_get_thread_num
	movl	%eax, %esi
	movl	$1024, %eax
	cltd
	idivl	%r12d
	movl	%eax, %ecx
	movl	%edx, %edi
	cmpl	%edx, %esi
	jl	.L26
.L37:
	imull	%ecx, %esi
	leal	(%rsi,%rdi), %r14d
	leal	(%r14,%rcx), %eax
	movl	%eax, 8(%rsp)
	cmpl	%eax, %r14d
	jge	.L25
	movl	%r14d, %ecx
	sall	$10, %ecx
.L29:
	movslq	%ecx, %r10
	leaq	0(,%r10,8), %rsi
	leaq	8192(%rsi), %r13
	movl	$0, %r12d
	movl	%r14d, %edi
.L31:
	movl	%r12d, %r15d
	sall	$10, %r15d
	movq	%rsi, %r14
	movl	$0, %r9d
	leal	8(%r12), %r11d
	jmp	.L28
.L41:
	movl	%edi, %r14d
	addl	$1, %r14d
	addl	$1024, %ecx
	cmpl	%r14d, 8(%rsp)
	jg	.L29
	jmp	.L25
.L42:
	addl	$8, %r12d
	cmpl	$1024, %r12d
	jne	.L31
	jmp	.L41
.L28:
	movq	(%rbx), %rax
	vmovapd	(%rax,%r14), %ymm0
	movl	%r15d, %edx
	movl	%r12d, %eax
	cmpl	%r12d, %r11d
	jg	.L40
.L35:
	movq	(%rbx), %rax
	vmovapd	%ymm0, (%rax,%r14)
	addq	$32, %r14
	addq	$4, %r9
	cmpq	%r13, %r14
	jne	.L28
	jmp	.L42
.L40:
	movl	%ecx, 12(%rsp)
.L38:
	movslq	%eax, %r8
	addq	%r10, %r8
	movq	8(%rbx), %rcx
	vbroadcastsd	(%rcx,%r8,8), %ymm1
	movslq	%edx, %r8
	addq	%r9, %r8
	movq	16(%rbx), %rcx
	vfmadd231pd	(%rcx,%r8,8), %ymm1, %ymm0
	addl	$1, %eax
	addl	$1024, %edx
	cmpl	%eax, %r11d
	jg	.L38
	movl	12(%rsp), %ecx
	jmp	.L35
.L26:
	leal	1(%rax), %ecx
	movl	$0, %edi
	jmp	.L37
.L25:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1057:
	.size	optimization_openmp_simd_cache_blocking._omp_fn.2, .-optimization_openmp_simd_cache_blocking._omp_fn.2
	.type	optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3, @function
optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3:
.LFB1058:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	.cfi_offset 14, -24
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -48
	movq	%rdi, %rbx
	call	omp_get_num_threads
	movl	%eax, %r12d
	call	omp_get_thread_num
	movl	%eax, %esi
	movl	$1024, %eax
	cltd
	idivl	%r12d
	movl	%eax, %ecx
	movl	%edx, %edi
	cmpl	%edx, %esi
	jl	.L45
.L53:
	imull	%ecx, %esi
	leal	(%rsi,%rdi), %r12d
	leal	(%r12,%rcx), %r14d
	cmpl	%r14d, %r12d
	jge	.L44
	movl	%r12d, %r13d
	sall	$10, %r13d
.L48:
	movslq	%r13d, %rax
	leaq	0(,%rax,8), %rdi
	negq	%rax
	leaq	0(,%rax,8), %r8
	leaq	8192(%rdi), %r10
	movq	%rdi, %r11
	movq	%r10, %r9
.L50:
	movq	%r11, %rcx
	jmp	.L47
.L49:
	addl	$1, %r12d
	addl	$1024, %r13d
	cmpl	%r12d, %r14d
	jg	.L48
	jmp	.L44
.L54:
	addq	$64, %rdi
	addq	$65536, %r8
	cmpq	%r10, %rdi
	jne	.L50
	jmp	.L49
.L47:
	movq	(%rbx), %rsi
	movq	%rdi, %rdx
	addq	8(%rbx), %rdx
	leaq	(%r8,%rcx), %rax
	addq	16(%rbx), %rax
	vbroadcastsd	(%rdx), %ymm0
	vmovapd	(%rsi,%rcx), %ymm2
	vfmadd132pd	(%rax), %ymm2, %ymm0
	vbroadcastsd	8(%rdx), %ymm1
	vfmadd231pd	8192(%rax), %ymm1, %ymm0
	vbroadcastsd	16(%rdx), %ymm1
	vfmadd231pd	16384(%rax), %ymm1, %ymm0
	vbroadcastsd	24(%rdx), %ymm1
	vfmadd231pd	24576(%rax), %ymm1, %ymm0
	vbroadcastsd	32(%rdx), %ymm1
	vfmadd231pd	32768(%rax), %ymm1, %ymm0
	vbroadcastsd	40(%rdx), %ymm1
	vfmadd231pd	40960(%rax), %ymm1, %ymm0
	vbroadcastsd	48(%rdx), %ymm1
	vfmadd231pd	49152(%rax), %ymm1, %ymm0
	vbroadcastsd	56(%rdx), %ymm1
	vfmadd231pd	57344(%rax), %ymm1, %ymm0
	vmovapd	%ymm0, (%rsi,%rcx)
	addq	$32, %rcx
	cmpq	%r9, %rcx
	jne	.L47
	jmp	.L54
.L45:
	leal	1(%rax), %ecx
	movl	$0, %edi
	jmp	.L53
.L44:
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1058:
	.size	optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3, .-optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3
	.type	optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4, @function
optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4:
.LFB1059:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	.cfi_offset 14, -24
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -48
	movq	%rdi, %rbx
	call	omp_get_num_threads
	movl	%eax, %r12d
	call	omp_get_thread_num
	movl	%eax, %ecx
	movl	$1024, %eax
	cltd
	idivl	%r12d
	movl	%eax, %r11d
	movl	%edx, %esi
	cmpl	%edx, %ecx
	jl	.L57
.L71:
	imull	%r11d, %ecx
	leal	(%rcx,%rsi), %r9d
	addl	%r9d, %r11d
	cmpl	%r11d, %r9d
	jge	.L56
	movl	%r9d, %r10d
	sall	$10, %r10d
.L60:
	movslq	%r10d, %rax
	leaq	0(,%rax,8), %r8
	negq	%rax
	leaq	0(,%rax,8), %r14
	leaq	8160(%r8), %r13
	movq	%r8, %r12
	leaq	8192(%r8), %rdi
	movq	%r14, %rsi
	jmp	.L59
.L72:
	addl	$1, %r9d
	addl	$1024, %r10d
	cmpl	%r9d, %r11d
	jg	.L60
	jmp	.L56
.L68:
	movq	%rdi, %rax
	addq	8(%rbx), %rax
	vbroadcastsd	(%rax), %ymm0
	movq	%r12, %rax
	jmp	.L61
.L73:
	addq	$8, %rdi
	addq	$8192, %r14
	cmpq	%r8, %rdi
	jne	.L68
	jmp	.L72
.L61:
	movq	%rax, %rdx
	addq	(%rbx), %rdx
	leaq	(%r14,%rax), %rcx
	addq	16(%rbx), %rcx
	vmovapd	(%rcx), %ymm2
	vfmadd213pd	(%rdx), %ymm0, %ymm2
	vmovapd	32(%rcx), %ymm1
	vfmadd213pd	32(%rdx), %ymm0, %ymm1
	vmovapd	%ymm2, (%rdx)
	vmovapd	%ymm1, 32(%rdx)
	addq	$64, %rax
	cmpq	%rsi, %rax
	jne	.L61
	jmp	.L73
.L59:
	movq	%r8, %rax
	addq	8(%rbx), %rax
	vbroadcastsd	(%rax), %ymm11
	vbroadcastsd	8(%rax), %ymm10
	vbroadcastsd	16(%rax), %ymm9
	vbroadcastsd	24(%rax), %ymm8
	vbroadcastsd	32(%rax), %ymm7
	vbroadcastsd	40(%rax), %ymm6
	vbroadcastsd	48(%rax), %ymm5
	vbroadcastsd	56(%rax), %ymm4
	vbroadcastsd	64(%rax), %ymm3
	vbroadcastsd	72(%rax), %ymm2
	vbroadcastsd	80(%rax), %ymm1
	vbroadcastsd	88(%rax), %ymm0
	movq	%r12, %rdx
	jmp	.L66
.L74:
	addq	$96, %r8
	addq	$98304, %rsi
	cmpq	%r13, %r8
	jne	.L59
	leaq	8160(%r12), %rdi
	addq	$8355840, %r14
	leaq	8192(%r12), %r8
	movq	%r8, %rsi
	jmp	.L68
.L66:
	movq	%rdx, %rcx
	addq	(%rbx), %rcx
	leaq	(%rsi,%rdx), %rax
	addq	16(%rbx), %rax
	vmovapd	(%rax), %ymm13
	vfmadd213pd	(%rcx), %ymm11, %ymm13
	vmovapd	32(%rax), %ymm12
	vfmadd213pd	32(%rcx), %ymm11, %ymm12
	vfmadd231pd	8192(%rax), %ymm10, %ymm13
	vfmadd231pd	8224(%rax), %ymm10, %ymm12
	vfmadd231pd	16384(%rax), %ymm9, %ymm13
	vfmadd231pd	16416(%rax), %ymm9, %ymm12
	vfmadd231pd	24576(%rax), %ymm8, %ymm13
	vfmadd231pd	24608(%rax), %ymm8, %ymm12
	vfmadd231pd	32768(%rax), %ymm7, %ymm13
	vfmadd231pd	32800(%rax), %ymm7, %ymm12
	vfmadd231pd	40960(%rax), %ymm6, %ymm13
	vfmadd231pd	40992(%rax), %ymm6, %ymm12
	vfmadd231pd	49152(%rax), %ymm5, %ymm13
	vfmadd231pd	49184(%rax), %ymm5, %ymm12
	vfmadd231pd	57344(%rax), %ymm4, %ymm13
	vfmadd231pd	57376(%rax), %ymm4, %ymm12
	vfmadd231pd	65536(%rax), %ymm3, %ymm13
	vfmadd231pd	65568(%rax), %ymm3, %ymm12
	vfmadd231pd	73728(%rax), %ymm2, %ymm13
	vfmadd231pd	73760(%rax), %ymm2, %ymm12
	vfmadd231pd	81920(%rax), %ymm1, %ymm13
	vfmadd231pd	81952(%rax), %ymm1, %ymm12
	vfmadd231pd	90112(%rax), %ymm0, %ymm13
	vfmadd231pd	90144(%rax), %ymm0, %ymm12
	vmovapd	%ymm13, (%rcx)
	vmovapd	%ymm12, 32(%rcx)
	addq	$64, %rdx
	cmpq	%rdi, %rdx
	jne	.L66
	jmp	.L74
.L57:
	leal	1(%rax), %r11d
	movl	$0, %esi
	jmp	.L71
.L56:
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1059:
	.size	optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4, .-optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4
	.globl	optimization_naive
	.type	optimization_naive, @function
optimization_naive:
.LFB1045:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, %r10d
	vxorpd	%xmm2, %xmm2, %xmm2
	jmp	.L77
.L80:
	leal	(%rax,%r10), %r9d
	movslq	%r9d, %r9
	movslq	%ecx, %r8
	vmovsd	(%rsi,%r9,8), %xmm1
	vmulsd	(%rdx,%r8,8), %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	addl	$1, %eax
	addl	$1024, %ecx
	cmpl	$1024, %eax
	jne	.L80
	leal	(%r10,%rdi), %eax
	cltq
	vmovsd	%xmm0, (%rbx,%rax,8)
	addl	$1, %r11d
	cmpl	$1024, %r11d
	je	.L79
.L82:
	movl	%r11d, %edi
	movl	%r11d, %ecx
	movl	$0, %eax
	vmovapd	%xmm2, %xmm0
	jmp	.L80
.L79:
	addl	$1024, %r10d
	cmpl	$1048576, %r10d
	je	.L76
.L77:
	movl	$0, %r11d
	jmp	.L82
.L76:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1045:
	.size	optimization_naive, .-optimization_naive
	.globl	optimization_openmp
	.type	optimization_openmp, @function
optimization_openmp:
.LFB1046:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, (%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, 16(%rsp)
	movl	$0, %edx
	movq	%rsp, %rsi
	movl	$optimization_openmp._omp_fn.0, %edi
	call	GOMP_parallel_start
	movq	%rsp, %rdi
	call	optimization_openmp._omp_fn.0
	call	GOMP_parallel_end
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1046:
	.size	optimization_openmp, .-optimization_openmp
	.globl	optimization_simd
	.type	optimization_simd, @function
optimization_simd:
.LFB1047:
	.cfi_startproc
	movq	%rdi, %r8
	leaq	8388608(%rdi), %r9
	vxorpd	%xmm2, %xmm2, %xmm2
	jmp	.L87
.L90:
	vbroadcastsd	(%rsi,%rax), %ymm1
	vfmadd231pd	(%rcx), %ymm1, %ymm0
	addq	$8, %rax
	addq	$8192, %rcx
	cmpq	$8192, %rax
	jne	.L90
	vmovupd	%ymm0, (%r8,%rdi)
	addq	$32, %rdi
	cmpq	$8192, %rdi
	je	.L89
.L92:
	leaq	(%rdx,%rdi), %rcx
	movl	$0, %eax
	vmovapd	%ymm2, %ymm0
	jmp	.L90
.L89:
	addq	$8192, %r8
	addq	$8192, %rsi
	cmpq	%r9, %r8
	je	.L86
.L87:
	movl	$0, %edi
	jmp	.L92
.L86:
	ret
	.cfi_endproc
.LFE1047:
	.size	optimization_simd, .-optimization_simd
	.globl	optimization_cache_blocking
	.type	optimization_cache_blocking, @function
optimization_cache_blocking:
.LFB1048:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$8, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movq	%rdx, %rbx
	movl	$8388608, %edx
	movl	$0, %esi
	call	memset
	movl	$0, %edi
	jmp	.L94
.L99:
	movslq	%r9d, %rax
	leaq	(%r12,%rax,8), %r10
	vmovsd	(%r10), %xmm0
	cmpl	%esi, %r11d
	jge	.L95
	leal	(%r9,%r14), %ecx
	movl	%r11d, %eax
.L96:
	leal	(%rax,%rdi), %edx
	movslq	%edx, %rdx
	movslq	%ecx, %r8
	vmovsd	0(%rbp,%rdx,8), %xmm1
	vmulsd	(%rbx,%r8,8), %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	addl	$1, %eax
	addl	$1024, %ecx
	cmpl	%esi, %eax
	jl	.L96
.L95:
	vmovsd	%xmm0, (%r10)
	addl	$1, %r9d
	cmpl	%r13d, %r9d
	jne	.L99
	addl	$8, %r11d
	addl	$8192, %r14d
	cmpl	$1024, %r11d
	je	.L98
.L101:
	movl	%r13d, %r15d
	movl	%edi, %r9d
	leal	8(%r11), %esi
	jmp	.L99
.L98:
	cmpl	$1048576, %r15d
	je	.L93
	movl	%r15d, %edi
.L94:
	movl	%edi, %r14d
	negl	%r14d
	movl	$0, %r11d
	leal	1024(%rdi), %r13d
	jmp	.L101
.L93:
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1048:
	.size	optimization_cache_blocking, .-optimization_cache_blocking
	.globl	optimization_loop_unrolling
	.type	optimization_loop_unrolling, @function
optimization_loop_unrolling:
.LFB1049:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	movl	$0, %edi
	vxorpd	%xmm4, %xmm4, %xmm4
	jmp	.L104
.L107:
	leal	(%r8,%rdi), %r11d
	movslq	%r11d, %r11
	leaq	0(,%r11,8), %rcx
	movslq	%r9d, %r10
	leaq	0(,%r10,8), %rax
	vmovsd	(%rsi,%r11,8), %xmm1
	vmulsd	(%rdx,%r10,8), %xmm1, %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	8(%rsi,%rcx), %xmm1
	vmulsd	8192(%rdx,%rax), %xmm1, %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	16(%rsi,%rcx), %xmm1
	vmulsd	16384(%rdx,%rax), %xmm1, %xmm1
	vaddsd	%xmm0, %xmm1, %xmm1
	vmovsd	24(%rsi,%rcx), %xmm0
	vmulsd	24576(%rdx,%rax), %xmm0, %xmm0
	vaddsd	%xmm1, %xmm0, %xmm0
	vmovsd	32(%rsi,%rcx), %xmm3
	vmulsd	32768(%rdx,%rax), %xmm3, %xmm3
	vaddsd	%xmm0, %xmm3, %xmm3
	vmovsd	40(%rsi,%rcx), %xmm2
	vmulsd	40960(%rdx,%rax), %xmm2, %xmm2
	vaddsd	%xmm3, %xmm2, %xmm2
	vmovsd	48(%rsi,%rcx), %xmm1
	vmulsd	49152(%rdx,%rax), %xmm1, %xmm1
	vaddsd	%xmm2, %xmm1, %xmm1
	vmovsd	56(%rsi,%rcx), %xmm0
	vmulsd	57344(%rdx,%rax), %xmm0, %xmm0
	vaddsd	%xmm1, %xmm0, %xmm0
	addl	$8, %r8d
	addl	$8192, %r9d
	cmpl	$1024, %r8d
	jne	.L107
	leal	(%rdi,%rbp), %eax
	cltq
	vmovsd	%xmm0, (%r12,%rax,8)
	addl	$1, %ebx
	cmpl	$1024, %ebx
	je	.L106
.L109:
	movl	%ebx, %ebp
	movl	%ebx, %r9d
	movl	$0, %r8d
	vmovapd	%xmm4, %xmm0
	jmp	.L107
.L106:
	addl	$1024, %edi
	cmpl	$1048576, %edi
	je	.L103
.L104:
	movl	$0, %ebx
	jmp	.L109
.L103:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1049:
	.size	optimization_loop_unrolling, .-optimization_loop_unrolling
	.globl	optimization_register_blocking
	.type	optimization_register_blocking, @function
optimization_register_blocking:
.LFB1050:
	.cfi_startproc
	movl	$0, %r10d
	vxorpd	%xmm13, %xmm13, %xmm13
	vmovapd	%xmm13, %xmm12
	jmp	.L112
.L115:
	leal	(%r10,%rcx), %eax
	cltq
	vmovsd	(%rsi,%rax,8), %xmm0
	movl	%ecx, %r8d
	sall	$10, %r8d
	addl	%r9d, %r8d
	movslq	%r8d, %r8
	leaq	0(,%r8,8), %rax
	vmulsd	(%rdx,%r8,8), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm1, %xmm1
	vmulsd	8(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm2, %xmm2
	vmulsd	16(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm3, %xmm3
	vmulsd	24(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm4, %xmm4
	vmulsd	32(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm5, %xmm5
	vmulsd	40(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm6, %xmm6
	vmulsd	48(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm7, %xmm7
	vmulsd	56(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm8, %xmm8
	vmulsd	64(%rdx,%rax), %xmm0, %xmm11
	vaddsd	%xmm11, %xmm9, %xmm9
	vmulsd	72(%rdx,%rax), %xmm0, %xmm0
	vaddsd	%xmm0, %xmm10, %xmm10
	addq	$1, %rcx
	cmpq	$1024, %rcx
	jne	.L115
	leal	(%r10,%r9), %ecx
	movslq	%ecx, %rcx
	leaq	0(,%rcx,8), %rax
	vmovsd	%xmm1, (%rdi,%rcx,8)
	vmovsd	%xmm2, 8(%rdi,%rax)
	vmovsd	%xmm3, 16(%rdi,%rax)
	vmovsd	%xmm4, 24(%rdi,%rax)
	vmovsd	%xmm5, 32(%rdi,%rax)
	vmovsd	%xmm6, 40(%rdi,%rax)
	vmovsd	%xmm7, 48(%rdi,%rax)
	vmovsd	%xmm8, 56(%rdi,%rax)
	vmovsd	%xmm9, 64(%rdi,%rax)
	vmovsd	%xmm10, 72(%rdi,%rax)
	addl	$10, %r9d
	cmpl	$1020, %r9d
	je	.L114
.L118:
	movl	$0, %ecx
	vmovapd	%xmm13, %xmm10
	vmovapd	%xmm12, %xmm9
	vmovapd	%xmm12, %xmm8
	vmovapd	%xmm12, %xmm7
	vmovapd	%xmm12, %xmm6
	vmovapd	%xmm12, %xmm5
	vmovapd	%xmm12, %xmm4
	vmovapd	%xmm12, %xmm3
	vmovapd	%xmm12, %xmm2
	vmovapd	%xmm12, %xmm1
	jmp	.L115
.L114:
	addl	$1024, %r10d
	cmpl	$1048576, %r10d
	jne	.L112
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$0, %r10d
	vxorpd	%xmm2, %xmm2, %xmm2
	jmp	.L117
.L112:
	.cfi_def_cfa_offset 8
	.cfi_restore 3
	movl	$0, %r9d
	jmp	.L118
.L121:
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leal	(%rax,%r10), %r9d
	movslq	%r9d, %r9
	movslq	%ecx, %r8
	vmovsd	(%rsi,%r9,8), %xmm1
	vmulsd	(%rdx,%r8,8), %xmm1, %xmm1
	vaddsd	%xmm1, %xmm0, %xmm0
	addl	$1, %eax
	addl	$1024, %ecx
	cmpl	$1024, %eax
	jne	.L121
	leal	(%rbx,%r10), %eax
	cltq
	vmovsd	%xmm0, (%rdi,%rax,8)
	addl	$1, %r11d
	cmpl	$1024, %r11d
	je	.L120
.L123:
	movl	%r11d, %ebx
	movl	%r11d, %ecx
	movl	$0, %eax
	vmovapd	%xmm2, %xmm0
	jmp	.L121
.L120:
	addl	$1024, %r10d
	cmpl	$1048576, %r10d
	je	.L111
.L117:
	movl	$1020, %r11d
	jmp	.L123
.L111:
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1050:
	.size	optimization_register_blocking, .-optimization_register_blocking
	.globl	optimization_openmp_simd
	.type	optimization_openmp_simd, @function
optimization_openmp_simd:
.LFB1051:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, (%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, 16(%rsp)
	movl	$0, %edx
	movq	%rsp, %rsi
	movl	$optimization_openmp_simd._omp_fn.1, %edi
	call	GOMP_parallel_start
	movq	%rsp, %rdi
	call	optimization_openmp_simd._omp_fn.1
	call	GOMP_parallel_end
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1051:
	.size	optimization_openmp_simd, .-optimization_openmp_simd
	.globl	optimization_openmp_simd_cache_blocking
	.type	optimization_openmp_simd_cache_blocking, @function
optimization_openmp_simd_cache_blocking:
.LFB1052:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %rbx
	movq	%rsi, %r12
	movq	%rdx, %rbp
	movl	$8388608, %edx
	movl	$0, %esi
	call	memset
	movq	%rbx, (%rsp)
	movq	%r12, 8(%rsp)
	movq	%rbp, 16(%rsp)
	movl	$0, %edx
	movq	%rsp, %rsi
	movl	$optimization_openmp_simd_cache_blocking._omp_fn.2, %edi
	call	GOMP_parallel_start
	movq	%rsp, %rdi
	call	optimization_openmp_simd_cache_blocking._omp_fn.2
	call	GOMP_parallel_end
	addq	$32, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1052:
	.size	optimization_openmp_simd_cache_blocking, .-optimization_openmp_simd_cache_blocking
	.globl	optimization_openmp_simd_cache_blocking_loop_unrolling
	.type	optimization_openmp_simd_cache_blocking_loop_unrolling, @function
optimization_openmp_simd_cache_blocking_loop_unrolling:
.LFB1053:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %rbx
	movq	%rsi, %r12
	movq	%rdx, %rbp
	movl	$8388608, %edx
	movl	$0, %esi
	call	memset
	movq	%rbx, (%rsp)
	movq	%r12, 8(%rsp)
	movq	%rbp, 16(%rsp)
	movl	$0, %edx
	movq	%rsp, %rsi
	movl	$optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3, %edi
	call	GOMP_parallel_start
	movq	%rsp, %rdi
	call	optimization_openmp_simd_cache_blocking_loop_unrolling._omp_fn.3
	call	GOMP_parallel_end
	addq	$32, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1053:
	.size	optimization_openmp_simd_cache_blocking_loop_unrolling, .-optimization_openmp_simd_cache_blocking_loop_unrolling
	.globl	optimization_openmp_simd_cache_register_blocking_loop_unrolling
	.type	optimization_openmp_simd_cache_register_blocking_loop_unrolling, @function
optimization_openmp_simd_cache_register_blocking_loop_unrolling:
.LFB1054:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$32, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, %rbx
	movq	%rsi, %r12
	movq	%rdx, %rbp
	movl	$8388608, %edx
	movl	$0, %esi
	call	memset
	movq	%rbx, (%rsp)
	movq	%r12, 8(%rsp)
	movq	%rbp, 16(%rsp)
	movl	$0, %edx
	movq	%rsp, %rsi
	movl	$optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4, %edi
	call	GOMP_parallel_start
	movq	%rsp, %rdi
	call	optimization_openmp_simd_cache_register_blocking_loop_unrolling._omp_fn.4
	call	GOMP_parallel_end
	addq	$32, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1054:
	.size	optimization_openmp_simd_cache_register_blocking_loop_unrolling, .-optimization_openmp_simd_cache_register_blocking_loop_unrolling
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
