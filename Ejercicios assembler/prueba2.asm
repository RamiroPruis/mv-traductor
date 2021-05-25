\\ASM
hola EQU "hola"
chau EQU "chau"

MAIN:       RND 10
            MOV [10], AC
            MOV AX, %800
            OR AX, %001
            MOV DX, 10
            MOV CX, 1
            SYS 2

            XOR [10],[10]
            SYS 2

            LDH 3
            LDL hola
            MOV FX, AC
            SLEN [10], FX
            MOV DX, 10
            MOV CX, 1
            SYS 2

            LDH 3
            LDL chau
            MOV EX, AC
            SCMP EX, FX
            MOV BX, 20
            MOV [BX], AC
            MOV DX, BX
            SYS 2

            STOP


