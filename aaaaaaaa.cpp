#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef struct {
    int id;
    float retornoCalculo1;
    float retornoCalculo2;
} PARAMETRO;


void minhaThread(void*);
void minhaThread1(void* parametroFuncao);

//uma fun��o para cada thread, mas se todas as threads fazem a mesma coisa, ent�o podemos usar a mesma fun��o, mas com par�metros diferentes
//multithread executando a fun��o em paralelo

//vector � uma classe com m�todos
//m�todo � a mesma coisa que fun��o
//objeto � uma c�pia de uma classe

//Handle para controlar acesso a regi�o cr�tica de c�digo


HANDLE secaoCritica;
HANDLE secaoCritica_1;
HANDLE secaoCritica_video;
vector<HANDLE> idThread; //dinamico
HANDLE idThread_1[10];   //estatico
vector<PARAMETRO> vetorParametro;
HANDLE flagEvento;

int matriz[10100][10100];
int contador = 0;

//HANDLE hMutex_01;
//HANDLE hMutex_02;

            //HANDLE hfinalizaThread;

/*Cria��o de um vetor de par�metros para evitar dar BO por conta da passagem por refer�ncia
Thread bloqueada, */
void main()
{
    time_t tinicial, tfinal;
    int n_threads = 8;
    PARAMETRO addVetor;

    secaoCritica = CreateMutex(NULL, FALSE, NULL);
    secaoCritica_1 = CreateMutex(NULL, FALSE, NULL);
    secaoCritica_video = CreateMutex(NULL, FALSE, NULL);

    flagEvento = CreateEvent(NULL, TRUE, FALSE, NULL); //se � falso fica parado no wait

    tinicial = time(0);
    for (int i = 0; i < n_threads; i++)
    {
        addVetor.id = i;
        addVetor.retornoCalculo1 = 0;
        addVetor.retornoCalculo2 = 0;
        vetorParametro.push_back(addVetor);

    }
    for (int i = 0; i < n_threads; i++)
    {
        idThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&minhaThread1, &vetorParametro[i], 1, NULL));
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&minhaThread1, &vetorParametro[i], 0, NULL); //0 cria em execu��o 
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&minhaThread1, &vetorParametro[i], 1, NULL); //1 cria em bloqueado
        //Se for criar uma unica thread n�o precisa de um vetor
    }

    cout << "Threads suspensas" << endl;


    getchar();

    /*
    for (int i = 0; i < n_threads; i++)
    {
        ResumeThread(idThread[i]);
    }
    getchar();

    SuspendThread(idThread[0]);

    getchar();
    */

    /*

    */
    WaitForMultipleObjects(n_threads, idThread.data(), TRUE, INFINITE);


    CloseHandle(secaoCritica);
    CloseHandle(flagEvento);

    for (int i = 0; i < n_threads; i++)
    {
        CloseHandle(idThread[i]); //fechar vetores, cada indice do vetor
    }
    //Boas pr�ticas, fechar handles para que n�o ocorra erros
    tfinal = time(0);
}



void minhaThread(void)
{

}
void minhaThread1(void* parametroFuncao) //PARA CADA THREAD CRIADA S�O POSI��ES DE MEM�RIA DIFERENTES    ------ V�o executar concorrentemente
{
    PARAMETRO* parDentroThread = (PARAMETRO*)parametroFuncao;
    int cont = 0;
    //se��o critica serializa as instru��es
    while (cont < 1000)
    {
        WaitForSingleObject(secaoCritica, INFINITE); //inicio se��o critica  --- serve para acabar com a concorr�ncia --- encontra-se nela acesso de escrita de vari�veis globais --acesso a uma vari�vel compartilhada por threads
        //como funcionaria mais de 1 se��o cr�tica
        soma++;
        ReleaseMutex(secaoCritica); //fim se��o critica
        cont++;
    }

    WaitForSingleObject(secaoCritica_1, INFINITE); //wait vai para o kernel    ---pode ocorrer deadlock se n�o ter feito o release
    soma2++;
    ReleaseMutex(secaoCritica_1);

    /*
   while (true)
   {
       WaitForSingleObject(secaoCritica_video, INFINITE); //inicio se��o critica  --- serve para acabar com a concorr�ncia --- encontra-se nela acesso de escrita de vari�veis globais --acesso a uma vari�vel compartilhada por threads
       //como funcionaria mais de 1 se��o cr�tica
       soma++;
       ReleaseMutex(secaoCritica); //fim se��o critica
       cont++;
       WaitForSingleObject(secaoCritica_1, INFINITE); //wait vai para o kernel    ---pode ocorrer deadlock se n�o ter feito o release
       soma2++;
       ReleaseMutex(secaoCritica_1);
   }


   */
   //se��o critica e mutex garante organiza��o de acesso
   //se usar a mesma vari�vel v�o concorrer pelo mesmo acesso, o ideal � usar se��es cr�ticas diferentes
   //usar 1 �nica vari�vel dese��o cr�tica para sa�da de v�deo
   //se usar a mesma var�avel na se��o cr�tica
   //n�o compensa multiplas threads se a se��o cr�ticas estiver em um loop
   //evento � para controlar as threads ao mesmo tempo

    _endthread();
}