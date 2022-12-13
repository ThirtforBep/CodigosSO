#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//BITS NECESARIOS PARA CONVERTIR DECIMAL A BINARIO; TAMAÑO DE LASUNIDADES DE MEMORIA -------

//BITS
#define DECIMAL 4096
#define BITS_DECIMAL 11
#define BITS_PAGINAS_MV 3
#define BITS_MARCOS_MF 2

//TAMAÑOS DE LA MEMORIA
#define MV_SIZE 16
#define MF_SIZE 8
#define UNIDADES_MEMORIA 4096

//ESTRUCTURAS DE LAS LISTAS_GENERALES -------------------------------------------------------
typedef struct lista_general{
//INDICE DE PROPIO E INDICE AJENO
int my_indice;
int its_indice;
int estado;
//PUNTERO A LISTA ENLAZADA
struct lista_general *sig_list;
}lista;
typedef lista *punterolista;

//PROTOTIPOS DE LAS FUNCIONES ----------------------------------------------------------------
void insertar (punterolista *cabeza, int e);
void imprimir_mv (punterolista cabeza);
void imprimir_mf (punterolista cabeza);
//void borrarLista (tpuntero *cabeza);
void binario4096(int i);
void binarioPagina(int i);
void binarioMarcos(int i);
void ocupacion(int mv, int mf, punterolista aux_mv_cabeza, punterolista aux_mf_cabeza);
//FUNCIONES ESTETICAS
void espera(int i);

//FUNCION MAIN ------------------------------------------------------------------------------
int main(){
    int i;
    int seleccion;
    int mv, mf;
    punterolista mv_cabeza; //Indica la cabeza de la lista enlazada, si la perdemos no
    podremos acceder a la lista
    punterolista mf_cabeza;
    mv_cabeza = NULL; //Se inicializa la cabeza como NULL ya que no hay ningun nodo
    cargado en la lista
    mf_cabeza = NULL;
    for(i=0;i<MV_SIZE;i++)
    insertar(&mv_cabeza, i);
    for(i=0;i<MF_SIZE;i++)
    insertar(&mf_cabeza, i);
    printf("\tESTO ES UN SIMULACRO!!!!!!!! ESTO ES UN SIMULACRO!!!!!!!!\n");
    printf("\tNINGUNA PARTE DE LA MEMORIA SERA DAÑADA DURANTE EL PROCESO DE ESTE SIMULACRO\n");
    //espera(3);
    printf("Bueno la verdad dañe poquito mi disco al principio xc\n");
    //espera(3);
    printf("Tamos haciendo las paginas espere :3\n");
    //espera(3);
    printf("\nCON USTEDES!!!! LAS PAGINAS!!!!! \n");
    //espera(1);
    imprimir_mv(mv_cabeza);
    printf("\n\t Y AHORA... LOS MARCOS PARA NUESTRAS PAGINAS\n");
    //espera(3);
    imprimir_mf(mf_cabeza);
    printf("\n\t AHORA LLEGO EL MOMENTO DE LA VERDAD USTED VA A DECIDIR QUE HACER CON El DESTINO DE LAS PAGINAS Y EN QUE MARCO SE QUEDARA CADA PAGINA\n ");
    printf("\t (que emocion, que nervios)");
    //espera(3);
    do{
        printf("\n\t DENTRO DE LA CATAFIXIA TENEMOS 3 OPCIONES, OKEY NO SOLO SON 2 NO HAY PRESUPUESTO \"F\"\n\n PERO DIGAME SEÑOR AGUILERA QUE TENEMOS!!!!!!! ");
        printf("\n[1] TENEMOS UN MARCO PARA UNA PAGINA!!");
        printf("\n[2] Con esta opcion puedes ver las paginas y los marcos :3");
        printf("\n[3] Salir :)\n");
        scanf("%d", &seleccion);
        if(seleccion==1){
            printf("\n-------------------------------------*MODO SERIO-------------------------------------");
            printf("\n\nMuy bien ahora necesito que me diga el indice de memoria VIRTUAL (pagina) quiere mover: ");
            scanf("%d",&mv);
            printf("\n\nMuy bien ahora necesito que me diga el indice de memoria FISICA (marco) que quiere ocupar: ");
            scanf("%d",&mf);
            printf("\nVamos a trabajar en esto");
            ocupacion(mv,mf,mv_cabeza,mf_cabeza);
        }
        if(seleccion == 2){
            imprimir_mv(mv_cabeza);
            espera(1);
            imprimir_mf(mf_cabeza);
        }
        else
        printf("\nHAIGA LAS COSAS VIEN NO CEA MENZO");
    }while(seleccion!=3);
    //printf ("\nSe borra la lista cargada\n");
    //borrarLista (&cabeza);
    return 0;
}

