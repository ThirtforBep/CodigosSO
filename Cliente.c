#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<locale.h>
#define PERMISOS 0644
#define N 5

int Crea_semaforo(key_t llave,int valor_inicial);
void down(int semid);
void up(int semid);
char Entra_al_bufer();
void escribe_hilo(int num, char mecom[], char ses);
int lee_hilo(char mecom[], char ses);

char sesion;
int main(){
    setlocale(LC_CTYPE,"spanish");
    int sel=1,bol,semaforo_mutex, mutex2;
    char conf='a';
    key_t llave1, llave2;
    llave1=ftok("Prueba1",'k');
    semaforo_mutex=Crea_semaforo(llave1,1);
    int semid=semget(llave2,1,IPC_CREAT|PERMISOS);
    while(sel == 1){
        printf("Bienvenido a Volaris\nPor favor ingresa el numero de boletos que deseas comprar:");
        scanf("%d", &bol);
        down(semaforo_mutex);
        sesion=Entra_al_bufer();
        up(semaforo_mutex);
        //Se crea un archivo vacio nuevo con touch para crear un nuevo semaforo con el hilo del servidor
        char comand[]= {'t','o','u','c','h',' ',sesion,'\0'};
        char comand2[]= {'t','o','u','c','h',' ',sesion,'1','\0'};
        char arch[]={sesion,'\0'};
        char mecom[]={sesion,'1','\0'};
        system(comand);
        system(comand2);
        escribe_hilo(bol, mecom, sesion);
        int i= lee_hilo(mecom, sesion);
        if(i == 0){
            printf("Solicitud Exitosa!\n Felicidades, has adquirido tus %d boletos\n", bol);
            sel = 0;
        }
        else if(i > 0){
            printf("Ha ocurrido un error, intentalo más tarde.\n");
            sel = 0;
        }
        else{
            printf("Lo sentimos, no hay disponilidad de tantos boletos en estos momentos.\n");
            exit(1);
        }
        //Se eliminan los archivos ocupados como canales y memoria compartida con el hilo
        char command3[]={'r','m',' ',sesion,'\0'};
        char command4[]={'r','m',' ',sesion,'1','\0'};
        system(command3);
        system(command4);
    }
}

int Crea_semaforo(key_t llave,int valor_inicial)
{
    int semid=semget(llave,1,IPC_CREAT|PERMISOS);
    if(semid==-1)
    {
        return -1;
    }
    semctl(semid,0,SETVAL,valor_inicial);
    return semid;
}

void down(int semid)
{
    struct sembuf op_p[]={0,-1,0};
    semop(semid,op_p,1);
}

void up(int semid)
{
    struct sembuf op_v[]={0,+1,0};
    semop(semid,op_v,1);
}

char Entra_al_bufer()
{
    int i,memoria1;
    char *dato, ses;
    key_t llave4;
    llave4=ftok("Prueba4",'n');
    memoria1=shmget(llave4,sizeof(char),IPC_CREAT|0600);
    dato=shmat(memoria1,0,0);
    ses=*dato;
    for(i=0;i<3;i++)
    {
        sleep(1);
    }
    printf("\nSesion del cliente con pid %d: %c\n", getpid(),*dato);
    *dato+=1;
    return ses;
}

void escribe_hilo(int num, char mecom[], char ses)
{
    printf("\nSe esta procesando tu solicitud");
    int memoria, *dato;
    key_t llaveH;
    llaveH=ftok(mecom,ses);
    memoria=shmget(llaveH,sizeof(int), IPC_CREAT|0600);
    dato=shmat(memoria,0,0);
    *dato=num;
    sleep(1);
    for(int i=0; i<3; i++){
        sleep(1);
        printf(".");
    }
}

int lee_hilo(char mecom[], char ses){
    int memoria, *dato;
    key_t llaveH;
    llaveH=ftok(mecom,ses);
    memoria=shmget(llaveH,sizeof(int), IPC_CREAT|0600);
    dato=shmat(memoria,0,0);
    for(int i=0; i<5; i++){
        sleep(1);
    }
    return *dato;
}
