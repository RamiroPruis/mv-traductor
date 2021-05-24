\\ASM DATA=38500
	hola equ "hola"
	chau equ "chau"
	mov DX,hola
	sys %4
	mov dx,chau
	sys %4
	stop
	