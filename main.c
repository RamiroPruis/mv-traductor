#include <stdio.h>
#include <stdlib.h>
#include "dic-mnemo.h"


int main()
{
    Tvec Mnemonicos[CANT];
    int i;

    creadicc(Mnemonicos);

    for(i=0;i<25;i++)
      printf("%s    %02X \n",Mnemonicos[i].mnemo,Mnemonicos[i].hex);

    return 0;
}
