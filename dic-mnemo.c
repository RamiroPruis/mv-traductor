#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic-mnemo.h"

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
}

void creaReg(Tvec registros[])
{
    //Inicializamos en null
    for (int i = 0; i < 16; i++)
        strcpy(registros[i].mnemo, "\0");

    strcpy(registros[0].mnemo, "DS");
    registros[0].hex = 0;
    strcpy(registros[5].mnemo, "IP");
    registros[5].hex = 5;
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
    int i = 0, j = 0, pos;
    char base = '\0';
    char num[6];
    Tvec reg[16];

    creaReg(reg);

    if (entrada[i] == '[')
    { //Operando indirecto
        i++;
        *tipo = 2;
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
    if (inst.cod >= 0 && inst.cod <= 11)
        resultado = ((inst.cod << 28) & 0xF0000000) | ((inst.topA << 26) & 0x0C000000) | ((inst.topB << 24) & 0x03000000) | ((inst.vopA << 12) & 0x00FFF000) | (inst.vopB & 0x00000FFF);
    //Instruccion de un operando
    else if (inst.cod >= 240 && inst.cod <= 251)
        resultado = ((inst.cod << 24) & 0xFF000000) | ((inst.topA << 22) & 0x00C00000) | (inst.vopA & 0x00000FFF);
    //Instrucciones sin operando
    else
        resultado = (inst.cod << 20) & 0xFFF00000;
    return resultado;
}

void cargaRotulos(TvecCadenas vec[], int n, TvecRotulo *rotulos)
{
    char cod[MAX];
    Rotulo rotAux;
    char cadenaActual[256];
    int j, k, l = 0;
    //Recorremos las lineas en busqueda de rotulos
    for (int i = 0; i <= n; i++)
    {
        //Leemos hasta que el siguiente sea ':'
        strcpy(cadenaActual, vec[i].cadena);
        j = 0;
        k = 0;
        comeBasura(cadenaActual, &j);
        if (cadenaActual[0] != '\0' && cadenaActual[0] != ';')
        {

            while (cadenaActual[j] != ' ' && cadenaActual[j] != ':')
            {
                cod[k] = cadenaActual[j];
                j++;
                k++;
            }
            cod[k] = '\0';
            //Caso rotulo
            if (cadenaActual[j] == ':')
            {
                //Vemos si el rotulo no existe
                if (buscaRotulo(cod, *rotulos) == -1)
                {
                    (*rotulos).tope++;
                    agregaRotulo(rotulos, cod, l);
                }
            }
            l++;
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

void agregaRotulo(TvecRotulo *rotulos, char cod[], int linea)
{
    Rotulo rotAux;

    rotAux.linea = linea;
    strcpy(rotAux.nombre, cod);
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
    char lineaentera[500];
    char cod[MAX] = "\0";
    char A[MAX] = "\0";
    char B[MAX] = "\0";
    char C[MAX] = "\0";
    int i = 0, j = 0, k = 0, l = 0, pos;
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
    //Caso con rotulo
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
    pos = encuentramnemo(cod, mnemos, 24); //busco la posicion del mnemonico en el diccionario, si no encuentro devuelve -1
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
                comeBasura(cadena, &i);
                B[j] = cadena[i];
                j++;
                i++;
                comeBasura(cadena, &i);
            }
            B[j] = '\0';
            elimEspacio(B);
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
            if ((*inst).cod < 0xFF1) //1 operando
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
        if (cadena[i] != ';' && cadena[i] != ' ' && cadena[i] != '\0' && cadena[i] != '\n')
        {
            printf("ERROR:\tNo existe la instruccion ingresada\n");
        }
        else
        {
            *vacia = 1;
        }
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
