#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	MULTIPLICACAO,
	POTENCIA,
	INDETERMINADO,
	ERRO,
} Token;

typedef struct {
	Token tipo;
	int valor;
} Info ;

Info * tokenizar(FILE *entrada, int* count) {
	Info * tokens = malloc(sizeof(Info ));
	int tokens_count = 0;
	
	int tamanho = 0;
	for (char x = getc(entrada); x != EOF; x = getc(entrada)) {
    tamanho = tamanho + 1;
  }

	rewind(entrada);

	int contador = 0;
	for (int i = 0; i < tamanho; i++) {
		char c = fgetc(entrada);
		if(c == '*') contador++;
			
		else if (c == ' ') break; 
		
		else{
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
						tokens[tokens_count - 1].tipo = ERRO;
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
			case MULTIPLICACAO:
				printf("×\n");
				break;
			case POTENCIA:
				printf("^\n");
				break;
			case INDETERMINADO:
				printf("Indeterminado('%c')\n", tokens[i].valor);
				break;
			case ERRO:
				printf("Não foi possível identificar a operação\n");
				break;
		}
	}
	
	free(tokens);
	
	return 0;
}