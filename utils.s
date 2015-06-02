	.file	"utils.c"
	.text
	.globl	compare_matrix
	.type	compare_matrix, @function
compare_matrix:
.LFB880:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2
.L6:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	vmovsd	(%rax), %xmm0
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	vmovsd	(%rax), %xmm1
	vsubsd	%xmm1, %xmm0, %xmm0
	vcvttsd2si	%xmm0, %eax
	cltd
	xorl	%edx, %eax
	subl	%edx, %eax
	vcvtsi2sd	%eax, %xmm0, %xmm0
	vucomisd	.LC0(%rip), %xmm0
	jbe	.L8
	movl	$1, %eax
	jmp	.L5
.L8:
	addl	$1, -4(%rbp)
.L2:
	cmpl	$1048575, -4(%rbp)
	jle	.L6
	movl	$0, %eax
.L5:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE880:
	.size	compare_matrix, .-compare_matrix
	.section	.rodata
	.align 8
.LC0:
	.long	2665960982
	.long	1020396463
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
