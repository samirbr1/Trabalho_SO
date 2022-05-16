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

//uma função para cada thread, mas se todas as threads fazem a mesma coisa, então podemos usar a mesma função, mas com parâmetros diferentes
//multithread executando a função em paralelo

//vector é uma classe com métodos
//método é a mesma coisa que função
//objeto é uma cópia de uma classe

//Handle para controlar acesso a região crítica de código


HANDLE secaoCritica;
vector<HANDLE> idThread; //dinamico
HANDLE idThread_1[10];   //estatico
vector<PARAMETRO> vetorParametro;
HANDLE flagEvento;


//HANDLE hMutex_01;
//HANDLE hMutex_02;
//HANDLE hfinalizaThread;

/*Criação de um vetor de parâmetros para evitar dar BO por conta da passagem por referência
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
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&minhaThread1, &vetorParametro[i], 0, NULL); //0 cria em execução 
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&minhaThread1, &vetorParametro[i], 1, NULL); //1 cria em bloqueado
        //Se for criar uma unica thread não precisa de um vetor
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
void minhaThread1(void* parametroFuncao) //PARA CADA THREAD CRIADA SÃO POSIÇÕES DE MEMÓRIA DIFERENTES    ------ Vão executar concorrentemente
{
    PARAMETRO* parDentroThread;
    parDentroThread = (PARAMETRO*)parametroFuncao;

    WaitForSingleObject(secaoCritica, INFINITE); //inicio seção critica  --- serve para acabar com a concorrência

    cout << "id da thread" << parDentroThread->id << endl;

    ReleaseMutex(secaoCritica); //fim seção critica

    while (true)
    {
        cout << "Estou em execucao" << endl;
    }

    _endthread();
}