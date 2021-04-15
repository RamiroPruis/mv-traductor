#include <stdlib.h>
#include <stdio.h>
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

void creaReg(Tvec registros[]){
  strcpy(registros[0].mnemo,'DS');
  registros[0].hex=0;
  strcpy(registros[5].mnemo,'IP');
  registros[5].hex=5;
  strcpy(registros[8].mnemo,'CC');
  registros[8].hex=8;
  strcpy(registros[9].mnemo,'AC');
  registros[9].hex=9;
  strcpy(registros[10].mnemo,'AX');
  registros[10].hex=10;
  strcpy(registros[11].mnemo,'BX');
  registros[11].hex=11;
  strcpy(registros[12].mnemo,'CX');
  registros[12].hex=12;
  strcpy(registros[13].mnemo,'DX');
  registros[13].hex=13;
  strcpy(registros[14].mnemo,'EX');
  registros[14].hex=14;
  strcpy(registros[15].mnemo,'FX');
  registros[15].hex=15;
}

int encuentramnemo(char mnem[], Tvec vec[],int max){
   int i=0;
   while (i<=max && strcmpi(strupr(mnem),vec[i].mnemo)!=0)
        i++;
   if (i<=max)
        return i;
   else
        return -1;
}

void tipoOperando(char entrada, int* tipo, int* operando){
  int i=0,pos;
  char base='',abre[2];
  char num[6];
  Tvec reg[16];

  creaReg(reg);

  if(entrada[i]=='[')
    i++;
    *tipo = 2;
    do{
      num[i] = entrada[i];
      i++;
    }while(entrada[i]!=']');
  else
    strcpy(num,entrada);

  if(*tipo!=2){
    pos=encuentramnemo(reg,num,16);
    if(pos!=-1){
      *tipo = 1;
    }
    else
      *tipo = 0;
  }


}
