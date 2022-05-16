#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#include <math.h>
#include <vector>

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
vector<HANDLE> idThread; //dinamico
HANDLE idThread_1[10];   //estatico
vector<PARAMETRO> vetorParametro;
HANDLE flagEvento;


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

    flagEvento = CreateEvent(NULL, TRUE, FALSE, NULL);

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
        ResumeThread(idThread[i]);

    getchar();

    SuspendThread(idThread[0]);

    getchar();
    */

    WaitForMultipleObjects(n_threads, idThread.data(), TRUE, INFINITE);


    tfinal = time(0);
}

void minhaThread(void)
{

}
void minhaThread1(void* parametroFuncao) //PARA CADA THREAD CRIADA S�O POSI��ES DE MEM�RIA DIFERENTES    ------ V�o executar concorrentemente
{
    PARAMETRO* parDentroThread;
    parDentroThread = (PARAMETRO*)parametroFuncao;

    WaitForSingleObject(secaoCritica, INFINITE); //inicio se��o critica  --- serve para acabar com a concorr�ncia

    cout << "id da thread" << parDentroThread->id << endl;

    ReleaseMutex(secaoCritica); //fim se��o critica

    while (true)
    {
        cout << "Estou em execucao" << endl;
    }

    _endthread();
}