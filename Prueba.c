#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define CANT 32

//.h
typedef struct
{
    char cadena[256];
} TvecCadenas;

typedef struct
{
    char mnemo[10];
    int hex;
} Tvec;

typedef struct
{
    char nombre[10];
    int linea;
    int String; //=0 si no es Const String, sino el largo del string
    char str[200];
} Rotulo;

typedef struct
{
    Rotulo rot[100];
    int tope;

} TvecRotulo;

typedef struct
{
    int cod;  //codigo instruccion
    int topA; //tipo operando A
    int topB; //tipo operando B
    int vopA; //valor operando A
    int vopB; //valor operando B
} instruccion;

typedef struct
{
    char cod[MAX];
    char mnemom[MAX];
    char op1[MAX];
    char op2[MAX];
    char comentario[200];
} lineacod;

void creadicc(Tvec[]);
int encuentramnemo(char[], Tvec[], int);
void tipoOperando(char[], int *, int *, int, TvecRotulo, int, int *);
int traduceInstruccion(instruccion);
void agregaRotulo(TvecRotulo *, char[], int, int, char[]);
void cargaRotulos(TvecCadenas[], int, TvecRotulo *);
void Desarma(char[], instruccion *, lineacod *, Tvec[], TvecRotulo *, int, int *, int *);
int buscaRotulo(char[], TvecRotulo);
void comeBasura(char[], int *);
void elimEspacio(char[]);
void trunca(int *, int);
void IniciaCadena(lineacod *);
void seteaHeader(char[], int *, int *, int *);

//.h

