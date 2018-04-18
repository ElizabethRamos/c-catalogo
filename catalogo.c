#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

/**
 * Convert string s to uppercase
 */
char* strupr(char* s)
{
  char* p = s;
  while(*p) {
    *p = toupper(*p);
		++p;
	}
  return s;
}

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
        else if(resp == '2')
            cadastrar_cliente();
        else if(resp == '3')
            listar_filmes();
        else if(resp == '4')
            listar_clientes();
        else if(resp == '5')
            pesquisar_filme();
        else if(resp == '6')
            pesquisar_cliente();
        else if(resp == '7')
            alugar_filme();
        else if(resp == '0')
            break;
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

void cadastrar_cliente()
{
    //abre um arquivo binario para leitura e escrita (a*b acrescenta dados ao final do arquivo, ou o cria se ele nao existir)
    FILE *arq_clientes = fopen("clientes.bin", "a+b");

    //teste a abertura do arquivo
    if(arq_clientes == NULL)
    {
        printf("\nFalha ao abrir o arquivo!\n");
        exit(1);
    }

    //se abrir...
    t_cliente cliente;

    int cont_bytes = 0;

    //seta o ponteiro para o final do arquivo
    fseek(arq_clientes, 0, SEEK_END);

    //pega a quantidade de bytes
    cont_bytes = ftell(arq_clientes);

    //se cont bytes for igual a zero, nao existe cliente cadastrado, entao atribuimos o 1o id
    if(cont_bytes == 0)
    {
        cliente.id = 1;
    }
    else
    {
        t_cliente ultimo_cliente;

        //posiciona o arquivo para pegar o ultimo cliente
        fseek(arq_clientes, cont_bytes - sizeof(t_cliente), SEEK_SET);

        //le o ultimo cliente e atribui o id do proximo cliente
        fread(&ultimo_cliente, sizeof(t_cliente), 1, arq_clientes);

		cliente.id = ultimo_cliente.id + 1;
    }
    printf("\nDigite o nome do cliente: ");

    scanf("%99[^\n]%*c", cliente.nome);

    //limpa o buffer
    fseek(stdin, 0, SEEK_END);

    //escreve o nome do cliente no arquivo
    fwrite(&cliente, sizeof(t_cliente), 1, arq_clientes);

    //fecha o arquivo
    fclose(arq_clientes);

    printf("\nCliente \"%s\" cadastrado com sucesso!\n", cliente.nome);
    printf("\nPressione <enter> para continuar...");
    scanf("%*c");

    //limpa o buffer
    fseek(stdin, 0, SEEK_END);
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

void listar_filmes()
{
  //abre os arquivos binarios em modo de leitura
  FILE *arq_filmes = fopen("filmes.bin", "rb");
  FILE *arq_clientes = fopen("clientes.bin", "rb");

  if(arq_filmes == NULL)
  {
    printf("\nFalha ao abrir o arquivo ou ");
    printf("Não há filmes cadastrados.\n");
    printf("\nPressione <enter> para continuar...");
    scanf("%*c");

    //limpa o buffer
    fseek(stdin, 0, SEEK_END);
    return;
  }

  int encontrou_filmes = 0;

  printf("\nListando todos os filmes...\n");

  t_filme filme;

  //loop para pegar todos os filmes
  while(1)
  {
      //atribui a variavel result  o numeto de filmes lidos com sucesso
      size_t result = fread(&filme, sizeof(t_filme), 1, arq_filmes);

      if(result == 0)
          break;

      encontrou_filmes = 1;

      printf("\nID do filme: %d\n", filme.id);
      printf("Nome do filme: %s\n", filme.nome);
      printf("Preço: %.2f\n", filme.preco);

      //se for diferente de -1, entao o filme esta alugado...
      if(filme.id_cliente != -1)
      {
          if(arq_clientes == NULL)
          {
              printf("\nFalha ao abrir o arquivo!n");
              fclose(arq_filmes);
              exit(1);
          }
          //... entao precisamos dos dados do cliente que alugou o filme :)
          t_cliente *cliente = obter_cliente(arq_clientes, filme.id_cliente);
          printf("Alugado? Sim. Cliente: %s\n", cliente->nome);
          free(cliente);
      }
      else
          printf("Alugado? Nao\n");
  }

  //verifica se encontrou pelo menos um filme
  if(encontrou_filmes == 0)
      printf("\nNao ha filmes cadastrados\n");

  if(arq_clientes != NULL)
        fclose(arq_clientes);

  fclose(arq_filmes);

  printf("\nPressione <enter> para continuar...");
  scanf("%*c");

  //limpa o buffer
  fseek(stdin, 0, SEEK_END);
}

t_cliente *obter_cliente(FILE *arq_clientes, int id_cliente)
{
  //vai para o inicio do arquivo
  rewind(arq_clientes);

  t_cliente *cliente;

  cliente = (t_cliente *)malloc(sizeof(t_cliente));

  while(1)
  {
    //atribui a variavel result  o numeto de clientes lidos com sucesso
    size_t result = fread(cliente, sizeof(t_cliente), 1, arq_clientes);

    if(result == 0)
    {
      free(cliente);
      return NULL;
    }

    if(cliente->id == id_cliente)
        break;
  }

  return cliente;
}

void listar_clientes()
{
  //abre o arquivo binario para leitura
  FILE *arq_clientes = fopen("clientes.bin", "rb");

  // se o arquivo estiver sem nada..
  if(arq_clientes == NULL)
  {
    printf("\nFalha ao abrir o arquivo ou");
    printf("Nenhum cliente encontrado.\n");
    printf("\nPressione <enter> para continuar");
    scanf("%*c");

    fseek(stdin, 0, SEEK_END);
    return;
  }

  //se o arquivo for aberto...
  int encontrou_clientes = 0;
  t_cliente cliente;

  printf("\nListando todos os clientes\n");

  while(1)
  {
    //atribui para a variavel result o numero de elementos lidos com sucesso
    size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);

    if(result == 0)
        break;

    encontrou_clientes = 1;

    printf("\nID do cliente: %d\n", cliente.id);
    printf("\nNome do cliente: %s\n", cliente.nome);
  }
  //se nao encontrar cliente...
  if(encontrou_clientes == 0)
      printf("\nNenhum cliente cadastrado.\n");

  //fecha o arquivo que foi aberto
  fclose(arq_clientes);

  //e segue!
  printf("\nPressione <enter> para continuar");
  scanf("%*c");

  //limpa o buffer de entrada
  fseek(stdin, 0, SEEK_END);
}

