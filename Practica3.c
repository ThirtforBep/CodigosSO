#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define DECIMAL 4096
#define BITS_DECIMAL 11
#define BITS_PAGINAS_MV 3
#define BITS_MARCOS_MF 2
#define MV_SIZE 16
#define MF_SIZE 8
#define UNIDADES_MEMORIA 4096

//Estructura de nuestra lista
typedef struct lista_general{
int my_indice;
int its_indice;
int estado;
struct lista_general *sig_list;
}lista;
typedef lista *punterolista;

//Llamadas a funcion
void insertar (punterolista *cabeza, int e);
void imprimir_mv (punterolista cabeza);
void imprimir_mf (punterolista cabeza);
void binario4096(int i);
void binarioPagina(int i);
void binarioMarcos(int i);
void ocupacion(int mv, int mf, punterolista aux_mv_cabeza, punterolista aux_mf_cabeza);


int main(){
    int i;
    int seleccion;
    int mv, mf;
    punterolista mv_cabeza; //Indica la cabeza de la lista enlazada, si la perdemos no podremos acceder a la lista
    punterolista mf_cabeza;
    mv_cabeza = NULL; //Se inicializa la cabeza como NULL ya que no hay ningun nodo cargado en la lista
    mf_cabeza = NULL;
    for(i=0;i<MV_SIZE;i++)
    	insertar(&mv_cabeza, i);
    for(i=0;i<MF_SIZE;i++)
    	insertar(&mf_cabeza, i);
    printf("Hola Usuario, muy buenas tardes. Permiteme mostrarte la tabla de paginas :D \n\n");
    imprimir_mv(mv_cabeza);
    printf("\n Ahora los Marcos \n\n");
    imprimir_mf(mf_cabeza);
    printf("Perfecto, ahora ayudame a decidir entre las opciones que tenemos: \n ");
    do{
        printf("[1] Poner un Marco en una pagina \n");
        printf("[2] Ver los cambios en las Paginas y los Marcos \n");
        printf("[3] Salir \n");
        scanf("%d", &seleccion);
        if(seleccion==1){
            printf("Muy buena eleccion \n");
            printf("Ahora indicame el indice de memoria VIRTUAL (Pagina) que quieres mover: \n");
            scanf("%d",&mv);
            printf("Excelente, ahora dime el indice de memoria FISICA (Marco) que deseas sustituir: ");
            scanf("%d",&mf);
            printf("Trabajando \n");
            ocupacion(mv,mf,mv_cabeza,mf_cabeza);
        }
        if(seleccion == 2){
            imprimir_mv(mv_cabeza);
            espera(1);
            imprimir_mf(mf_cabeza);
        }
        else
        printf("\n");
    }while(seleccion!=3);
    return 0;
}

//FUNCIONES CREADAS ------------------------------------------------------------------------

void insertar (punterolista *cabeza, int e){
    punterolista nuevo; //Creamos un nuevo nodo
    nuevo = malloc(sizeof(lista)); //Utilizamos malloc para reservar memoria para ese nodo
    nuevo->my_indice = e; //Le asignamos el valor ingresado por pantalla a ese nodo
    nuevo->estado = 1;
    nuevo->sig_list = *cabeza; //Le asignamos al siguiente el valor de cabeza
    *cabeza = nuevo; //Cabeza pasa a ser el ultimo nodo agregado
}

//Imprimimos las paginas
void imprimir_mv(punterolista cabeza){
    printf("----------------------------------------\n");
	printf("|Indice\t|\tMV\t|  MF   |ESTADO|\n");
	printf("----------------------------------------\n");
    while(cabeza != NULL){ //Mientras cabeza no sea NULL
        printf("|  %d\t|",cabeza->my_indice);//Indice
        binarioPagina(cabeza->my_indice);
        printf(" / %d", cabeza->my_indice);//MV
        printf("\t|");
        binarioMarcos(cabeza->its_indice);
        printf("/%d",cabeza->its_indice);//MF
		printf("\t|");
        printf("  %d   |",cabeza->estado);//Estado
        printf("\n");
        cabeza = cabeza->sig_list; //Pasamos a la siguiente columna de nodos
    }
	printf("----------------------------------------\n");
}

//Imprimimos Marcos
void imprimir_mf(punterolista cabeza){
    printf("-----------------------------------------\n");
	printf("|Indice\t|\tMV\t|  MF    |ESTADO|\n");
	printf("-----------------------------------------\n");
    while(cabeza != NULL){ //Mientras cabeza no sea NULL
        printf("|  %d    |",cabeza->my_indice);//Indice
        binarioMarcos(cabeza->my_indice);
        printf(" / %d", cabeza->my_indice);
        printf("\t|");
        binarioPagina(cabeza->its_indice);
        printf(" / %d|",cabeza->its_indice);//MF
        printf("  %d   |",cabeza->estado);
        printf("\n");
        cabeza = cabeza->sig_list; //Pasamos a la siguiente columna de nodos
    }
	printf("-----------------------------------------\n");
}

void binario4096(int i){
    int bits = BITS_DECIMAL;
    while(bits >= 0){
        if(i & (((long int) 1) << bits))
        printf("1");
        else
        printf("0");
        bits--;
    }
}

void binarioPagina(int i){
    int bits = BITS_PAGINAS_MV;
    while(bits >= 0){
    if(i & (((long int) 1) << bits))
        printf("1");
    else
        printf("0");
    bits--;
    }
}

void binarioMarcos(int i){
    int bits = BITS_MARCOS_MF;
    while(bits >= 0){
        if(i & (((long int) 1) << bits))
        printf("1");
        else
        printf("0");
        bits--;
    }
}

void ocupacion(int mv, int mf, punterolista aux_mv_cabeza, punterolista aux_mf_cabeza){
    int i;
    while(aux_mv_cabeza!= NULL){
        if(aux_mv_cabeza->my_indice == mv){
            while(aux_mf_cabeza != NULL){
                if(aux_mf_cabeza->my_indice == mf){
                    aux_mv_cabeza->its_indice = aux_mf_cabeza->my_indice;
                    aux_mf_cabeza->its_indice = aux_mv_cabeza->my_indice;
                    aux_mv_cabeza->estado = 0;
                    aux_mf_cabeza->estado = 0;
                    for(i=aux_mv_cabeza->my_indice * UNIDADES_MEMORIA;i < aux_mv_cabeza->my_indice * UNIDADES_MEMORIA + UNIDADES_MEMORIA;i++){
                        binarioPagina(aux_mv_cabeza->my_indice);
                        printf(" ");
                        binario4096(i);
                        printf("\t");
                        binarioMarcos(aux_mf_cabeza->my_indice);
                        printf(" ");
                        binario4096(i);
                        printf("\n");
                    }
                }
                aux_mf_cabeza = aux_mf_cabeza->sig_list;
            }
        }
        aux_mv_cabeza = aux_mv_cabeza->sig_list;
    }
}