//COMIENZA  MAIN
int main(int argc, char *argv[])
{
    Tvec Mnemonicos[CANT];
    int vacia = 0;
    // Variables para :Lectura archivos
    FILE *arch;
    int flag = 0;
    int topeLineas = 0;
    int vectorbinario[2000];
    TvecCadenas vecLineas[2000];
    lineacod LineaCodigo;
    int i = 0, j;
    int traduce, creaBin = 1;
    int n, k = 0;
    int tamDS, tamSS, tamES;
    int kString;
    char header[256];

    tamDS = tamSS = tamES = 1024; //Por defecto cada segment ocupa 1024

    instruccion num;
    TvecRotulo rotulos;

    //Inicializaciones
    rotulos.tope = -1;
    creadicc(Mnemonicos);

    // if (argc > 4)
    // {
    //     printf("Error. Demasiados argumentos");
    //     exit(1);
    // }
    // else
    // {
    //     if (argc < 3)
    //     {
    //         printf("Error. Pocos argumentos \n");
    //         printf("Ejemplo: mvc.exe AsmFilename.asm BinFilename.bin [-o]");
    //         exit(1);
    //     }
    //     else
    //     {
    //         if (argc == 4 && strcmp(argv[3], "-o") == 0)
    //             flag = 1; //Omite la salida por pantalla de la traduccion.
    //     }
    // }
    // COMIENZA Lectura del archivo .asm
    if ((arch = fopen("Ejercicios assembler\\prueba2.asm", "r")) == NULL)
        return 1;

    //LEE HEADER
    fgets(header, 256, arch);
    seteaHeader(header, &tamDS, &tamES, &tamSS);
    //LEE HEADER

    while (fgets(vecLineas[topeLineas].cadena, 256, arch) != NULL)
    {
        j = 0;
        while (vecLineas[topeLineas].cadena[j] != 13 && vecLineas[topeLineas].cadena[j] != '\n')
            j++;
        if (j != 0)
            vecLineas[topeLineas].cadena[j] = '\0';
        if (vecLineas[topeLineas].cadena[0] != '\n' && vecLineas[topeLineas].cadena[0] != '\0')
            topeLineas++;
        fflush(stdin);
    }

    topeLineas--;
    //   for (int i = 0; i < topeLineas; i++)
    //       vecLineas[i].cadena[strlen(vecLineas[i].cadena) - 1] = '\0';
    fclose(arch);
    // FIN Lectura del archivo .asm
    //carga de Rotulos y constantes
    cargaRotulos(vecLineas, topeLineas, &rotulos);

    //Ciclo desarmado

    int topeBinario = topeLineas;
    do
    {
        vacia = 0;
        IniciaCadena(&LineaCodigo);
        Desarma(vecLineas[i].cadena, &num, &LineaCodigo, Mnemonicos, &rotulos, k, &traduce, &vacia);

        if (traduce)
        {
            n = traduceInstruccion(num);
            vectorbinario[k] = n;
        }
        else
        {
            if (!vacia)
            {
                n = -1; //FF FF FF FF
                creaBin = 0;
            }
            else
            {
                k--;
                topeBinario--;
                printf("\t%s \n", LineaCodigo.comentario);
            }
        }
        if (flag == 0 && !vacia)
        {
            printf("[%04d]: %02X %02X %02X %02X", k, (n >> 24) & 0xFF, (n >> 16) & 0xFF, (n >> 8) & 0xFF, (n >> 0) & 0xFF);
            printf("%10s %4s %s %s \t%s \n", LineaCodigo.cod, strupr(LineaCodigo.mnemom), strupr(LineaCodigo.op1), strupr(LineaCodigo.op2), LineaCodigo.comentario);
        }
        i++;
        k++;
    } while (i <= topeLineas);

    //cargo constantes String
    kString = k;
    for (int z = 0; z <= rotulos.tope; z++)
    {
        if (rotulos.rot[z].String)
        {
            rotulos.rot[z].linea = kString;
            kString += rotulos.rot[z].String;
        }
    }

    if (creaBin)
    {
        i = 0;
        char FIJO[5] = "MV21";

        if ((arch = fopen("Ejercicios assembler\\prueba2.bin", "wb")) == NULL)
        {
            return -1;
        }
        // Se rompe

        //HEADER
        fwrite(&FIJO, sizeof(char), strlen(FIJO), arch); //escribe "MVC 21"
        fwrite(&tamDS, sizeof(int), 1, arch);
        fwrite(&tamSS, sizeof(int), 1, arch);
        fwrite(&tamES, sizeof(int), 1, arch);
        fwrite(&kString, sizeof(int), 1, arch);
        //HEADER

        //ARRANCA A ESCRIBIR EL CODIGO TRADUCIDO
        while (i <= topeBinario)
        {
            fwrite(&(vectorbinario[i]), sizeof(int), 1, arch);
            i++;
        }
        int letra;
        for (int z = 0; z <= rotulos.tope; z++)
            if (rotulos.rot[z].String)
                for (j = 0; j < rotulos.rot[z].String; j++)
                {
                    letra = rotulos.rot[z].str[j];
                    fwrite(&letra, sizeof(int), 1, arch);
                }
        fclose(arch);
        printf("Archivo binario creado con exito. Traduccion exitosa");
    }
    else
        printf("No se creo el binario");
    return 0;
}

//FIN MAIN

