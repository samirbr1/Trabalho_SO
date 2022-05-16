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

#define INICIO(X) clock_t X = clock()                                                // macros para calculo do tempo
#define FIM(X) printf("%s: %g segundos para contagem de primos.\n", (#X), (double) (clock() - (X)) / CLOCKS_PER_SEC)

#define LINHAS 10100
#define COLUNAS 10100

typedef struct {
    int id;
    float retornoCalculo;
} PARAMETRO;

typedef struct {
    int id;
    int posicao;
};

//int matriz[linhas][colunas];
vector<vector<int>> matriz(LINHAS, vector<int>(COLUNAS));

//HANDLE idThread[6];
vector<HANDLE> idThread;
HANDLE secaoCritica;
vector<PARAMETRO> vetorParametro;
HANDLE flagEvento;

HANDLE hMutex;
HANDLE hfinalizaThread;

int contador = 0;

void myThread(void* parametroFuncao);
//void checkPrimo(int n);
//void preencheMatriz();
//void contaPrimos();

void main()
{
    int numeroThreads = 6;
    PARAMETRO addVetor;

    secaoCritica = CreateMutex(NULL, FALSE, NULL);
    flagEvento = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (int i = 0; i < numeroThreads; i++)
    {
        addVetor.id = i;
        addVetor.retornoCalculo = 0;
        vetorParametro.push_back(addVetor);
    }

    for (int i = 0; i < numeroThreads; i++)
    {
        idThread.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&myThread, &vetorParametro[i], 1, NULL));
                                                                                                       //0 cria em execução 
                                                                                                       //1 cria em bloqueado
    }

    WaitForMultipleObjects(numeroThreads, idThread.data(), TRUE, INFINITE);

    //preencheMatriz();
    //contaPrimos();

    CloseHandle(secaoCritica);
    CloseHandle(flagEvento);

    for (int i = 0; i < numeroThreads; i++)
    {
        CloseHandle(idThread[i]); //fechar vetores, cada indice do vetor
    }
}
/*
void checkPrimo(int n)
{
    int raizNum = sqrt(n);

    if (n == 2 || n == 3)
        contador++;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return;

    /*for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return;*/
/*
    for (int i = 5; i <= raizNum; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return;

    contador++;
}

void preencheMatriz()
{
    int i, j;
    srand(15687);

    for (i = 0; i < LINHAS; i++)
        for (j = 0; j < COLUNAS; j++)
            matriz[i][j] = rand() % 99999999;
}
/*
void contaPrimos()
{
    int i, j;

    INICIO(AlgoritmoMultithread);

    for (i = 0; i < LINHAS; i++)
        for (j = 0; j < COLUNAS; j++)
            checkPrimo(matriz[i][j]);

    FIM(AlgoritmoMultithread);

    cout << "A quantidade de numeros primos eh " << contador << endl;
}

void imprimeMatriz()
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
            cout << "[" << matriz[i][j] << "]";
        cout << endl;
    }
}*/

void myThread(void* parametroFuncao)
{
    PARAMETRO* parDentroThread;
    parDentroThread = (PARAMETRO*)parametroFuncao;

    WaitForSingleObject(secaoCritica, INFINITE); //inicio seção critica  --- serve para acabar com a concorrência

    cout << "id da thread" << parDentroThread->id << endl;

    ReleaseMutex(secaoCritica); //fim seção critica

    _endthread();
}