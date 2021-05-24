\\ASM
	hola equ "hola"
	chau equ "chau"
	ldh 3
	ldl hola
	mov dx , AC
	sys %4
	ldh 3
	ldl chau
	mov dx , AC
	sys %4
	stop
	