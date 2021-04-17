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
    char mnemom[MAX];
    char cod[MAX];
    char op1[MAX];
    char op2[MAX];
    char comentario[100];
} lineacod;

void creadicc(Tvec[]);
int encuentramnemo(char[], Tvec[], int);
void tipoOperando(char[], int *, int *, TvecRotulo);
int traduceInstruccion(instruccion);
void agregaRotulo(TvecRotulo *, char[], int);
void cargaRotulos(TvecCadenas[], int, TvecRotulo *);
<<<<<<< HEAD
void Desarma(char[], instruccion *,lineacod *, Tvec[], TvecRotulo *, int);
=======
void Desarma(char[], instruccion *, lineacod *, Tvec[], TvecRotulo *, int, int *);
>>>>>>> d836e14bdabb04063fcebb7f490ab99d4bc457da
int buscaRotulo(char[], TvecRotulo);
void comeBasura(char[], int *);
void elimEspacio(char[]);