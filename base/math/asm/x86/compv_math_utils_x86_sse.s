;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>   ;
; File author: Mamadou DIOP (Doubango Telecom, France).                 ;
; License: GPLv3. For commercial license please contact us.             ;
; Source code: https://github.com/DoubangoTelecom/compv                 ;
; WebSite: http://compv.org                                             ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "compv_common_x86.s"

COMPV_YASM_DEFAULT_REL

global sym(CompVMathUtilsMax_16u_Asm_X86_SSE41)
global sym(CompVMathUtilsSumAbs_16s16u_Asm_X86_SSSE3)
global sym(CompVMathUtilsSum_8u32u_Asm_X86_SSSE3)
global sym(CompVMathUtilsSum2_32s32s_Asm_X86_SSE2)

section .data

section .text

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; arg(0) -> COMPV_ALIGNED(SSE) const uint16_t* data
; arg(1) -> compv_uscalar_t width
; arg(2) -> compv_uscalar_t height
; arg(3) -> COMPV_ALIGNED(SSE) compv_uscalar_t stride
; arg(4) -> uint16_t *max
sym(CompVMathUtilsMax_16u_Asm_X86_SSE41):
	push rbp
	mov rbp, rsp
	COMPV_YASM_SHADOW_ARGS_TO_STACK 5
	push rsi
	push rdi
	push rbx
	;; end prolog ;;

	; align stack and alloc memory
	COMPV_YASM_ALIGN_STACK 16, rax
	sub rsp, (4*COMPV_YASM_UINT32_SZ_BYTES)

	%define data				rbx
	%define width				rdx
	%define height				rsi
	%define i					rcx
	%define widthMinus7			rdi
	%define widthMinus31		rax
	%define vecMax				xmm4
	%define vecOrphansSuppress	xmm5
	
	mov rax, arg(1) ; width
	and rax, 7
	jz .OrphansIsZero
		pcmpeqw xmm0, xmm0
		movdqa [rsp + 0], xmm0
		shl rax, 4
		mov rcx, 8*16
		sub rcx, rax
		xor rdi, rdi
		%assign index 0
		%rep 4
			test rcx, rcx
			js .OrphansCopied
			mov rax, 0xffffffff
			shr rax, cl
			cmp rcx, 31
			cmovg rax, rdi ; rdi = 0
			mov [rsp + (3-index)*COMPV_YASM_UINT32_SZ_BYTES], dword eax
			sub rcx, 32
			%assign index index+1
		%endrep
		.OrphansCopied:
			movdqa vecOrphansSuppress, [rsp + 0]
		.OrphansIsZero:

	; convert stride from shorts to bytes
	mov rax, arg(3) ; stride
	lea rax, [rax * COMPV_YASM_INT16_SZ_BYTES]
	mov arg(3), rax

	mov data, arg(0)
	mov width, arg(1)
	mov height, arg(2)
	lea widthMinus31, [width - 31]
	lea widthMinus7, [width - 7]
	
	pxor vecMax, vecMax

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; for (compv_uscalar_t j = 0; j < height; ++j)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.LoopHeight:
		xor i, i
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (i = 0; i < widthSigned - 31; i += 32)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		test widthMinus31, widthMinus31
		js .EndOf_LoopWidth32
		.LoopWidth32:
			movdqa xmm0, [data + (i + 0)*COMPV_YASM_UINT16_SZ_BYTES]
			movdqa xmm1, [data + (i + 8)*COMPV_YASM_UINT16_SZ_BYTES]
			movdqa xmm2, [data + (i + 16)*COMPV_YASM_UINT16_SZ_BYTES]
			movdqa xmm3, [data + (i + 24)*COMPV_YASM_UINT16_SZ_BYTES]
			add i, 32
			pmaxuw xmm0, xmm1
			pmaxuw xmm2, xmm3
			cmp i, widthMinus31
			pmaxuw vecMax, xmm0
			pmaxuw vecMax, xmm2
			jl .LoopWidth32
			.EndOf_LoopWidth32
			;; EndOf_LoopWidth32 ;;

		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (; i < widthSigned - 7; i += 8)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		cmp i, widthMinus7
		jge .EndOf_LoopWidth7
		.LoopWidth7:
			pmaxuw vecMax, [data + (i + 0)*COMPV_YASM_UINT16_SZ_BYTES]
			add i, 8
			cmp i, widthMinus7
			jl .LoopWidth7
			.EndOf_LoopWidth7
			;; EndOf_LoopWidth7 ;;

		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; if (orphans)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		test width, 7
		jz .NoOrphans
			movdqa xmm0, [data + (i + 0)*COMPV_YASM_UINT16_SZ_BYTES]
			pand xmm0, vecOrphansSuppress
			pmaxuw vecMax, xmm0
			.NoOrphans

		add data, arg(3)
		dec height
		jnz .LoopHeight
		;; EndOf_LoopHeight ;;

	movdqa xmm0, vecMax
	movdqa xmm1, vecMax
	movdqa xmm2, vecMax
	psrldq xmm0, 8
	psrldq xmm1, 4
	psrldq xmm2, 2
	pmaxuw xmm0, xmm1
	pmaxuw vecMax, xmm2
	pmaxuw vecMax, xmm0

	pextrw eax, vecMax, 0
	mov rdx, arg(4)
	mov [rdx], word ax

	; free memory and unalign stack
	add rsp, (4*COMPV_YASM_UINT32_SZ_BYTES)
	COMPV_YASM_UNALIGN_STACK

	%undef data
	%undef width
	%undef height
	%undef i
	%undef widthMinus7
	%undef widthMinus31
	%undef vecMax
	%undef vecOrphansSuppress

	;; begin epilog ;;
	pop rbx
	pop rdi
	pop rsi
	COMPV_YASM_UNSHADOW_ARGS
	mov rsp, rbp
	pop rbp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; arg(0) -> const COMPV_ALIGNED(SSE) int16_t* a
