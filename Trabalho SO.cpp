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


#define INICIO(X) clock_t X = clock()                                                // macros para calculo do tempo
#define FIM(X) printf("%s: %g segundos para contagem de primos.\n", (#X), (double) (clock() - (X)) / CLOCKS_PER_SEC)

#define LINHAS 10100
#define COLUNAS 10100

using namespace std;

//int matriz[linhas][colunas];
vector<vector<int>> matriz(LINHAS, vector<int>(COLUNAS));
int contador = 0;


void checkPrimo(int n);
void preencheMatriz();
void contaPrimos();

void main()
{
    preencheMatriz();
    contaPrimos();


}

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

void contaPrimos()
{
    int i, j;

    INICIO(AlgoritmoSerial);

    for (i = 0; i < LINHAS; i++)
        for (j = 0; j < COLUNAS; j++)
            checkPrimo(matriz[i][j]);

    FIM(AlgoritmoSerial);

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
}