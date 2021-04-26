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
    char txt[25];
    int i = 0;
    int traduce, creaBin = 1;
    int n;
    int k = 0;
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
     else{
        if (argc < 3)
        {
            printf("Error. Pocos argumentos \n");
            printf("Ejemplo: mvc.exe AsmFilename.asm BinFilename.bin [-o]");
            exit(1);
        }
        else{
            if (argc==4 && strcmp(argv[3],"-o")==0)
             flag = 1; //Omite la salida por pantalla de la traduccion.
         }
     }
    // COMIENZA Lectura del archivo .asm
    if ((arch = fopen(argv[1], "r")) == NULL)
        return 1;
    while (fgets(vecLineas[topeLineas].cadena, 256, arch) != NULL)
        topeLineas++;
    topeLineas--;
    for (int i = 0; i < topeLineas; i++)
        vecLineas[i].cadena[strlen(vecLineas[i].cadena) - 1] = '\0';
    fclose(arch);
    // FIN Lectura del archivo .asm

    cargaRotulos(vecLineas, topeLineas, &rotulos);

    //Ciclo desarmado

    int topeBinario = topeLineas;
    do
    {
        vacia = 0;
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
                creaBin = 1;
                printf("\t\t\t%s \n", LineaCodigo.comentario);
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

    if (creaBin)
    {
        i = 0;
        if ((arch = fopen(argv[2], "wb")) == NULL)
        {
            return 1;
        }
        // Se rompe
        while (i <= topeBinario)
        {
            fwrite(&(vectorbinario[i]), sizeof(int), 1, arch);
            i++;
        }
        fclose(arch);
        printf("Archivo binario creado con exito. Traduccion exitosa");
    }

    return 0;
}
