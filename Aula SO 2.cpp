// AULA SO.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

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
HANDLE secaoCritica_1;
HANDLE secaoCritica_video;
vector<HANDLE> idThread; //dinamico
HANDLE idThread_1[10];   //estatico
vector<PARAMETRO> vetorParametro;
HANDLE flagEvento;

int soma = 0;
int soma2 = 0;

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
    secaoCritica_1 = CreateMutex(NULL, FALSE, NULL);
    secaoCritica_video = CreateMutex(NULL, FALSE, NULL);

    flagEvento = CreateEvent(NULL, TRUE, FALSE, NULL); //se é falso fica parado no wait

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
    //Boas práticas, fechar handles para que não ocorra erros
    tfinal = time(0);
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln


void minhaThread(void)
{

}
void minhaThread1(void* parametroFuncao) //PARA CADA THREAD CRIADA SÃO POSIÇÕES DE MEMÓRIA DIFERENTES    ------ Vão executar concorrentemente
{
    PARAMETRO* parDentroThread = (PARAMETRO*)parametroFuncao;
    int cont = 0;
    //seção critica serializa as instruções
    while (cont < 1000)
    {
        WaitForSingleObject(secaoCritica, INFINITE); //inicio seção critica  --- serve para acabar com a concorrência --- encontra-se nela acesso de escrita de variáveis globais --acesso a uma variável compartilhada por threads
        //como funcionaria mais de 1 seção crítica
        soma++;
        ReleaseMutex(secaoCritica); //fim seção critica
        cont++;
    }
   
    WaitForSingleObject(secaoCritica_1, INFINITE); //wait vai para o kernel    ---pode ocorrer deadlock se não ter feito o release
    soma2++;
    ReleaseMutex(secaoCritica_1);

    /*
   while (true)
   {
       WaitForSingleObject(secaoCritica_video, INFINITE); //inicio seção critica  --- serve para acabar com a concorrência --- encontra-se nela acesso de escrita de variáveis globais --acesso a uma variável compartilhada por threads
       //como funcionaria mais de 1 seção crítica
       soma++;
       ReleaseMutex(secaoCritica); //fim seção critica
       cont++;
       WaitForSingleObject(secaoCritica_1, INFINITE); //wait vai para o kernel    ---pode ocorrer deadlock se não ter feito o release
       soma2++;
       ReleaseMutex(secaoCritica_1);
   }


   */
    //seção critica e mutex garante organização de acesso
    //se usar a mesma variável vão concorrer pelo mesmo acesso, o ideal é usar seções críticas diferentes
    //usar 1 única variável deseção crítica para saída de vídeo
    //se usar a mesma varíavel na seção crítica
    //não compensa multiplas threads se a seção críticas estiver em um loop
    //evento é para controlar as threads ao mesmo tempo

    _endthread();
}