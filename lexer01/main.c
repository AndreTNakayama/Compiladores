#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	MAIS,
	MENOS,
	INDETERMINADO
} Token;

typedef struct {
	Token tipo;
	char valor;
} Info ;

Info * tokenizar(FILE *entrada, int* count) {
	Info * tokens = malloc(sizeof(Info ));
	int tokens_count = 0;
	
	int tamanho = 0;
	for (char x = getc(entrada); x != EOF; x = getc(entrada)) {
    tamanho = tamanho + 1;
  }

	rewind(entrada);
	
	for (int i = 0; i < tamanho; i++) {
		char c = fgetc(entrada);
		switch (c) {
			case '+':
				tokens_count++;
				tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));
				tokens[tokens_count - 1].tipo = MAIS;
				break;
			case '-':
				tokens_count++;
				tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));
				tokens[tokens_count - 1].tipo = MENOS;
				break;
			case ' ':
				break;
			default:
				tokens_count++;
				tokens = realloc(tokens, sizeof(Info ) * (tokens_count + 1));
				tokens[tokens_count - 1].tipo = INDETERMINADO;
				tokens[tokens_count - 1].valor = c;
				break;
		}
	}
	*count = tokens_count;
	return tokens;
}

int main(int argc, char **argv) {
	FILE *file;
  file = fopen(argv[1], "r");

  if (!file) {
    printf("Arquivo não foi encontrado\n");
    return (0);
  }
	
	int tokens_count;
	
	Info * tokens = tokenizar(file, &tokens_count);
	
	for (int i = 0; i < tokens_count; i++) {
		switch (tokens[i].tipo) {
			case MAIS:
				printf("Mais\n");
				break;
			case MENOS:
				printf("Menos\n");
				break;
			case INDETERMINADO:
				printf("Indeterminado('%c')\n", tokens[i].valor);
				break;
		}
	}
	
	free(tokens);
	
	return 0;
}