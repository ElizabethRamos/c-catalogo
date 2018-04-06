#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// quantidade m�xima de letras do nome do cliente e do filme
#define MAX 100

// defini��o da struct do filme
typedef struct filme
{
	// campos da struct filme
	int id;
	int id_cliente;
	char nome[MAX];
	float preco;
} t_filme;

// defini��o da struct cliente
typedef struct cliente
{
	// campos da struct cliente
	int id;
	char nome[MAX];
} t_cliente;

// prot�tipos de fun��es

char menu(); // exibe as op��es do programa
void cadastrar_cliente(); // cadastra um cliente pelo nome
void cadastrar_filme(); // cadastra um filme com o nome e pre�o
void listar_filmes(); // exibe todos os filmes cadastrados e as info. de aluguel
void listar_clientes(); // exibe todos os clientes cadastrados
t_filme *obter_filme(FILE *arq_filmes, int id_filme); // procuma um filme pelo id
t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente); // procura um cliente pelo id
void pesquisar_filme(); // pesquisa por um filme e exibe suas informa��es
void pesquisar_cliente(); // pesquisa por um cliente e exibe suas informa��es
int existe_filme(FILE *arq_filmes, int id_filme); // procura um filme pelo seu id
int existe_cliente(FILE *arq_clientes, int id_cliente); // procura um cliente pelo seu id
void atualizar_filmes(FILE *arq_filmes, t_filme *filme_alugado); // atualiza as info. do filme
void alugar_filme(); // aluga um filme
void excluir_filme(); // exclui um filme
int str_somente_numeros(char str[]); // informa se a string � um n�mero

int main(int argc, char *argv[])
{
	char resp;

	while(1)
	{
		// obt�m a escolha do usu�rio
		resp = menu();

		// testa o valor de resp
		if(resp == '1')
			cadastrar_filme();
		else
		{
			printf("\nOpcao invalida! Pressione <Enter> para continuar...");
			scanf("%*c");

			// uma forma de limpar o buffer de entrada
			fseek(stdin, 0, SEEK_END);
		}

		system(limpar_tela);
	}

	printf("\nBye! Curso de Linguagem C para Iniciantes ;-)\n");

	return 0;
}

char menu()
{
	char resp[2];

	printf("                       Catálogo de filmes                         ");
	printf("\n\n1 - Cadastrar um filme\n");
	printf("2 - Cadastrar um cliente\n");
	printf("3 - Listar todos os filmes\n");
	printf("4 - Listar todos os clientes\n");
	printf("5 - Pesquisar por filme\n");
	printf("6 - Pesquisar por cliente\n");
	printf("7 - Alugar um filme\n");
	printf("8 - Entregar um filme\n");
	printf("9 - Excluir um filme\n");
	printf("0 - Sair\n");
	printf("Digite o numero da opcao: ");
	scanf("%1s%*c", resp);

	fseek(stdin, 0, SEEK_END);

	// se chegou aqui, � porque a op��o � v�lida
	return resp[0];
}

int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);

	for(i = 0; i < len_str; i++)
	{
		if(str[i] < '0' || str[i] > '9')
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