void creadicc(Tvec vec[])
{
    strcpy(vec[0].mnemo, "MOV");
    vec[0].hex = 0x00;
    strcpy(vec[1].mnemo, "ADD");
    vec[1].hex = 0x01;
    strcpy(vec[2].mnemo, "SUB");
    vec[2].hex = 0x02;
    strcpy(vec[3].mnemo, "SWAP");
    vec[3].hex = 0x03;
    strcpy(vec[4].mnemo, "MUL");
    vec[4].hex = 0x04;
    strcpy(vec[5].mnemo, "DIV");
    vec[5].hex = 0x05;
    strcpy(vec[6].mnemo, "CMP");
    vec[6].hex = 0x06;
    strcpy(vec[7].mnemo, "SHL");
    vec[7].hex = 0x07;
    strcpy(vec[8].mnemo, "SHR");
    vec[8].hex = 0x08;
    strcpy(vec[9].mnemo, "AND");
    vec[9].hex = 0x09;
    strcpy(vec[10].mnemo, "OR");
    vec[10].hex = 0x0A;
    strcpy(vec[11].mnemo, "XOR");
    vec[11].hex = 0x0B;
    strcpy(vec[12].mnemo, "SYS");
    vec[12].hex = 0xF0;
    strcpy(vec[13].mnemo, "JMP");
    vec[13].hex = 0xF1;
    strcpy(vec[14].mnemo, "JZ");
    vec[14].hex = 0xF2;
    strcpy(vec[15].mnemo, "JP");
    vec[15].hex = 0xF3;
    strcpy(vec[16].mnemo, "JN");
    vec[16].hex = 0xF4;
    strcpy(vec[17].mnemo, "JNZ");
    vec[17].hex = 0xF5;
    strcpy(vec[18].mnemo, "JNP");
    vec[18].hex = 0xF6;
    strcpy(vec[19].mnemo, "JNN");
    vec[19].hex = 0xF7;
    strcpy(vec[20].mnemo, "LDL");
    vec[20].hex = 0xF8;
    strcpy(vec[21].mnemo, "LDH");
    vec[21].hex = 0xF9;
    strcpy(vec[22].mnemo, "RND");
    vec[22].hex = 0xFA;
    strcpy(vec[23].mnemo, "NOT");
    vec[23].hex = 0xFB;
    strcpy(vec[24].mnemo, "STOP");
    vec[24].hex = 0xFF1;
    strcpy(vec[25].mnemo, "SLEN");
    vec[25].hex = 0x0C;
    strcpy(vec[26].mnemo, "SMOV");
    vec[26].hex = 0x0D;
    strcpy(vec[27].mnemo, "SCMP");
    vec[27].hex = 0x0E;
    strcpy(vec[28].mnemo, "PUSH");
    vec[28].hex = 0xFC;
    strcpy(vec[29].mnemo, "POP");
    vec[29].hex = 0xFD;
    strcpy(vec[30].mnemo, "CALL");
    vec[30].hex = 0xFE;
    strcpy(vec[31].mnemo, "RET");
    vec[31].hex = 0xFF0;
}

void creaReg(Tvec registros[])
{
    //Inicializamos en null
    for (int i = 0; i < 16; i++)
        strcpy(registros[i].mnemo, "\0");

    strcpy(registros[0].mnemo, "DS");
    registros[0].hex = 0;
    strcpy(registros[1].mnemo, "SS");
    registros[1].hex = 1;
    strcpy(registros[2].mnemo, "ES");
    registros[2].hex = 2;
    strcpy(registros[3].mnemo, "CS");
    registros[3].hex = 3;
    strcpy(registros[4].mnemo, "HP");
    registros[4].hex = 4;
    strcpy(registros[5].mnemo, "IP");
    registros[5].hex = 5;
    strcpy(registros[6].mnemo, "SP");
    registros[6].hex = 6;
    strcpy(registros[7].mnemo, "BP");
    registros[7].hex = 7;
    strcpy(registros[8].mnemo, "CC");
    registros[8].hex = 8;
    strcpy(registros[9].mnemo, "AC");
    registros[9].hex = 9;
    strcpy(registros[10].mnemo, "AX");
    registros[10].hex = 10;
    strcpy(registros[11].mnemo, "BX");
    registros[11].hex = 11;
    strcpy(registros[12].mnemo, "CX");
    registros[12].hex = 12;
    strcpy(registros[13].mnemo, "DX");
    registros[13].hex = 13;
    strcpy(registros[14].mnemo, "EX");
    registros[14].hex = 14;
    strcpy(registros[15].mnemo, "FX");
    registros[15].hex = 15;
}

int encuentramnemo(char mnem[], Tvec vec[], int max)
{
    int i = 0;
    while (i <= max && strcmpi(mnem, vec[i].mnemo) != 0)
        i++;
    if (i <= max && mnem[0] != '\0')
        return i;
    else
        return -1;
}

