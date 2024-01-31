.file "boot.s"
.option arch, rv32i

.section .boot
.align 4

.global _start
.type _start @function
_start: 									# kernel designed purely to run "main" then hold

.option push
.option norelax
	la 			gp, __global_pointer$
.option pop

	#la 			a0, __bss_start 	# clear bss section
	#li 			a1, 0
	#la 			a2, _end
	#sub 		a2, a2, a0
	#call memset

	la 			sp, _sstack 		# set stack pointer to stack start
	lw 			a0, 0(sp) 			# argc
	addi 		a1, sp, 4 			# argv
	li 			a2, 0 					# envp = NULL

	call 		main

	mv 			s1, a0 					# main stores retcode in s0

	auipc 	ra, 0
	ret

