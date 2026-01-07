	.file	"sanity_kernel.cpp"
	.option nopic
	.attribute arch, "rv32i2p1_m2p0_zmmul1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z3addii
	.type	_Z3addii, @function
_Z3addii:
.LFB0:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	add	a5,a4,a5
	mv	a0,a5
	lw	ra,28(sp)
	.cfi_restore 1
	lw	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE0:
	.size	_Z3addii, .-_Z3addii
	.align	2
	.globl	_Z8subtractii
	.type	_Z8subtractii, @function
_Z8subtractii:
.LFB1:
	.cfi_startproc
	addi	sp,sp,-32
	.cfi_def_cfa_offset 32
	sw	ra,28(sp)
	sw	s0,24(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,32
	.cfi_def_cfa 8, 0
	sw	a0,-20(s0)
	sw	a1,-24(s0)
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	sub	a5,a4,a5
	mv	a0,a5
	lw	ra,28(sp)
	.cfi_restore 1
	lw	s0,24(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 32
	addi	sp,sp,32
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE1:
	.size	_Z8subtractii, .-_Z8subtractii
	.align	2
	.globl	_Z8fsa_funcPiS_S_S_
	.type	_Z8fsa_funcPiS_S_S_, @function
_Z8fsa_funcPiS_S_S_:
.LFB2:
	.cfi_startproc
	addi	sp,sp,-48
	.cfi_def_cfa_offset 48
	sw	ra,44(sp)
	sw	s0,40(sp)
	.cfi_offset 1, -4
	.cfi_offset 8, -8
	addi	s0,sp,48
	.cfi_def_cfa 8, 0
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	a2,-44(s0)
	sw	a3,-48(s0)
	lw	a5,-36(s0)
	lw	a4,0(a5)
	lw	a5,-40(s0)
	lw	a5,0(a5)
	mv	a1,a5
	mv	a0,a4
	call	_Z8subtractii
	sw	a0,-20(s0)
	lw	a5,-44(s0)
	lw	a5,0(a5)
	mv	a1,a5
	lw	a0,-20(s0)
	call	_Z3addii
	sw	a0,-24(s0)
	lw	a5,-48(s0)
	lw	a4,-24(s0)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	mv	a0,a5
	lw	ra,44(sp)
	.cfi_restore 1
	lw	s0,40(sp)
	.cfi_restore 8
	.cfi_def_cfa 2, 48
	addi	sp,sp,48
	.cfi_def_cfa_offset 0
	jr	ra
	.cfi_endproc
.LFE2:
	.size	_Z8fsa_funcPiS_S_S_, .-_Z8fsa_funcPiS_S_S_
	.ident	"GCC: (g5115c7e447f) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
