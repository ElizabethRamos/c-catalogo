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
}t_cliente;

//prototipos de funcoes
char menu();
void cadastrar_cliente();
void cadastrar_filme();
void listar_filmes();
void listar_clientes();
t_filme *obter_filme(FILE *arq_filmes, int id_filme);
t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente);
void pesquisar_filme();
void pesquisar_cliente();
int existe_filme(FILE *arq_filmes, int id_filme);
int existe_cliente(FILE *arq_clientes, int id_cliente);
void atualizar_filmes(FILE *arq_filmes, t_filme *filme_alugado);
void alugar_filme();
void excluir_filme();
int str_somente_numeros(char str[]);

int main()
{   
    char resp;

    while(1)
    {
        //obtem a escolha do usuario
        resp = menu();

        //testa o valor de resp
        if(resp == 1)
            cadastrar_filme();
        else
        {
            printf("\nOpção inválida, pressione <enter> para continuar");
            scanf("%*c");

            //limpa o buffer de entrada
            fseek(stdin, 0, SEEK_END);
        }
        system("clear");    
    }

    printf("\nBye");

    return 0;
}

char menu()
{
    char resp[2];
    printf("                              Catálogo de filmes                              ");
    printf("\n\n1 - Cadastrar um filme\n");
    printf("\n\n2 - Cadastrar um cliente\n");
    printf("\n\n3 - Listar todos os filmes\n");
    printf("\n\n4 - Listar todos os clientes\n");
    printf("\n\n5 - Pesquisar filmes\n");
    printf("\n\n6 - Pesquisar clientes\n");
    printf("\n\n7 - Alugar um filme\n");
    printf("\n\n8 - Entregar um filme\n");
    printf("\n\n9 - Excluir um filme\n");
    printf("\n\n0 - Sair\n");
    printf("Digite o número da opção: ");
    scanf("%1s%*c", resp);

    fseek(stdin, 0, SEEK_END);

    //se chegou aqui, é porque a opção é valida
    return resp[0];

}

void cadastrar_filme()
{
    
}