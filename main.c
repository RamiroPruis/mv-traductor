#include <stdio.h>
#include <stdlib.h>
#include "dic-mnemo.h"
#include <string.h>
#define MAX 10

void Desarma(char[], instruccion *, Tvec[], TvecRotulo *, int);

int main()
{
    Tvec Mnemonicos[CANT];
    char linea[255];

    // Variables para :Lectura archivos
    FILE *arch, *nombrearch; //¿NombreArch para que?
    char *filename;          //Viene por parametro del main
    int topeLineas = 0;
    char vecLineas[100];

    instruccion num;
    TvecRotulo rotulos;

    //Inicializaciones
    rotulos.tope = -1;

    // COMIENZA Lectura del archivo .asm
    if (arch = fopen(filename, "r") == NULL)
        return 1;
    while (fgets(vecLineas[topeLineas], 256, arch) != NULL)
        topeLineas++;
    // FIN Lectura del archivo .asm

    creadicc(Mnemonicos);
    strcpy(linea, "otro:LDL OTRO");
    Desarma(linea, &num, Mnemonicos, &rotulos, 1); //Cambie esto
    /*if (arch=fopen(nombrearch,"r")==NULL) return 1;
    while (fgets(linea,sizeof(linea),nombrearch)!=NULL){
        Desarma(linea,num,mnemos);
    }
    */
    printf("%02X %d %d %d %d", num.cod, num.topA, num.topB, num.vopA, num.vopB);

    //num = (num.cod << 24) | (num.topA <<16) | (num.topB << 8) |  ;
    //printf("%02X",num.cod);

    return 0;
}

// cadena[] = "ADD [5],10;";
void Desarma(char cadena[], instruccion *inst, Tvec mnemos[], TvecRotulo *rotulos, int nroLinea) //Cambie esto
{
    char cod[MAX];
    char A[MAX];
    char B[MAX];
    int i = 0, j = 0, k = 0, l = 0, pos;

    while (cadena[i] != ' ' && cadena[i] != ':')
    {
        cod[i] = cadena[i];
        i++;
    }
    cod[i] = '\0';
    //Caso con rotulo
    if (cadena[i] == ':')
    {
        (*rotulos).tope += 1;
        agregaRotulo(rotulos, cod, nroLinea);
        strcpy(cod, ""); //Ponemos en cero nuevamente el cod

        //Debe de seguir leyendo hasta encontrar un mnemonico
        i++; //Como estabamos parados en ':' ahora avanza al siguiente caracter
        l = 0;
        while (cadena[i] != ' ')
        {
            cod[l] = cadena[i];
            i++;
            l++;
        }
        cod[l] = '\0';
    }
    i++;
    //Para que no se rompa

    pos = encuentramnemo(cod, mnemos, 24); //busco la posicion del mnemonico en el diccionario, si no encuentro devuelve -1
    //Agrego codigo instruccion
    if (pos != -1)
    {
        (*inst).cod = mnemos[pos].hex;
        if ((*inst).cod <= 0xF0)
        { //2 operandos

            while (cadena[i] != ',')
            {
                A[j] = cadena[i];
                j++;
                i++;
            }
            A[j] = '\0';
            (*inst).topA = -1;
            tipoOperando(A, &(*inst).topA, &(*inst).vopA, *rotulos);
            j = 0;
            i++;
            while (cadena[i] != '\0')
            {
                B[j] = cadena[i];
                j++;
                i++;
            }
            B[j] = '\0';
            (*inst).topB = -1;
            tipoOperando(B, &(*inst).topB, &(*inst).vopB, *rotulos);
        }
        else
        {
            if ((*inst).cod < 0xFF1) //1 operando
            {
                j = 0;
                while (cadena[i] != '\0')
                {
                    A[j] = cadena[i];
                    j++;
                    i++;
                }
                A[j] = '\0';
                (*inst).topA = -1;
                tipoOperando(A, &(*inst).topA, &(*inst).vopA, *rotulos);
            }
            else
            { //STOP
                (*inst).topA = i;
                (*inst).topB = 1;
            }
        }
    }
    else
        printf("Error\n");
}

void agregaRotulo(TvecRotulo *rotulos, char cod[], int linea)
{
    Rotulo rotAux;

    rotAux.linea = linea;
    strcpy(rotAux.nombre, cod);
    (*rotulos).rot[(*rotulos).tope] = rotAux; //Medio enroscado, pero hace que el tema rotulo quede todo en una sola estructora, charlar con los chicos
}
