#define CANT 32

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

void creadicc(Tvec[]);
int encuentramnemo(char[], Tvec[], int);
void tipoOperando(char[], int *, int *, TvecRotulo);
int traduceInstruccion(instruccion);
void agregaRotulo(TvecRotulo *, char[], int);
void cargaRotulos(TvecCadenas[], int, TvecRotulo *);
void Desarma(char[], instruccion *, Tvec[], TvecRotulo *, int);
int buscaRotulo(char[], TvecRotulo);
void comeBasura(char[], int *);
