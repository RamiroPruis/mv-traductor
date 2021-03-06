#define CANT 32
#define MAX 10

typedef struct
{
    char cadena[256];
} TvecCadenas;

typedef struct
{
    char mnemo[10];
    int hex;
} Tvec;

typedef struct
{
    char nombre[10];
    int linea;
    int String; //=0 si no es Const String, sino el largo del string
    char str[200];
} Rotulo;

typedef struct
{
    Rotulo rot[100];
    int tope;

} TvecRotulo;

typedef struct
{
    int cod;  //codigo instruccion
    int topA; //tipo operando A
    int topB; //tipo operando B
    int vopA; //valor operando A
    int vopB; //valor operando B
} instruccion;

typedef struct
{
    char cod[MAX];
    char mnemom[MAX];
    char op1[MAX];
    char op2[MAX];
    char comentario[200];
} lineacod;

void creadicc(Tvec[]);
int encuentramnemo(char[], Tvec[], int);
void tipoOperando(char[], int *, int *, int, TvecRotulo, int, int *);
int traduceInstruccion(instruccion);
void agregaRotulo(TvecRotulo *, char[], int,int,char[]);
void cargaRotulos(TvecCadenas[], int, TvecRotulo *);
void Desarma(char[], instruccion *, lineacod *, Tvec[], TvecRotulo *, int, int *, int *);
int buscaRotulo(char[], TvecRotulo);
void comeBasura(char[], int *);
void elimEspacio(char[]);
void trunca(int *, int);
void IniciaCadena(lineacod *);
void seteaHeader(char[],int*,int*,int*);