//FUNCIONES CREADAS ------------------------------------------------------------------------

//FUNCIONES PARA LISTAS
void insertar (punterolista *cabeza, int e){
    punterolista nuevo; //Creamos un nuevo nodo
    nuevo = malloc(sizeof(lista)); //Utilizamos malloc para reservar memoria para ese nodo
    nuevo->my_indice = e; //Le asignamos el valor ingresado por pantalla a ese nodo
    nuevo->estado = 1;
    nuevo->sig_list = *cabeza; //Le asignamos al siguiente el valor de cabeza
    *cabeza = nuevo; //Cabeza pasa a ser el ultimo nodo agregado
}

//IMPRIMIR EN PANTALLA
void imprimir_mv(punterolista cabeza){
    printf("Indice\tMV\t\tMF\t\tESTADO\n");
    while(cabeza != NULL){ //Mientras cabeza no sea NULL
        printf("%d\t",cabeza->my_indice);
        binarioPagina(cabeza->my_indice);
        printf(" / %d", cabeza->my_indice);
        printf("\t");
        binarioMarcos(cabeza->its_indice);
        printf(" / %d",cabeza->its_indice);
        printf("\t\t %d",cabeza->estado);
        printf("\n");
        cabeza = cabeza->sig_list; //Pasamos al siguiente nodo
    }
}
void imprimir_mf(punterolista cabeza){
    printf("Indice\tMV\t\tMF\t\tESTADO\n");
    while(cabeza != NULL){ //Mientras cabeza no sea NULL
        printf("%d\t",cabeza->my_indice);
        binarioMarcos(cabeza->my_indice);
        printf(" / %d", cabeza->my_indice);
        printf("\t\t");
        binarioPagina(cabeza->its_indice);
        printf(" / %d",cabeza->its_indice);
        printf("\t\t %d",cabeza->estado);
        printf("\n");
        cabeza = cabeza->sig_list; //Pasamos al siguiente nodo
    }
}
/*void borrarLista(tpuntero *cabeza){
tpuntero actual; //Puntero auxiliar para eliminar correctamente la lista
while(*cabeza != NULL){ //Mientras cabeza no sea NULL
actual = *cabeza; //Actual toma el valor de cabeza
*cabeza = (*cabeza)->sig; //Cabeza avanza 1 posicion en la lista
free(actual); //Se libera la memoria de la posicion de Actual (el primer nodo), y cabeza
queda apuntando al que ahora es el primero
}
}
*/

//FUNCION DE CONVERSION DE DECIAMAL A BINARIO CREDITOS: EJEMPLO1
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

//FUNCIONES ESTETICAS
void espera(int i){
    while(i){
        printf(".");
        sleep(i);
        i--;
    }
    printf("\n");
}

void ocupacion(int mv, int mf, punterolista aux_mv_cabeza, punterolista aux_mf_cabeza){
    //printf("\nSi entro a ocuapcion");
    int i;
    //punterolista *aux_mv_cabeza = m:v_cabeza;
    //punterolista *aux_mf_cabeza = mf_cabeza;
    while(aux_mv_cabeza!= NULL){
        if(aux_mv_cabeza->my_indice == mv){
            while(aux_mf_cabeza != NULL){
                if(aux_mf_cabeza->my_indice == mf){
                    aux_mv_cabeza->its_indice = aux_mf_cabeza->my_indice;
                    aux_mf_cabeza->its_indice = aux_mv_cabeza->my_indice;
                    aux_mv_cabeza->estado = 0;
                    aux_mf_cabeza->estado = 0;
                    //printf("\n%d %d %d",aux_mv_cabeza->my_indice, aux_mv_cabeza->its_indice,
                    aux_mv_cabeza->estado);
                    //printf("\n%d %d %d",aux_mf_cabeza->my_indice, aux_mf_cabeza->its_indice,
                    aux_mf_cabeza->estado);
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