//tipo tiene que entrar con un valor
void tipoOperando(char entrada[], int *tipo, int *operando, int bitsoperando, TvecRotulo rotulos, int nroLinea, int *traduce)
{
    int i = 0, j = 0, pos, pos2;
    int offset = 0;
    char base = '\0';
    char num[15] = {'\0'};
    int suma = 0;
    Tvec reg[16];

    creaReg(reg);
    *tipo = 0;

    if (entrada[i] == '[')
    { //Operando indirecto
        i++;
        if (entrada[i] >= 'A' && entrada[i] <= 'z')
            *tipo = 3; //Operando indirecto con contenido indirecto
        else
            *tipo = 2; //Operando indirecto con contenido inmediato
        do
        {
            num[j] = entrada[i];
            i++;
            j++;
        } while (entrada[i] != ']');
        num[j] = '\0';
    }
    else
        strcpy(num, entrada);
    if (*tipo == 3)
    {
        j = 0;
        int k = 0;
        char regAct[3], offsetcad[20];

        while (num[j] && num[j] != '+' && num[j] != '-')
        {
            regAct[j] = num[j];
            j++;
        }
        suma = (num[j] == '+');
        regAct[j] = '\0';
        j++;
        pos = encuentramnemo(regAct, reg, 16);
        //caso con offset
        if (num[j] != '\0')
        {
            while (num[j])
            {
                offsetcad[k] = num[j];
                k++;
                j++;
            }
            offsetcad[k] = '\0';
            //tiene que identificar si es una constante o un numero
            //constante
            if (offsetcad[0] >= 'A' && offsetcad[0] <= 'z')
            {
                pos2 = buscaRotulo(offsetcad, rotulos);
                if (pos2 == -1)
                {
                    printf("ERROR:\tSimbolo inexistente : %s\n", offsetcad);
                    *traduce = 0;
                }
                else
                    offset = rotulos.rot[pos2].linea;
                //numero
            }
            else
                offset = strtol(offsetcad, NULL, 10);
            if (!suma)
                offset = ~offset + 1;
        }
        //caso de offset negativo
        *operando = pos;
        offset = offset << 4;
        *operando &= 0x00F;
        *operando |= offset;
        return; //hago esto para salir de la funcion rapido sin cambiar las condiciones de los if para agregar el nuevo tipo
    }
    pos = (*tipo != 2) ? encuentramnemo(num, reg, 16) : -1;
    if (pos != -1)
    { //Operando registro
        *tipo = 1;
        *operando = reg[pos].hex;
    }
    else
    {
        if ((num[0] >= 'A' && num[0] <= 'Z') || (num[0] >= 'a' && num[0] <= 'z'))
        { //Posible rotulo
            pos = buscaRotulo(num, rotulos);
            if (pos != -1)
            { //encuentra rotulo
                *tipo = 0;
                *operando = rotulos.rot[pos].linea; //Carga la linea del rotulo, no se bien como esta implementada la estructura.
            }
            else
            {
                printf("ERROR ln.%d:No se encuentra el rotulo\n", nroLinea);
                *tipo = 0;
                *operando = 0xFFF;
                *traduce = 0;
            }
        }
        else
        {
            //Operando inmediato
            if (*tipo != 2)
                *tipo = 0;
            if (num[0] == '#' || num[0] == '@' || num[0] == '%' || num[0] == '\'')
            {
                base = num[0];
                j = 0;
                while (num[j] != '\0')
                {
                    num[j] = num[j + 1];
                    j++;
                }
            }
            switch (base)
            {
            case '#':
                *operando = strtol(num, NULL, 10);
                break;
            case '@':
                *operando = strtol(num, NULL, 8);
                break;
            case '%':
                *operando = strtol(num, NULL, 16);
                break;
            case '\'':
                *operando = num[0];
                break;
            default:
                if (num[0] >= 48 && num[0] <= 57) //Si es un numero
                    *operando = strtol(num, NULL, 10);
                else
                    *tipo = -1;
                break;
            }
            trunca(&(*operando), bitsoperando);
        }
    }
}