; arg(1) -> const COMPV_ALIGNED(SSE) int16_t* b
; arg(2) -> COMPV_ALIGNED(SSE) uint16_t* r
; arg(3) -> compv_uscalar_t width
; arg(4) -> compv_uscalar_t height
; arg(5) -> COMPV_ALIGNED(SSE) compv_uscalar_t stride
sym(CompVMathUtilsSumAbs_16s16u_Asm_X86_SSSE3):
	push rbp
	mov rbp, rsp
	COMPV_YASM_SHADOW_ARGS_TO_STACK 6
	COMPV_YASM_SAVE_XMM 7
	push rsi
	push rdi
	push rbx
	;; end prolog ;;

	mov rcx, arg(5)
	mov rax, arg(0) ; rax = a
	mov rdx, arg(1) ; rdx = b
	lea rcx, [rcx * COMPV_YASM_INT16_SZ_BYTES]
	mov rbx, arg(2) ; rbx = r
	mov rdi, arg(3) 
	lea rdi, [rdi - 31] ; rdi = width - 31
	mov rsi, arg(4) ; rsi = height
	mov arg(5), rcx ; strideInBytes

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; for (j = 0; j < height; ++j)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.LoopRows
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (i = 0; i < width_ - 31; i += 32)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		xor rcx, rcx ; rcx = i = 0
		test rdi, rdi
		js .EndOfLoopCols32
		.LoopCols32:
			pabsw xmm0, [rax + (rcx + 0)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm1, [rax + (rcx + 8)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm2, [rax + (rcx + 16)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm3, [rax + (rcx + 24)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm4, [rdx + (rcx + 0)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm5, [rdx + (rcx + 8)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm6, [rdx + (rcx + 16)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm7, [rdx + (rcx + 24)*COMPV_YASM_INT16_SZ_BYTES]
			add rcx, 32
			paddusw xmm0, xmm4
			paddusw xmm1, xmm5
			paddusw xmm2, xmm6
			paddusw xmm3, xmm7
			cmp rcx, rdi
			movdqa [rbx + (rcx - 32)*COMPV_YASM_INT16_SZ_BYTES], xmm0
			movdqa [rbx + (rcx - 24)*COMPV_YASM_INT16_SZ_BYTES], xmm1
			movdqa [rbx + (rcx - 16)*COMPV_YASM_INT16_SZ_BYTES], xmm2
			movdqa [rbx + (rcx - 8)*COMPV_YASM_INT16_SZ_BYTES], xmm3
			jl .LoopCols32
		.EndOfLoopCols32:

		add rdi, 31

		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (i = 0; i < width_; i += 8)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		cmp rcx, rdi
		jge .EndOfLoopCols8
		.LoopCols8:
			pabsw xmm0, [rax + (rcx + 0)*COMPV_YASM_INT16_SZ_BYTES]
			pabsw xmm1, [rdx + (rcx + 0)*COMPV_YASM_INT16_SZ_BYTES]
			add rcx, 8
			paddusw xmm0, xmm1
			cmp rcx, rdi
			movdqa [rbx + (rcx - 8)*COMPV_YASM_INT16_SZ_BYTES], xmm0
			jl .LoopCols8
		.EndOfLoopCols8:

		sub rdi, 31

		add rax, arg(5)
		add rdx, arg(5)
		add rbx, arg(5)
		dec rsi
		jnz .LoopRows

	;; begin epilog ;;
	pop rbx
	pop rdi
	pop rsi
	COMPV_YASM_RESTORE_XMM
	COMPV_YASM_UNSHADOW_ARGS
	mov rsp, rbp
	pop rbp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; arg(0) -> COMPV_ALIGNED(SSE) const uint8_t* data
; arg(1) -> compv_uscalar_t count
; arg(2) -> uint8_t *mean1
sym(CompVMathUtilsSum_8u32u_Asm_X86_SSSE3):
	push rbp
	mov rbp, rsp
	COMPV_YASM_SHADOW_ARGS_TO_STACK 3
	push rsi
	push rdi
	push rbx
	;; end prolog ;;
	
	pxor xmm0, xmm0 ; xmm0 = xmmZer0
	pxor xmm3, xmm3 ; xmm3 = xmmSum
	mov rsi, arg(0) ; rsi = data
	mov rdx, arg(1)
	; rcx = i
	lea rbx, [rdx - 15] ; rbx = count - 15
	lea rax, [rdx - 7] ; rax = count - 7
	lea rdi, [rdx - 3] ; rdi = count - 3
	dec rdx ; rdx = count - 1

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; for (i = 0; i < count_ - 15; i += 16)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	xor rcx, rcx
	.LoopRows 
		movdqa xmm1, [rsi + rcx]
		lea rcx, [rcx + 16]
		movdqa xmm2, xmm1
		punpcklbw xmm1, xmm0
		punpckhbw xmm2, xmm0
		paddw xmm1, xmm2
		movdqa xmm2, xmm1
		cmp rcx, rbx
		punpcklwd xmm1, xmm0
		punpckhwd xmm2, xmm0
		paddd xmm1, xmm2
		paddd xmm3, xmm1		
		jl .LoopRows

	;;;;;;;;;;;;;;;;;;;;;;;;;;
	; if (i < count_ - 7)
	;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp rcx, rax
	jge .EndOfMoreThanSevenRemains
		movq xmm1, [rsi + rcx]
		lea rcx, [rcx + 8]
		punpcklbw xmm1, xmm0
		movdqa xmm2, xmm1
		punpcklwd xmm1, xmm0
		punpckhwd xmm2, xmm0
		paddd xmm1, xmm2
		paddd xmm3, xmm1
	.EndOfMoreThanSevenRemains

	;;;;;;;;;;;;;;;;;;;;;;;;;;
	; if (i < count_ - 3)
	;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp rcx, rdi
	jge .EndOfMoreThanThreeRemains
		movd xmm1, [rsi + rcx]
		lea rcx, [rcx + 4]
		punpcklbw xmm1, xmm0
		punpcklwd xmm1, xmm0
		paddd xmm3, xmm1
	.EndOfMoreThanThreeRemains

	mov rdi, arg(1) ; rdi = count

	phaddd xmm3, xmm3
	phaddd xmm3, xmm3
	movd rbx, xmm3

	;;;;;;;;;;;;;;;;;;;;;;;;
	; if (i < count_ - 1)
	;;;;;;;;;;;;;;;;;;;;;;;;
	cmp rcx, rdx
	jge .EndOfMoreThanOneRemains
		movzx rdx, byte [rsi + rcx]
		movzx rax, byte [rsi + rcx + 1]
		lea rcx, [rcx + 2]
		add rax, rdx
		add rbx, rax
	.EndOfMoreThanOneRemains

	;;;;;;;;;;;;;;;;;;;;
	; if (count_ & 1)
	;;;;;;;;;;;;;;;;;;;;
	test rdi, 1
	jz .EndOfMoreThanZeroRemains
		movzx rdx, byte [rsi + rcx]
		add rbx, rdx
	.EndOfMoreThanZeroRemains

	mov rax, arg(2)
	mov [rax], dword ebx

	;; begin epilog ;;
	pop rbx
	pop rdi
	pop rsi
	COMPV_YASM_UNSHADOW_ARGS
	mov rsp, rbp
	pop rbp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; arg(0) -> COMPV_ALIGNED(SSE) const int32_t* a
; arg(1) -> COMPV_ALIGNED(SSE) const int32_t* b
; arg(2) -> COMPV_ALIGNED(SSE) int32_t* s
; arg(3) -> compv_uscalar_t width
; arg(4) -> compv_uscalar_t height
; arg(5) -> COMPV_ALIGNED(SSE) compv_uscalar_t stride
sym(CompVMathUtilsSum2_32s32s_Asm_X86_SSE2):
	push rbp
	mov rbp, rsp
	COMPV_YASM_SHADOW_ARGS_TO_STACK 6
	push rsi
	push rdi
	push rbx
	;; end prolog ;;

	; alloc memory
	sub rsp, 8
	; [rsp + 0] = strideInBytes

	mov rax, arg(5)
	shl rax, 2
	mov [rsp + 0], rax

	mov rsi, arg(0) ; rsi = a
	mov rdi, arg(1) ; rdi = b
	mov rbx, arg(2) ; rbx = s
	mov rdx, arg(4) ; rdx = height
	mov rax, arg(3) ; rax = width

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; for (j = 0; j < height; ++j)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.LoopRows
		lea rax, [rax - 15] ; rax = width - 15
		xor rcx, rcx ; rcx = i = 0
		cmp rcx, rax
		jge .EndOfLoopCols16
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (i = 0; i < width_ - 15; i += 16) 
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		.LoopCols16
			lea rcx, [rcx + 16]
			movdqa xmm0, [rsi + rcx*4 - 64]
			movdqa xmm1, [rsi + rcx*4 - 48]
			movdqa xmm2, [rsi + rcx*4 - 32]
			movdqa xmm3, [rsi + rcx*4 - 16]
			paddd xmm0, [rdi + rcx*4 - 64]
			paddd xmm1, [rdi + rcx*4 - 48]
			cmp rcx, rax
			paddd xmm2, [rdi + rcx*4 - 32]
			paddd xmm3, [rdi + rcx*4 - 16]
			movdqa [rbx + rcx*4 - 64], xmm0
			movdqa [rbx + rcx*4 - 48], xmm1
			movdqa [rbx + rcx*4 - 32], xmm2
			movdqa [rbx + rcx*4 - 16], xmm3			
			jl .LoopCols16
		.EndOfLoopCols16

		lea rax, [rax + 15] ; rax = width
		cmp rcx, rax
		jge .EndOfLoopCols4
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		; for (; i < width_; i += 4)
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		.LoopCols4
			lea rcx, [rcx + 4]
			movdqa xmm0, [rsi + rcx*4 - 16]
			cmp rcx, rax
			paddd xmm0, [rdi + rcx*4 - 16]
			movdqa [rbx + rcx*4 - 16], xmm0			
			jl .LoopCols4
		.EndOfLoopCols4
		
		add rsi, [rsp + 0]
		add rdi, [rsp + 0]
		add rbx, [rsp + 0]

		dec rdx
		jnz .LoopRows

	; free memory
	add rsp, 8

	;; begin epilog ;;
	pop rbx
	pop rdi
	pop rsi
	COMPV_YASM_UNSHADOW_ARGS
	mov rsp, rbp
	pop rbp
	ret