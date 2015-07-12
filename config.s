	.file	"config.c"
	.globl	h2
	.globl	w2h1
	.globl	w1
	.section	.sdata,"aw",@progbits
	.align	2
	.type	h2, @object
	.size	h2, 4
h2:
	.word	1024
	.type	w2h1, @object
	.size	w2h1, 4
w2h1:
	.word	1024
	.type	w1, @object
	.size	w1, 4
w1:
	.word	1024
	.ident	"GCC: (GNU) 5.1.0"