//Generamos la cadena de 32 bits de binario
int traduceInstruccion(instruccion inst)
{
    int resultado = 0;
    //Instruccion de dos operandos
    if (inst.cod >= 0 && inst.cod <= 0xE)
        resultado = ((inst.cod << 28) & 0xF0000000) | ((inst.topA << 26) & 0x0C000000) | ((inst.topB << 24) & 0x03000000) | ((inst.vopA << 12) & 0x00FFF000) | (inst.vopB & 0x00000FFF);
    //Instruccion de un operando
    else if (inst.cod >= 0xF0 && inst.cod <= 0xFE)
        resultado = ((inst.cod << 24) & 0xFF000000) | ((inst.topA << 22) & 0x00C00000) | (inst.vopA & 0x0000FFFF);
    //Instrucciones sin operando
    else
        resultado = (inst.cod << 20) & 0xFFF00000;
    return resultado;
}

void cargaRotulos(TvecCadenas vec[], int n, TvecRotulo *rotulos)
{
    char cod[MAX];
    char equ[MAX];

    char cadenaActual[256];
    int valorConst;
    char valorConstCAD[10];
    char base = '\0';
    int j, k, l = 0;
    int largoString = 0;
    char str[200] = {"\0"};
    //Recorremos las lineas en busqueda de rotulos
    for (int i = 0; i <= n; i++)
    {
        largoString = 0;
        //Leemos hasta que el siguiente sea ':'
        strcpy(cadenaActual, vec[i].cadena);
        j = 0;
        k = 0;
        comeBasura(cadenaActual, &j);
        if (cadenaActual[j] != '\0' && cadenaActual[j] != ';')
        {
            while (cadenaActual[j] != ' ' && cadenaActual[j] != ':' && cadenaActual[j] != '\t' && cadenaActual[j] != '\0')
            {
                cod[k] = cadenaActual[j];
                j++;
                k++;
            }
            cod[k] = '\0';
            //Caso rotulo
            k = 0;
            if (cadenaActual[j] == ':')
            {
                //Vemos si el rotulo no existe
                if (buscaRotulo(cod, *rotulos) == -1)
                {
                    (*rotulos).tope++;
                    agregaRotulo(rotulos, cod, l, largoString, str);
                }
            }
            //posible constante
            else
            {
                comeBasura(cadenaActual, &j);
                while (cadenaActual[j] != ' ' && cadenaActual[j] != '\0')
                {
                    equ[k] = cadenaActual[j];
                    k++;
                    j++;
                }
                equ[k] = '\0';
                k = 0;
                //encuentra constante
                if (strcmpi(equ, "EQU") == 0)
                {
                    comeBasura(cadenaActual, &j);
                    //carga el valor con su respectiva base en valorConst
                    while (cadenaActual[j])
                    {
                        valorConstCAD[k] = cadenaActual[j];
                        k++;
                        j++;
                    }
                    valorConstCAD[k] = '\0';
                    k = 0;
                    if (valorConstCAD[0] != '"')
                    {
                        if (valorConstCAD[0] == '#' || valorConstCAD[0] == '@' || valorConstCAD[0] == '%' || valorConstCAD[0] == '\'')
                        {
                            base = valorConstCAD[0];
                            while (valorConstCAD[k] != '\0')
                            {
                                valorConstCAD[k] = valorConstCAD[k + 1];
                                k++;
                            }
                        }
                        switch (base)
                        {
                        case '#':
                            valorConst = strtol(valorConstCAD, NULL, 10);
                            break;
                        case '@':
                            valorConst = strtol(valorConstCAD, NULL, 8);
                            break;
                        case '%':
                            valorConst = strtol(valorConstCAD, NULL, 16);
                            break;
                        case '\'':
                            valorConst = valorConstCAD[0];
                            break;
                        default:
                            valorConst = strtol(valorConstCAD, NULL, 10);
                            break;
                        }
                    }
                    //constante String
                    else
                    {
                        valorConst = n;
                        k++;
                        while (valorConstCAD[k] != '"')
                        {
                            str[largoString] = valorConstCAD[k];
                            largoString++;
                            k++;
                        }
                        str[largoString] = '\0';
                        largoString++;
                        n += largoString;
                    }
                    if (buscaRotulo(cod, *rotulos) == -1)
                    {
                        (*rotulos).tope++;

                        agregaRotulo(rotulos, cod, valorConst, largoString, str);
                        l--;
                    }
                }
            }
            l++;
        }
    }
    l--;
    for (int i = 0; i <= rotulos->tope; i++)
    {
        if (rotulos->rot[i].String)
        {
            rotulos->rot[i].linea = l;
            l += rotulos->rot[i].String;
        }
    }
}