void pesquisar_filme()
{
	char nome[MAX];
	int encontrou_filme = 0;

	FILE *arq_filmes = fopen("filmes.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");

	if(arq_filmes == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}

	printf("\nDigite o nome do filme: ");
	scanf("%99[^\n]%*c", nome);

	printf("\nFilmes com o nome \"%s\":\n\n", nome);

	t_filme filme;

	while(1)
	{
		size_t result = fread(&filme, sizeof(t_filme), 1, arq_filmes);

		if(result == 0)
			break;

		char nome_aux[MAX];

		strcpy(nome_aux, filme.nome);

		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			printf("ID do filme: %d\n", filme.id);
			printf("Preco: %.2lf\n", filme.preco);

			if(filme.id_cliente != -1)
			{
				if(arq_clientes == NULL)
				{
					printf("\nFalha ao abrir arquivo!\n");
					fclose(arq_filmes);
					exit(1);
				}

				t_cliente *cliente = obter_cliente(arq_clientes, filme.id_cliente);

				printf("Alugado? Sim. Cliente: %s\n", cliente->nome);
				free(cliente);
			}
			else
				printf("Alugado? Nao\n");
			encontrou_filme = 1;
			printf("\n");
		}
	}

	if(encontrou_filme == 0)
		printf("Nenhum filme encontrado.\n\n");

	fclose(arq_filmes);

	printf("Pressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END);
}

void pesquisar_cliente()
{
  char nome[MAX];
  int encontrou_cliente = 0;

  //abre o arquivo
  FILE *arq_clientes = fopen("clientes.bin", "rb");

  //testa a abertura do arquivo
  if(arq_clientes == NULL)
  {
    printf("Falha ao abrir o arquivo\n");
    exit(1);
  }

  //pede para digitar o nome do cliente
  printf("\nDigite o nome do cliente: ");
  scanf("%99[^\n]%*c", nome);

	printf("\nClientes com o nome \"%s\":\n\n", nome);

  t_cliente cliente;

  while(1)
  {
    //retorna a quantidade de arquivos lidos
    size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);

    if(result == 0)
      break;

    char nome_aux[MAX];

    strcpy(nome_aux, cliente.nome);

    //verifica se nome_aux é igual a nome
    if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
    {
      //mostra os dados do cliente
      printf("ID do cliente: %d\n\n", cliente.id);
      encontrou_cliente = 1;
    }
  }

  if(encontrou_cliente == 0)
    printf("Nenhum cliente encontrado.\n\n");

    fclose(arq_clientes);

  	printf("Pressione <Enter> para continuar...");
  	scanf("%*c");

  	fseek(stdin, 0, SEEK_END);
}

