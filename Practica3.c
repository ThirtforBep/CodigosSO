#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

#define TAM16 16
#define TAM8 8
#define TAM7 7

//Estructura memoria Fisica
typedef struct MemoriaFisica MarcoPaginas;
struct MemoriaFisica{

    int NumMa;
    int Direc;
    int Age;
    MarcoPaginas *sig;
};

//Estructura Memory
typedef struct Memory Pages;
struct Memory{
    int Direc;
    int NumPa;
    Pages *sig;
};

void *PaginaNueva();
void *MarcoNuevo();
void MarcoPagIns(MarcoPaginas **Puntero, int numMarco, int numDirec, int edad);
void AgregarPag(Pages **Puntero, int numPag, int numDirec);
void ImpMV(Pages *Puntero);
void ImpMF(MarcoPaginas *Puntero);
int Num(int posicion, int Vector[]);
void Mal(int numPag, Pages **Puntero1, MarcoPaginas **Puntero2);
void ImBin(int num);
void MDirec(int selecciono, MarcoPaginas **Puntero2);

int main(){
    Pages *Memoria64k = NULL;
    MarcoPaginas *Memoria32k = NULL;
    int posicion, Vector[TAM16], VectorEdad[TAM16], NumePag, Opcion, Selecciono, edad;
    system("clear");
    srand(time(NULL));
    for(int i = 0; i < TAM16; i++){
        do{
            posicion = 1+rand()%TAM16;
        }while(Num(posicion, Vector));{
            Vector[i] = posicion;
            if(posicion >= 9){
            AgregarPag(&Memoria64k, i, -1);
            }else{
                do{
                edad = 1+rand()%50;
                }while(Num(edad, VectorEdad));{
                    VectorEdad[i] = edad;
                    AgregarPag(&Memoria64k, i, posicion -1);
                    MarcoPagIns(&Memoria32k, i, posicion - 1 , edad);
                }
            }
        }
    }
    do{
        printf("\n");

        ImpMV(Memoria64k);
        ImpMF(Memoria32k);

        printf( "\n\t\t\t\t\t\t\tMP/Marco de pagina\n\t\t\t\t\t\t\tPA -> Pagina\n\n");

        printf("\t\t\t\t\t\t\t1) Agregar una pagina\n");
        printf("\t\t\t\t\t\t\t2) Visualizar direcciones de memoria\n");
        printf("\t\t\t\t\t\t\t3) Exit\n");
        printf("\t\t\t\t\t\t\tOpcion: ");
        scanf("%d", &Opcion);
        switch(Opcion){
        case 1:
            printf("\t\t\t\t\t\t\tPagina que desea pasar a la pagina ");
            scanf("%d", &Selecciono);
            Mal(Selecciono, &Memoria64k,
            &Memoria32k);
            break;
        case 2:
            printf("\t\t\t\t\t\t\tD que marco de pagina dessea visualizar sus direcciones ");
            scanf("%d", &Selecciono);
            MDirec(Selecciono, &Memoria32k);
            break;
        default:
            printf("\t\t\t\t\t\t Ingresa otra opcion\n");
            sleep(2);
            break;
        }
    }while(Opcion != 3);{
        printf("Fin\n");
    }
}

void *PaginaNueva(){
    Pages Nueva = (Pages)malloc(sizeof(Pages));
    if(Nueva == NULL){
        perror("No se puede crear\n");
    }else{
        return Nueva;
    }
}

void *MarcoNuevo(){
    MarcoPaginas *Nueva = (MarcoPaginas*)malloc(sizeof(MarcoPaginas));
    if(Nueva == NULL){
        perror("No se puede crear pagina\n");
    }else{
        return Nueva;
    }
}


void MarcoPagIns(MarcoPaginas **Puntero, int numMarco, int numDirec, int edad){

    MarcoPaginas *Actual, *Anterior, *pNuevo, *pPrimeraMarco;
    pPrimeraMarco = *Puntero;
    Actual = pPrimeraMarco;
    Anterior = pPrimeraMarco;
    if(pPrimeraMarco == NULL){
        pPrimeraMarco = MarcoNuevo();
        pPrimeraMarco->NumMa = numMarco;
        pPrimeraMarco->Direc = numDirec;
        pPrimeraMarco->Age = edad;
        pPrimeraMarco->sig = NULL;
        *Puntero = pPrimeraMarco;
    }else{
        while(Actual != NULL && numDirec < Actual->Direc){
            Anterior = Actual;
            Actual = Actual->sig;
        }
        pNuevo = MarcoNuevo();
        pNuevo->NumMa = numMarco;
        pNuevo->Direc = numDirec;
        pNuevo->Age = edad;
        if(Anterior == Actual){
            pNuevo->sig = pPrimeraMarco;
            pPrimeraMarco = pNuevo;
        }else{
            pNuevo->sig = Actual;
            Anterior->sig = pNuevo;
        }
        *Puntero = pPrimeraMarco;
    }
}


void AgregarPag(Pages **Puntero, int numPag, int numDirec){

    Pages *Actual, *Anterior, *pNuevo, *pPrimeraPag;
    pPrimeraPag = *Puntero;
    Actual = pPrimeraPag;
    Anterior = pPrimeraPag;
    if(pPrimeraPag == NULL){
        pPrimeraPag = PaginaNueva();
        pPrimeraPag->NumPa = numPag;
        pPrimeraPag->Direc = numDirec;
        pPrimeraPag->sig = NULL;
        *Puntero = pPrimeraPag;
    }else{
        while(Actual != NULL && numPag < Actual->NumPa){
            Anterior = Actual;
            Actual = Actual->sig;
        }
        pNuevo = PaginaNueva();
        pNuevo->NumPa = numPag;
        pNuevo->Direc = numDirec;
        if(Anterior == Actual){
        pNuevo->sig = pPrimeraPag;
        pPrimeraPag = pNuevo;
        }else{
            pNuevo->sig = Actual;
            Anterior->sig = pNuevo;
        }
        *Puntero = pPrimeraPag;
    }
}