int buscaRotulo(char cod[], TvecRotulo rotulos)
{
    int i = 0;
    while (i <= rotulos.tope && strcmpi(cod, rotulos.rot[i].nombre))
        i++;
    if (i <= rotulos.tope)
        return i;
    else
        return -1;
}

void agregaRotulo(TvecRotulo *rotulos, char cod[], int linea, int String, char str[])
{
    Rotulo rotAux;

    rotAux.linea = linea;
    strcpy(rotAux.nombre, cod);
    rotAux.String = String;
    if (String)
        strcpy(rotAux.str, str);
    (*rotulos).rot[(*rotulos).tope] = rotAux; //Medio enroscado, pero hace que el tema rotulo quede todo en una sola estructora, charlar con los chicos
}

void IniciaCadena(lineacod *LineaCodigo)
{
    memset((*LineaCodigo).cod, 0, sizeof((*LineaCodigo).cod));
    memset((*LineaCodigo).mnemom, 0, sizeof((*LineaCodigo).mnemom));
    memset((*LineaCodigo).op1, 0, sizeof((*LineaCodigo).op1));
    memset((*LineaCodigo).op2, 0, sizeof((*LineaCodigo).op2));
    memset((*LineaCodigo).comentario, 0, sizeof((*LineaCodigo).comentario));
}

