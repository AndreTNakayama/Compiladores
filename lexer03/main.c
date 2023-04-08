#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000

// Criação dos Tokens
typedef enum {
	NUMERO,
	SOMA,
	SUBTRACAO,
	MULTIPLICACAO,
	DIVISAO,
	POTENCIA,
	INDETERMINADO,
	ERRO
} Token;

// Dados dentro do Token
typedef struct {
	Token tipo;
	char * valor;
} Info ;

// Função utilizada para verificar se é um número válido ou uma operação
int verificacao(char * palavra) {
	int v_Ponto = 0;
	int v_Virgula = 0;

	char numeros[12] = {'0','1','2','3','4','5','6','7','8','9','.',','};
	char simbolos[4] = {'+', '-', '*', '/'};

	for(int i = 0; i < strlen(palavra); i++){
		char v_Numero = 'F';
		
		if(palavra[i] == '.'){
			v_Ponto++;
		}

		if(palavra[i] == ','){
			v_Virgula++;
		}

		// Enquanto v_Numero for igual a 'T' é um número válido
		for(int j = 0; j < 12; j++){
			if (palavra[i] == numeros[j]){
				v_Numero = 'T';
			}
		}

		if(v_Numero == 'T'){
			continue;
		}
		else{
			// Nao é um numero, então vamos verificar se é uma operação
			for(int j = 0; j < 4; j++){
				if (palavra[i] == simbolos[j]) return 2;
			}
			
			return 0;
		}
	}

	// Se ele for um número, verifica se tem mais de 1 ponto ou mais de 1 vírgula
	if(v_Ponto <= 1 && v_Virgula <= 1)
		return 1;
	else
		return 0;
}

// Função responsável por retornar o Token com os dados
Info * lexer(FILE * file, int* count) {
	// Criação da variavel para controle dos Tokens
	Info *tokens = malloc(sizeof(Info));
  int tokens_count = 0;

	// Enquanto existir palavras para serem lidas dentro do arquivo de entrada
	while(!feof(file)){
		char palavra[MAX];
		int retorno;

		// Passa a palavra lida dentro do arquivo do tipo FILE para um vetor do tipo char
		int scan = fscanf(file, "%s", palavra);
		
		retorno = verificacao(palavra);

		// Se o retorno é 1 significa que é um número
		if(retorno == 1){
			// Contador de Token recebe +1, pois será adicionado um novo Token
			tokens_count++;
			// Realocamos memório pois temos um novo elemento a ser inserido
			tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));
			// Como o controle começa com a variável 0, temos que fazer o tokens_count - 1 para colocar o novo elemento na posição correta
			tokens[tokens_count - 1].tipo = NUMERO;
			tokens[tokens_count - 1].valor = malloc(sizeof(char) * (strlen(palavra) + 1));
			// Strcpy é utilizado para copiar strings
			strcpy(tokens[tokens_count - 1].valor, palavra);
		}

		// Se o retorno é 2, significa que é uma operação
		// No código abaixo vamos verificar a quantidade de asterisco (*), para validarmos se é uma operação de potenciação 
		if(retorno == 2){
			int tamanho = strlen(palavra);
			int cont_asterisco = 0;

			// Se chegarmos no final da string, verificamos a quantidade de * 
			for (int i = 0; i < tamanho; i++) {
				if (palavra[i] == '*'){
					cont_asterisco++;
					
					if(cont_asterisco == tamanho){
						// A mesma lógica utilizada quando o retorno é 1
						tokens_count++;
	      		tokens = realloc(tokens, sizeof(Info) * (tokens_count + 1));

						// Switch para definir qual operação equivale a quantidade de *
						if(cont_asterisco != 0){
							switch (cont_asterisco) {
								case 1:
									tokens[tokens_count - 1].tipo = MULTIPLICACAO;
									break;
								case 2:
									tokens[tokens_count - 1].tipo = POTENCIA;
									break;
								default:
									tokens[tokens_count - 1].tipo = ERRO;
							}
						}
					}
				}

				else if (palavra[i] == ' '){
					continue;
				} 

				// Se entrar no else, temos um operação que não envolve asterisco (*)
		    else {
					tokens_count++;
					tokens = realloc(tokens, sizeof(Info) * (tokens_count + 1));
				
					switch (palavra[i]) {
						case '+':
							tokens[tokens_count - 1].tipo = SOMA;
							break;
						case '-':	
							tokens[tokens_count - 1].tipo = SUBTRACAO;
							break;
						// case '*':	
						// 	tokens[tokens_count - 1].tipo = MULTIPLICACAO;
						// 	break;
						case '/':	
							tokens[tokens_count - 1].tipo = DIVISAO;
							break;
						default:
							tokens[tokens_count - 1].tipo = INDETERMINADO;
							break;
					}
				}
			}
		}
	}

	// Retorno o contador de tokens e o Tokens com as informações
	*count = tokens_count;
  return tokens;
}

int main(int argc, char **argv) {
	char *vetor = malloc(sizeof(char)); 
	
	size_t count;
	
	int retorno;

	// Leitura de arquivo
	FILE *file;
  file = fopen(argv[1], "r");

  if (!file) {
    printf("Arquivo não foi encontrado\n");
    return (0);
  }
	
	int tokens_count;

	Info *tokens = lexer(file, &tokens_count);

	// For utilizado para printar os dados retornados do Lexer
	// Com base nas quantidades de tokens retornados, pegamos as informações de cada um e printamos na tela
	for (int i = 0; i < tokens_count; i++) {
		switch (tokens[i].tipo) {
			case NUMERO:
	      printf("%s\n", tokens[i].valor);
	      break;
			case SOMA:
	      printf("+\n");
	      break;
	    case SUBTRACAO:
	      printf("-\n");
	      break;
	    case MULTIPLICACAO:
	      printf("×\n");
	      break;
	    case DIVISAO:
	      printf("÷\n");
	      break;
			case POTENCIA:
	      printf("^\n");
	      break;
	    case INDETERMINADO:
	      printf("Indeterminado('%s')\n", tokens[i].valor);
	      break;
			case ERRO:
				printf("Não foi possível identificar a operação\n");
				break;
		}
	}
	
  free(tokens);
	
	return 0;
}