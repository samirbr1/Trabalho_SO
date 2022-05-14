#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <process.h>
//#include <windows.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <cstring>


#define INICIO(X) clock_t X = clock()       // macros para calculo do tempo
#define FIM(X) printf("%s: %g segundos para contagem de primos.\n", (#X), (double) (clock() - (X)) / CLOCKS_PER_SEC)

using namespace std;

int matriz[10100][10100];
int contador = 0;


void checkPrimo(int n);
void isPrime(int n);

int main()
{
    //srand(15687);
    srand(88773);
    int i, j;
    for(i = 0; i < 10100; i++)
        for(j = 0; j < 10100; j++)
        {
            matriz[i][j] = rand() % 99999999;
        }
        
    /*
    for(i = 0; i < 10000; i++)
    {
        for(j = 0; j < 10000; j++)
        {
            cout << "[" << matriz[i][j] << "]";
        }
        cout << endl;
    }
    */
    
    INICIO(AlgoritmoSerial);
    for(i = 0; i < 10100; i++)
    {
        for(j = 0; j < 10100; j++)
        {
            checkPrimo(matriz[i][j]);
        }
    }
    cout << "A quantidade de numeros primos eh " << contador << endl;
    FIM(AlgoritmoSerial);
}

void checkPrimo(int n)
{
    if (n == 2 || n == 3)
        contador++;

    int raizNum = sqrt(n);

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return;

    /*for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return;*/
    
     for (int i = 5;  i <= raizNum; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return;

    contador++; 
}
