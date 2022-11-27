#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#define PERMISOS 0644
//#define N 5

int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
void Entra_al_bufer(char sesion);
char obten_sesion();
void escribe_hilo(int num, char mecom[], char ses);
int lee_hilo(char mecom[], char ses);
void *Hilo(void *arg);
void crea_Hilos(int num, char ses_nue);
int compara_sesion(char sesion);

char sesion_ser = 'a';
int disp = 20;
//int rest = 5;

int main()
{
    char ses_aux, sesion=sesion_ser;
    char command[]={'i','p','c','r','m',' ','-','a','\0'};
    system(command);
    int semaforo_mutex,semaforo_vacio,semaforo_lleno;
    key_t llave1;
    llave1=ftok("Prueba1",'k');
    down(semaforo_mutex);
    Entra_al_bufer(sesion);
    up(semaforo_mutex);
    while(1){
        down(semaforo_mutex);
        ses_aux = obten_sesion();
        crea_Hilos(compara_sesion(ses_aux), ses_aux);
        up(semaforo_mutex);
    }
}

int Crea_semaforo(key_t llave,int valor_inicial)
{
    int semid=semget(llave,1,IPC_CREAT|PERMISOS);
    if(semid==-1)
    {
        
    }
    semctl(semid,0,SETVAL,valor_inicial);
    return semid;
}

void down(int semid)
{
    struct sembuf op_p[]={0,-1,0};
    printf("Espere por favor\n");
    semop(semid,op_p,1);
}

void up(int semid)
{
    struct sembuf op_v[]={0,+1,0};
    semop(semid,op_v,1);
}

void Entra_al_bufer(char sesion)
{
    int i,memoria1;
    char *dato;
    key_t llave4;
    llave4=ftok("Prueba4",'n');
    memoria1=shmget(llave4,sizeof(char),IPC_CREAT|0600);
    dato=shmat(memoria1,0,0);
    for(i=0;i<3;i++)
    {
        //printf("\nProceso con pid=%d, en el bufer=%d\n",getpid(),i);
        sleep(1);
    }
    *dato=sesion;
    printf("\nSesion aÃ±adida: %c\n",*dato);
}
char obten_sesion()
{
    int i, memoria;
    char *dato;
    key_t llave;
    llave=ftok("Prueba4", 'n');
    memoria=shmget(llave, sizeof(char), IPC_CREAT|0600);
    dato=shmat(memoria,0,0);
    for(i=0;i<3;i++){
        sleep(1);
    }
    printf("\nSesion obtenida: %c\n", *dato);
    return *dato;
}

void escribe_hilo(int num, char mecom[], char ses)
{
    int memoria, *dato;
    key_t llaveH;
    llaveH=ftok(mecom,ses);
    memoria=shmget(llaveH,sizeof(int), IPC_CREAT|0600);
    dato=shmat(memoria,0,0);
    *dato=num;
    for(int i=0; i<2; i++){
        sleep(1);
    }
    printf("Se le ha escrito al cliente%d\n", num);
}

int lee_hilo(char mecom[], char ses)
{
    int memoria, *dato;
    key_t llaveH;
    llaveH=ftok(mecom,ses);
    memoria=shmget(llaveH,sizeof(int), IPC_CREAT|0600);
    dato=shmat(memoria,0,0);
    for(int i=0; i<2; i++){
        sleep(1);
    }
    printf("Se ha leido al cliente: %d\n", *dato);
    return *dato;
}

void *Hilo(void *arg)
{
    int i=0, mutex;
    char *sesion;
    sesion = (char *) arg;
    printf("Sesion obtenida por el hilo: %c\n", *sesion);
    char arch[]={*sesion,'\0'};
    char mecom[]={*sesion,'1','\0'};
    key_t llave;
    i=lee_hilo(mecom, *sesion);
    if((disp-i)<0){
        printf("No hay disponibilidad de boletos.");
        escribe_hilo(-1, mecom, *sesion);
        //pthread_exit(NULL);
    }else{
        printf("Si hay disponibilidad de boletos.");
        escribe_hilo(0, mecom, *sesion);
        disp-=i;
        printf("Se han vendido %d boletos; quedan %d\n", i, disp);
    }
    pthread_exit(NULL);
}

void crea_Hilos(int num, char ses_nue)
{
    printf("\nSe van a crear %d hilos para atender a los clientes\n", num);
    for(int i=0; i<num; i++){
        printf("Hilo nuevo con llave: %c\n", sesion_ser+i);
        pthread_t id_hilo;
        char s=sesion_ser+i;
        pthread_create(&id_hilo,NULL,Hilo,(void*)&s) ;
        pthread_join(id_hilo,NULL);
        sleep(1);
    }
    sesion_ser=ses_nue;
}

int compara_sesion(char sesion)
{
    int dif=0;;
    if(sesion == sesion_ser){
        return dif;
    }
    else if(sesion > sesion_ser){
        dif= sesion-sesion_ser;
        return dif;
    }
    printf("A ocurrido un error!");
    return -1;
}