void Desarma(char cadena[], instruccion *inst, lineacod *LineaCodigo, Tvec mnemos[], TvecRotulo *rotulos, int nroLinea, int *traduce, int *vacia)
{
    char cod[MAX] = "\0", equ[4];
    char A[MAX] = "\0";
    char B[MAX] = "\0";
    char C[MAX] = "\0";
    int i = 0, j = 0, l = 0, pos;
    *traduce = 1;
    //Inicializamos la instruccion toda en NULL(-1)
    (*inst).cod = (*inst).topA = (*inst).topB = -1;
    comeBasura(cadena, &i);
    while (cadena[i] != ';' && cadena[i] != ' ' && cadena[i] != ':' && cadena[i] != '\0')
    {
        cod[j] = cadena[i];
        i++;
        j++;
    }
    cod[j] = '\0';
    j = 0;
    //Caso con rotulo o declaracion de constante
    if (cadena[i] == ':')
    {
        sprintf((*LineaCodigo).cod, "%s:", cod);
        strcpy(cod, ""); //Ponemos en cero nuevamente el cod
        //Debe de seguir leyendo hasta encontrar un mnemonico
        i++; //Como estabamos parados en ':' ahora avanza al siguiente caracter
        l = 0;
        comeBasura(cadena, &i);
        while (cadena[i] != ' ' && cadena[i] != '\0')
        {
            cod[l] = cadena[i];
            i++;
            l++;
        }
        cod[l] = '\0';
    }
    else
    {
        sprintf((*LineaCodigo).cod, "%d:", nroLinea);
    }

    // if (cadena[i] != ';')
    //     i++;

    //Para que no se rompa
    //strcpy((*LineaCodigo).comentario, cod);

    pos = encuentramnemo(cod, mnemos, 31); //busco la posicion del mnemonico en el diccionario, si no encuentro devuelve -1
    //Agrego codigo instruccion
    if (pos != -1)
    {
        strcpy((*LineaCodigo).mnemom, cod);
        (*inst).cod = mnemos[pos].hex;
        if ((*inst).cod < 0xF0)
        { //2 operandos
            comeBasura(cadena, &i);
            while (cadena[i] != ',' && cadena[i] != '\0')
            {
                A[j] = cadena[i];
                j++;
                i++;
            }
            A[j] = '\0';
            elimEspacio(A);
            tipoOperando(A, &(*inst).topA, &(*inst).vopA, 12, *rotulos, nroLinea, traduce);
            A[j] = ',';
            j = 0;
            i++;
            comeBasura(cadena, &i);
            while (cadena[i] != '\0' && cadena[i] != ';' && cadena[i] != ',')
            {
                //Come basura necesario adentro para que detecte el futuro error si hay otro operando
                //comeBasura(cadena, &i);
                B[j] = cadena[i];
                j++;
                i++;
                if (B[0] != '\'')
                    comeBasura(cadena, &i);
            }
            B[j] = '\0';
            //elimEspacio(B);
            (*inst).topB = -1;
            tipoOperando(B, &(*inst).topB, &(*inst).vopB, 12, *rotulos, nroLinea, traduce);
            //Seguimos leyendo en busqueda de errores
            comeBasura(cadena, &i);
            j = 0;
            C[0] = '\0';
            while (cadena[i] != '\0' && cadena[i] != ';')
            {
                C[j] = cadena[i];
                j++;
                i++;
            }
            if (*traduce && (*inst).topB != -1 && (*inst).topA != -1 && C[0] == '\0')
                *traduce = 1;
            else
            {
                printf("ERROR:\tNo existe la instruccion ingresada\n");
                *traduce = 0;
            }
        }
        else
        {
            if ((*inst).cod < 0xFF0) //1 operando
            {
                j = 0;
                comeBasura(cadena, &i);
                //Leemos el primer operando
                while (cadena[i] != '\0' && cadena[i] != ' ' && cadena[i] != ';')
                {
                    A[j] = cadena[i];
                    j++;
                    i++;
                }
                A[j] = '\0';
                tipoOperando(A, &(*inst).topA, &(*inst).vopA, 16, *rotulos, nroLinea, traduce);
                A[j] = '\t';
                //Seguimos leyendo en busqueda de errores
                comeBasura(cadena, &i);
                j = 0;
                while (cadena[i] != '\0' && cadena[i] != ';' && B[0] == '\0')
                {
                    B[j] = cadena[i];
                    j++;
                    i++;
                }
                if (*traduce && (*inst).topB == -1 && (*inst).topA != -1 && B[0] == '\0')
                    *traduce = 1;
                else
                {
                    printf("ERROR:\tNo existe la instruccion ingresada\n");
                    *traduce = 0;
                }
            }
            else
            { //STOP
                //Seguimos leyendo en busqueda de errores
                comeBasura(cadena, &i);
                j = 0;
                while (cadena[i] != '\0' && A[0] == '\0')
                {
                    A[j] = cadena[i];
                    j++;
                    i++;
                }
                if (*traduce && (*inst).topB == -1 && (*inst).topA == -1 && A[0] == '\0')
                    *traduce = 1;
                else
                {
                    printf("ERROR:\tNo existe la instruccion ingresada\n");
                    *traduce = 0;
                }
            }
        }
    }
    else
    {
        *traduce = 0;
        comeBasura(cadena, &i);
        j = 0;
        if (cadena[i] != ';' && cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '\n' && cadena[i] != 'e' && cadena[i] != 'E')
        {
            printf("ERROR:\tNo existe la instruccion ingresada\n");
        }
        else if (cadena[i] == 'e' || cadena[i] == 'E')
        {
            while (cadena[i] != ' ')
            {
                equ[j] = cadena[i];
                j++;
                i++;
            }
            equ[j] = '\0';
            if (strcmpi(equ, "EQU") == 0)
            {
                strcpy((*LineaCodigo).comentario, cadena);
                *vacia = 1;
            }
        }
        else
            *vacia = 1;
    }
    strcpy((*LineaCodigo).op1, A);
    strcpy((*LineaCodigo).op2, B);
    if (cadena[i] != '\n')
    { //no cambia de linea, entonces tengo un comentario u otra instruccion;
        if (cadena[i] == ';')
        {
            j = 0;
            while (cadena[i] != '\n' && cadena[i] != '\0')
            {
                (*LineaCodigo).comentario[j] = cadena[i];
                i++;
                j++;
            }
            (*LineaCodigo).comentario[j] = '\0';
        }
    }
}

