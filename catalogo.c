#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

//struct filme
typedef struct filme 
{
    int id;
    int id_cliente;
    char nome[MAX];
    float preco;
}t_filme;

//struct cliente
typedef struct cliente
{
    int id;
    char nome[MAX];
};

int main()
{
    return 0;
}