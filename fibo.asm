        mov [10], 3000 		;inicializo variables
        mov [20], 1
otro: 	cmp [20], 100 		;compara
        jp  fin			;salta si llego a 100 o mas
        swap [10], [20]
        add 30,[20], [10]
        mov ax, %001
        mov cx, 1
        mov dx,10
        sys  			;print [10] en decimal
        jmp otroooooo
fin: 	stop