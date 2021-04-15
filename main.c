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
    char linea[255];
    char a[4]="A1";
    int i = strtol(a,NULL,16);

    FILE *arch,*nombrearch;
    instruccion num;
    creadicc(Mnemonicos);
    strcpy(linea,"STOP ");
    //Desarma(linea,num,Mnemonicos);
    /*if (arch=fopen(nombrearch,"r")==NULL) return 1;
    while (fgets(linea,sizeof(linea),nombrearch)!=NULL){
        Desarma(linea,num,mnemos);
    }
    */
    printf("%d",i);






    //num = (num.cod << 24) | (num.topA <<16) | (num.topB << 8) |  ;
    //printf("%02X",num.cod);



    return 0;
}

// cadena[] = "ADD [5],10;";
void Desarma(char cadena[],instruccion* inst,Tvec mnemos[]){
  char cod[MAX];
  char A[MAX];
  char B[MAX];
  int i=0,j=0,k=0,pos;

  while(cadena[i] != ' '){
    cod[i]=cadena[i];
    i++;
  }
  i++;
  pos = encuentramnemo(cod,mnemos,24);  //busco la posicion del mnemonico en el diccionario, si no encuentro devuelve -1
  //Agrego codigo instruccion
  if (pos!=-1){
    (*inst).cod = mnemos[pos].hex;
    if (inst.cod < 11){ //2 operandos
        printf("%02X",inst.cod);
        while(cadena[i] != ','){
          A[j]=cadena[i];
          i++;
        }
        tipoOperando(A,inst.topA,inst.vopA);

    }
    else{
        if ((*inst).cod<23) //1 operando
        {

        }
        else{ //STOP
            (*inst).topA=i;
            (*inst).topB=1;
        }
    }
  }
  else
    printf("Error");
}