void ImpMV(Pages *Puntero){

    Pages *Auxiliar;
    int Num;
    Auxiliar = Puntero;
    if(Auxiliar == NULL){
        perror("Sin memoria\n");
    }else{
        printf("\t\t\t\t\t\t\tMemory Virtual \n");
        printf( "\t\t\t\t\t\t\t\t|PA|MP|\n");
        while(Auxiliar != NULL){
            if(Auxiliar->Direc != -1){
                printf( "\t\t\t\t\t\t\t\t %.2d %.2d",
                Auxiliar->NumPa, Auxiliar->Direc);
            }else{
                printf("\t\t\t\t\t\t\t\t %.2d %s",
                Auxiliar->NumPa, "xx");
            }
            printf("\n");
            Auxiliar = Auxiliar->sig;
        }
    }
}


void ImpMF(MarcoPaginas *Puntero){
    MarcoPaginas *Auxiliar;
    int Num;
    Auxiliar = Puntero;
    if(Auxiliar == NULL){
        perror("Sin memoria\n");
    }else{
        printf( "\t\t\t\t\t\t\tMemory Fisica\n");
        printf( "\t\t\t\t\t\t\t\t|MP|PA|\n");
        while(Auxiliar != NULL){
            printf( "\t\t\t\t\t\t\t\t %.2d %.2d",
            Auxiliar->Direc, Auxiliar->NumMa);
            printf("\n");
            Auxiliar = Auxiliar->sig;
        }
    }
}

int Num(int posicion, int Vector[]){

    int bandera = 0;
    for(int i = 0; i < TAM16; i++){
        if(posicion == Vector[i]){
            bandera = 1;
        }
    }
    return bandera;
}

int Bin(int Num){

    if(Num > 1){
        Bin(Num/2);
    }
    printf("%d", Num%2);
}

void Mal(int numPag, Pages **Puntero1, MarcoPaginas **Puntero2){

    Pages *MemoriaVirtual, *Anterior, *Actual;
    MarcoPaginas *MemoriaFisica, *AnteriorFisca, *ActualFisica, *Auxiliar;
    int NumPag, NumDirec, NumMarco, edadMenor = 0, edad;
    MemoriaVirtual = *Puntero1;
    Anterior = MemoriaVirtual;
    Actual = MemoriaVirtual;

    if(MemoriaVirtual == NULL){
        perror("no se selecciono la memoria virtual\n");
    }else{
        while(Actual != NULL && numPag != Actual->NumPa){
        Anterior = Actual;
        Actual = Actual->sig;
        }
        if(Actual == NULL){
        perror("Dato no encontrado\n");
        }else{
            if(Actual->Direc != -1){
            printf("Pagina en memoria\n");
            sleep(2);
            }else{
                NumPag = Actual->NumPa;
                MemoriaFisica = *Puntero2;
                AnteriorFisca = MemoriaFisica;
                ActualFisica = MemoriaFisica;
                Auxiliar = MemoriaFisica;

                if(MemoriaFisica == NULL){
                    perror("Sin memoria fisica\n");
                }else{
                    while(Auxiliar != NULL){
                        if(Auxiliar->Age > edadMenor){
                        edadMenor = Auxiliar->Age;
                        }
                        Auxiliar = Auxiliar->sig;
                    }
                    while(ActualFisica != NULL && edadMenor != ActualFisica->Age){
                        AnteriorFisca = ActualFisica;
                        ActualFisica =
                        ActualFisica->sig;
                    }
                    if(ActualFisica == NULL){
                        perror("Dato no encontrado\n");
                    }else{
                        edad = 1+rand()%50;
                        ActualFisica->NumMa = NumPag;
                        ActualFisica->Age = edad;
                        NumMarco = ActualFisica->Direc;
                    }
                }
                *Puntero2 = MemoriaFisica;
            }
            *Puntero1 = MemoriaVirtual;
        }
    }
}

void ImBin(int num){
    int cont1 = 1, cont2 = 0;
    while(cont1 != 4097){
        if(cont1 % 8 == 0){
            Bin(num);
            printf("-");
            Bin(cont2);
            printf("\n");
        }else{
            Bin(num);
            printf("-");
            Bin(cont2);
            printf(" ");
        }
        cont2++;
        cont1++;
    }
}

void MDirec(int selecciono, MarcoPaginas **Puntero2){
    MarcoPaginas *MemoriaFisica, *Anterior, *Actual;
    int numPag, numMar;
    MemoriaFisica = *Puntero2;
    Anterior = MemoriaFisica;
    Actual = MemoriaFisica;
    if(MemoriaFisica == NULL){
        perror("Sin memoria fisica\n");
    }else{
        while(Actual != NULL && selecciono != Actual->Direc){
            Anterior = Actual;
            Actual = Actual->sig;
        }
        if(Actual == NULL){
            perror("Sin dato\n");
        }else{
            numPag = Actual->Direc;
            numMar = Actual->NumMa;
            system("clear");
            printf("\n\n\t\t\t\t\t\tMemory Virtual\n");
            ImBin(numPag);
            printf("\n\n\t\t\t\t\t\t\tMemory Fisica\n");
            ImBin(numMar);
        }
    }
}