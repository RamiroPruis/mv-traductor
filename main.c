#include <stdio.h>
#include <stdlib.h>
#include "dic-mnemo.h"
#include <string.h>
#define MAX 10

typedef struct{
  int cod;  //codigo instruccion
  int topA; //tipo operando A
  int topB; //tipo operando B
  int vopA; //valor operando A
  int vopB; //valor operando B
} instruccion;


int main()
{
    Tvec Mnemonicos[CANT];
    int i;
    FILE *arch;
    instruccion num;

    creadicc(Mnemonicos);


    num = (num.cod << 24) | (num.topA <<16) | (num.topB << 8) |  ;
    printf("%4X",num);



    return 0;
}

// cadena[] = "ADD [5],10;";
void Desarma(char[] cadena,instruccion inst,Tvec mnemos){
  char cod[MAX];
  char A[MAX];
  char B[MAX];
  char c = cadena[0];
  int i=0,pos;

  while(c[i] != ' '){
    cod[i]=cadena[i];
    i++;
  }
  pos = binarySearch(mnemos,0,24,cod))  //busco la posicion del mnemonico en el diccionario, si no encuentro devuelve -1
  //Agrego codigo instruccion
  if (pos!=-1)
    inst.cod = mnemos[pos].hex;

}

int binarySearch(Tvec arr[], int l, int r, cad[] x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (strcmp(arr[mid].mnemo,x)==0)
            return mid;
        if (strcmp(arr[mid].mnemo,x)>0)
            return binarySearch(arr, l, mid - 1, x);
        return binarySearch(arr, mid + 1, r, x);
    }
    return -1;
}


