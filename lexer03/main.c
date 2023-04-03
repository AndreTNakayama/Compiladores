#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000

typedef enum {
	ZERO,
	UM,
	DOIS,
	TRES,
	QUATRO,
	CINCO,
	SEIS,
	SETE,
	OITO,
	NOVE,
	PONTO,
	VIRGULA,
	INDETERMINADO,
	ESPACO,
	SOMA,
	SUBTRACAO,
	DIVISAO,
	MULTIPLICACAO,
	POTENCIA,
} Token;

typedef struct {
	Token tipo;
	int valor;
} Info ;

Info * tokenizar(FILE *entrada, int* count) {
	Info * tokens = malloc(sizeof(Info ));
	int tokens_count = 0;
	
	size_t v_Ponto = 0;
	size_t v_Virgula = 0;
	
	size_t tamanho = 0;
	for (char x = getc(entrada); x != EOF; x = getc(entrada)) {
    tamanho = tamanho + 1;
  }
	
	rewind(entrada);

	// printf("Tamanho: %d\n", tamanho);
	int contador = 0;
	for (int i = 0; i < tamanho; i++) {
		char c = fgetc(entrada);

		if(c == '*') contador++;
		
		else if (c == ' '){
			tokens_count++;
			tokens[tokens_count - 1].tipo = ESPACO;
			tokens[tokens_count - 1].valor = c;
			continue;
		}
		else
		{
			tokens_count++;
			tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));
			if(contador != 0){
				switch (contador) {
					case 1:
						tokens[tokens_count - 1].tipo = MULTIPLICACAO;
						tokens[tokens_count - 1].valor = c;
						break;
					case 2:
						tokens[tokens_count - 1].tipo = POTENCIA;
						tokens[tokens_count - 1].valor = c;
						break;
					default:
						tokens[tokens_count - 1].tipo = INDETERMINADO;
						tokens[tokens_count - 1].valor = c;
						*count = tokens_count;
						return tokens;
				}

				tokens_count++;
				tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));		
			}

			tokens[tokens_count - 1].tipo = INDETERMINADO;
			tokens[tokens_count - 1].valor = c;
			
			contador = 0;
	
			switch (c) {
				case '+':
					tokens[tokens_count - 1].tipo = SOMA;
					tokens[tokens_count - 1].valor = c;
					break;
				case '-':
					tokens[tokens_count - 1].tipo = SUBTRACAO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '/':
					tokens[tokens_count - 1].tipo = DIVISAO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '0':
					tokens[tokens_count - 1].tipo = ZERO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '1':
					tokens[tokens_count - 1].tipo = UM;
					tokens[tokens_count - 1].valor = c;
					break;
				case '2':
					tokens[tokens_count - 1].tipo = DOIS;
					tokens[tokens_count - 1].valor = c;
					break;
				case '3':
					tokens[tokens_count - 1].tipo = TRES;
					tokens[tokens_count - 1].valor = c;
					break;
				case '4':
					tokens[tokens_count - 1].tipo = QUATRO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '5':
					tokens[tokens_count - 1].tipo = CINCO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '6':
					tokens[tokens_count - 1].tipo = SEIS;
					tokens[tokens_count - 1].valor = c;
					break;
				case '7':
					tokens[tokens_count - 1].tipo = SETE;
					tokens[tokens_count - 1].valor = c;
					break;
				case '8':
					tokens[tokens_count - 1].tipo = OITO;
					tokens[tokens_count - 1].valor = c;
					break;
				case '9':
					tokens[tokens_count - 1].tipo = NOVE;
					tokens[tokens_count - 1].valor = c;
					break;
				case '.':
					tokens[tokens_count - 1].tipo = PONTO;
					tokens[tokens_count - 1].valor = c;
					break;
				case ',':
					tokens[tokens_count - 1].tipo = VIRGULA;
					tokens[tokens_count - 1].valor = c;
					break;
				default:
					tokens[tokens_count - 1].tipo = INDETERMINADO;
					tokens[tokens_count - 1].valor = c;
					break;
					// *count = tokens_count;
					// return tokens;

				tokens_count++;
				tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));		
			}
		}
	}

	*count = tokens_count;
	return tokens;
}

int main(int argc, char **argv) {
	char *vetor = malloc(sizeof(char)); 
	size_t count;
	
	FILE *file;
  file = fopen(argv[1], "r");

  if (!file) {
    printf("Arquivo não foi encontrado\n");
    return (0);
  }
	
	int tokens_count;
	
	Info * tokens = tokenizar(file, &tokens_count);
	
	int v_Ponto = 0;
	int v_Virgula = 0;

	for (int i = 0; i < tokens_count; i++) {
		switch (tokens[i].tipo) {
			case ZERO:
				printf("0");
				break;
			case UM:
				printf("1");
				break;
			case DOIS:
				printf("2");
				break;
			case TRES:
				printf("3");
				break;
			case QUATRO:
				printf("4");
				break;
			case CINCO:
				printf("5");
				break;
			case SEIS:
				printf("6");
				break;
			case SETE:
				printf("7");
				break;
			case OITO:
				printf("8");
				break;
			case NOVE:
				printf("9");
				break;
			case PONTO:
				printf(".");
				break;
			case VIRGULA:
				printf(",");
				break;
			case SOMA:
				printf("+");
				break;
			case SUBTRACAO:
				printf("-");
				break;
			case DIVISAO:
				printf("÷");
				break;
			case MULTIPLICACAO:
				printf("×");
				count++;
				vetor = realloc(vetor, sizeof(char) * (count + 1));
				vetor[count - 1] = '*';
				break;
			case POTENCIA:
				printf("^\n");
				count++;
				vetor = realloc(vetor, sizeof(char) * (count + 1));
				vetor[count - 1] = '^';
				break;
			case INDETERMINADO:
				printf("\nIndeterminado('%c')", tokens[i].valor);
				break;
			case ESPACO:
				printf("\n");
				break;
		}
	}
	
	free(tokens);
	free(vetor);
	
	return 0;
}