void comeBasura(char cad[], int *i)
{
    while (cad[*i] == ' ' || cad[*i] == '\t')
        *i = *i + 1;
}

void elimEspacio(char cad[])
{
    int i, j = 0;

    for (i = 0; cad[i]; i++)
        if (cad[i] != ' ' && cad[i] != '\t')
            cad[j++] = cad[i];

    cad[j] = '\0';
}

void trunca(int *ValorOperando, int bitsmax)
{
    int valororiginal = *ValorOperando;
    if ((*ValorOperando > 2047 || *ValorOperando < -2048) && bitsmax == 12)
    {
        if ((*ValorOperando & 0xFFF) >> 11) //Bit mas significativo de los 12bits es un 1 --> numero negativo
            *ValorOperando |= 0xFFFFF000;
        else
            *ValorOperando = (*ValorOperando & 0x00000FFF);
        printf("Warning Operacion : Truncamiento del Operando %d, valor nuevo: %d\n", valororiginal, (*ValorOperando));
    }
    else if ((*ValorOperando >= 32767 || *ValorOperando <= -32768) && bitsmax == 16)
    {
        if ((*ValorOperando & 0xFFFF) >> 15)
            *ValorOperando |= 0xFFFF0000;
        else
            *ValorOperando &= 0x0000FFFF;
        printf("Warning Operacion : Truncamiento del Operando %d, valor nuevo: %d\n", valororiginal, (*ValorOperando));
    }
}

void seteaHeader(char header[], int *tamDS, int *tamES, int *tamSS)
{
    char ASM[6];
    char SEGMENTO[8];
    char numchar[8];
    int i, j, num;
    for (i = 0; i < 5; i++)
        ASM[i] = header[i];
    ASM[5] = '\0';
    i++;
    if (strcmpi(ASM, "\\\\ASM") == 0)
    {
        //ciclo hasta terminar el string
        while (header[i])
        {
            j = 0; //auxiliar para avanzar en la cadena
            while (header[i] == ' ' || header[i] == '\t' || header[i] == '\n')
                i++;
            //si no me cai entro al if
            if (header[i])
            {
                while (header[i] != '=')
                {
                    SEGMENTO[j] = header[i];
                    j++;
                    i++;
                }
                SEGMENTO[j] = '\0';
                i++; //salteo el igual
                j = 0;
                while (header[i] != ' ' && header[i] != '\n')
                {
                    numchar[j] = header[i];
                    j++;
                    i++;
                }
                numchar[j] = '\0';
                num = strtol(numchar, NULL, 10);
                if (num > 32767 || num < 0)
                { //si es mayor a 2 bytes corta el programa
                    printf("ERROR:\t el tamano del segmento no es coherente con la arquitectura");
                    exit(1);
                }
                if (strcmpi(SEGMENTO, "DATA") == 0)
                    *tamDS = num;
                else if (strcmpi(SEGMENTO, "EXTRA") == 0)
                    *tamES = num;
                else if (strcmpi(SEGMENTO, "STACK") == 0)
                    *tamSS = num;
            }
        }
    }
    else
        printf("Warning: %s deberia ser \\\\ASM", ASM);
}
