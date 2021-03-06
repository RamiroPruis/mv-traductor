#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic-mnemo.h"
#define MAX 10

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

    tamDS = tamSS = tamES = 1024; //Por defecto cada segment ocupa 1024

    instruccion num;
    TvecRotulo rotulos;

    //Inicializaciones
    rotulos.tope = -1;
    creadicc(Mnemonicos);

    if (argc > 4)
    {
        printf("Error. Demasiados argumentos");
        exit(1);
    }
    else
    {
        if (argc < 3)
        {
            printf("Error. Pocos argumentos \n");
            printf("Ejemplo: mvc.exe AsmFilename.asm BinFilename.bin [-o]");
            exit(1);
        }
        else
        {
            if (argc == 4 && strcmp(argv[3], "-o") == 0)
                flag = 1; //Omite la salida por pantalla de la traduccion.
        }
    }

    // COMIENZA Lectura del archivo .asm

    if ((arch = fopen(argv[1], "r")) == NULL)
        return 1;

/*
    if ((arch = fopen("6.asm", "r")) == NULL)
        return 1;*/
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

    i=0;
    while (i<=topeLineas && (tamDS == 1024 || tamES == 1024 || tamSS == 1024)){
        seteaHeader(vecLineas[i].cadena,&tamDS,&tamES,&tamSS);
        i++;
    }

    i=0;


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

        if ((arch = fopen(argv[2], "wb")) == NULL)        {
            return -1;
        }
        /*
        if ((arch = fopen("6.bin", "wb")) == NULL)        {
            return -1;
        }*/
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
        printf("Archivo binario creado con exito. Traduccion exitosa. \n");
    }
    else
        printf("No se creo el binario");

    printf("Gracias por utilizar MV21   ? Copyright ? 2021 Grupo D MV Inc. ? \n");
    return 0;
}