void alugar_filme()
{
  char str_id_cliente[10];
  int id_cliente;

  //abre os arquivos
  FILE *arq_filmes = fopen("filmes.bin", "rb");
	FILE *arq_clientes = fopen("clientes.bin", "rb");

//se or arquivos nao existirem, cria os arquivos
  if(arq_filmes == NULL)
  {
      arq_filmes = fopen("filmes.bin", "wb+");
      if(arq_filmes == NULL)
      {
        printf("\nFalha ao abrir o arquivo");
        exit(1); // aborta
      }
  }

  if(arq_clientes == NULL)
  {
      arq_clientes = fopen("clientes.bin", "wb+");
      if(arq_clientes == NULL)
      {
        printf("\nFalha ao abrir o arquivo");
        exit(1); // aborta
      }
  }

  //obtem o id do cliente
  printf("\nDigite o ID do cliente ");
  scanf("%10s%*c", str_id_cliente);

  fseek(stdin, 0, SEEK_END);

  //verifica se o id tem somente_numeros
  if(str_somente_numeros(str_id_cliente) == 1)
  {
    //preenche a variavel id_cliente com o valor de str_id_cliente
      sscanf(str_id_cliente, "%d", &id_cliente);

      //verifica se o cliente existe
      if(existe_cliente(arq_clientes, id_cliente))
      {
          char str_id_filme[10];
          int id_filme;

          //pega o id do filme a ser aligado
          printf("\nDigite o ID do filme: ");
          scanf("%10s%*c", str_id_filme);

          fseek(stdin, 0, SEEK_END);

          if(str_somente_numeros(str_id_filme) == 1)
          {
            //preenche a variavel id_filme com o valor de str_id_filme
            sscanf(str_id_filme, "%d", &id_filme);

            //obtem o filme pelo id
            t_filme *filme = obter_filme(arq_filmes, id_filme);

            //testa se o filme existe
            if(filme != NULL)
            {   //testa se o filme foi alugado
                if(filme->id_cliente != -1)
                    printf("\n O filme \"%s\" ja esta alugado\n", filme->nome);
                else
                {
                  filme->id_cliente = id_cliente;
                  atualizar_filmes(arq_filmes, filme);
                  printf("\nFilme  \"%s\" alugado com sucesso\n", filme->nome);
                }
                free(filme);
            }
            else
              printf("\nNao existe filme com o ID \"%d\".\n", id_filme);
          }
          else
            printf("\no ID só pode conter numeros\n");
      }
        else
          printf("\nNao existe cliente com o ID \"%d\".\n", id_cliente);
  }
    else
      printf("\nID so pode conter numeros\n");

      fclose(arq_clientes);
      fclose(arq_filmes);

      printf("\nPressione <enter> para continuar...");
      scanf("%*c");

      //limpa o buffer
      fseek(stdin, 0, SEEK_END);
}

int existe_cliente(FILE *arq_clientes, int id_cliente)
{
  //vai para o inicio do arquivo
  rewind(arq_clientes);

  t_cliente cliente;

  while (1)
  {
    //atribui a variavel result  o numeto de filmes lidos com sucesso
    size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_clientes);

    if(result == 0)
        break;
    //se o id for igual, retornamos 1, indicando que o cliente existe
    if(cliente.id == id_cliente)
        return 1;
  }

  return 0;
}

t_filme *obter_filme(FILE *arq_filmes, int id_filme)
{
  //vai para o incio do arquivo
  rewind(arq_filmes);

  t_filme *filme;

  //aloca a variavel dinamicamente
  filme = (t_filme *)malloc(sizeof(t_filme));

  while (1)
  {
    //atribui a variavel result  o numeto de filmes lidos com sucesso
    size_t result = fread(&filme, sizeof(t_filme), 1, arq_filmes);

    //se o arquivo nao tiver nada, sai do loop
    if(result == 0)
        break;
    //se os ids forem iguais, retorna o filme
    if(filme->id == id_filme)
          return filme;
  }
  //libera a memoria
  free(filme);
  return NULL;
}
