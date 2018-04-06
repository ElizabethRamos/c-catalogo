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
        if(resp == '1')
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

int str_somente_numeros(char str[])
{
    int i = 0;
    int len_str = strlen(str);

    for(i = 0; i < len_str; i++)
    {
        if(str[i] < '0' || str[1] > '9')
            return 0;
    }
    return 1;
}

void cadastrar_filme()
{
	FILE *arq_filmes = fopen("filmes.bin", "a+b");

	if(arq_filmes == NULL)
	{
		printf("\nFalha ao abrir o arquivo!\n");
		exit(1);
	}

	t_filme filme;

	int cont_bytes = 0;

	// seta o ponteiro do arquivo para o final do arquivo
	fseek(arq_filmes, 0, SEEK_END);

	// guarda o n�mero total de bytes
	cont_bytes = ftell(arq_filmes);

	if(cont_bytes == 0)
	{
		filme.id = 1;
	}
	else
	{
		t_filme ultimo_filme;

		fseek(arq_filmes, cont_bytes - sizeof(t_filme), SEEK_SET);

		// ler o filme
		fread(&ultimo_filme, sizeof(t_filme), 1, arq_filmes);

		filme.id = ultimo_filme.id + 1;
	}

	// obt�m o nome do filme
	printf("\nDigite o nome do filme: ");
	scanf("%99[^\n]%*c", filme.nome);

	fseek(stdin, 0, SEEK_END);

	do
	{
		char str_preco[5];
		float float_preco;
		int somente_numeros = 1;

		// obt�m o pre�o do filme
		printf("Digite o preco do filme: ");
		scanf("%4s%*c", str_preco);

		fseek(stdin, 0, SEEK_END);

		// verifica se o pre�o possui somente n�meros
		somente_numeros = str_somente_numeros(str_preco);

		if(somente_numeros == 1)
		{
			int int_preco;

			// exemplo: 49,10 deve digitar 4910
			sscanf(str_preco, "%d", &int_preco);
			float_preco = int_preco / 100.0;
			filme.preco = float_preco;
			break;
		}
	}
	while(1);

	// inicializa o id do cliente com -1 para indicar que o filme n�o est� alugado
	filme.id_cliente = -1;

	fseek(arq_filmes, 0, SEEK_END);

	// escreve no arquivo
	fwrite(&filme, sizeof(t_filme), 1, arq_filmes);

	// fecha o arquivo
	fclose(arq_filmes);

	printf("\nFilme \"%s\" cadastrado com sucesso!\n", filme.nome);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END);
}

