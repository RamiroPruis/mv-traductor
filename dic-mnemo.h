#define CANT 32

typedef struct{
  char mnemo[10];
  int hex;
} Tvec;

typedef struct{
  int cod;  //codigo instruccion
  int topA; //tipo operando A
  int topB; //tipo operando B
  int vopA; //valor operando A
  int vopB; //valor operando B
} instruccion;

void creadicc(Tvec[]);
int encuentramnemo(char [], Tvec [],int);
void tipoOperando(char[], int*, int*);
int traduceInstruccion(instruccion);
