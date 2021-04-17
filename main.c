#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dic-mnemo.h"
#define MAX 10

int main(int argc, char *argv[])
{
    Tvec Mnemonicos[CANT];
    char linea[255];

    // Variables para :Lectura archivos
    FILE *arch;
    int flag = 0;
    int topeLineas = 0;

    TvecCadenas vecLineas[2000];
    lineacod LineaCodigo;

    int i = 0;
    int n;
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
    // else if (argc < 3)
    // {
    //     printf("Error. Pocos argumentos \n");
    //     printf("Ejemplo: mvc.exe AsmFilename.asm BinFilename.bin [-o]");
    //     exit(1);
    //     else if (argc == 4 && *argv[3] == "-o")
    //         flag = 1; //Omite la salida por pantalla de la traduccion.
    // }

    // COMIENZA Lectura del archivo .asm
    //strcpy(argv[1], "holaquetal.txt");
    if ((arch = fopen("holaquetal.txt", "r")) == NULL)
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

    do
    {
<<<<<<< HEAD
        Desarma(vecLineas[i].cadena, &num, &LineaCodigo,Mnemonicos, &rotulos, i);
        i++;
        n = traduceInstruccion(num);
        printf("[%04d]: %02X %02X %02X %02X",i,(n >> 24) & 0xFF, (n >> 16) & 0xFF, (n >> 8) & 0xFF, (n >> 0) & 0xFF);
        printf("%10s %2s %5s %s %15s \n",LineaCodigo.cod,LineaCodigo.mnemom,LineaCodigo.op1,LineaCodigo.op2,LineaCodigo.comentario);
=======
        Desarma(vecLineas[i].cadena, &num, &LineaCodigo, Mnemonicos, &rotulos, i);
        i++;
        n = traduceInstruccion(num);
        printf("[%04d]: %02X %02X %02X %02X", i, (n >> 24) & 0xFF, (n >> 16) & 0xFF, (n >> 8) & 0xFF, (n >> 0) & 0xFF);
        printf("%10s %2s %5s %s %15s \n", LineaCodigo.cod, strupr(LineaCodigo.mnemom), strupr(LineaCodigo.op1), strupr(LineaCodigo.op2), LineaCodigo.comentario);
>>>>>>> 8ecee5b46a7672dda64d5c9726bf8e72b7afe934

    } while (i <= topeLineas);

    //Ciclo desarmado

    /*creadicc(Mnemonicos);
    strcpy(linea, "otro:LDL OTRO");
    Desarma(linea, &num, Mnemonicos, &rotulos, 1); //Cambie esto
    /*if (arch=fopen(nombrearch,"r")==NULL) return 1;
    while (fgets(linea,sizeof(linea),nombrearch)!=NULL){
        Desarma(linea,num,mnemos);
    }
    */

    //num = (num.cod << 24) | (num.topA <<16) | (num.topB << 8) |  ;
    //printf("%02X",num.cod);

    return 0;
}
