#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX 255
#define HEADER 42   

// Mnemônicos
#define NOP 00
#define STA 16
#define LDA 32
#define ADD 48
#define OR 64
#define AND 80
#define NOT 96
#define JMP 128
#define JN 144
#define JZ 160
#define HLT 240

void verifica_extensao(char *arquivo){
  char *extensao = strrchr(arquivo, '.');
  
  if (strcmp(extensao,".nar") != 0){
    printf("Erro ao ler o arquivo\nO arquivo deve ser do formato .nar!\n");
    exit(0);
  }
}

int negativo(uint8_t num){
  return num >> 7;
}
 
void ler_binario(char *arquivo){
  static uint8_t comando[MAX];
  FILE *file;
  file = fopen(arquivo, "rb");

	verifica_extensao(arquivo);
}

void verifica_bytes(char *arquivo){
  uint8_t comando[MAX], acumulador, PC;
  int f = 3;
  int k = 0;
	
  FILE *file;
  file = fopen(arquivo, "rb");
  fseek(file, 0, SEEK_SET);
  int retorno = fread(comando, MAX, 1, file);
  
  if(comando[0] != HEADER){
    printf("\nRejeitado! O primeiro byte deve ser 42.");
    exit(0);
  }
	
  acumulador = comando[1];
  PC = comando[2];

	int contador = f;
	while(comando[f] != HLT){
		contador++;
		f++;
	}

  int i = 3;

  while(comando[i] != HLT){
    switch(comando[i]){
      case STA:
        i++;
        comando[comando[i]] = acumulador;
        break;
      case LDA:
				k++;
				printf("LDA: %u\n", comando[contador+k]);
        i++;
        acumulador = comando[comando[i]];
        break;
      case ADD:
				k++;
				printf("ADD: %u\n", comando[contador+k]);
        i++;
        acumulador += comando[comando[i]];
        break;
      case OR:
        i++;
        acumulador |= comando[comando[i]];
        break;
      case AND:
        i++;
        acumulador &= comando[comando[i]];
        break;
      case NOT:
        acumulador = ~acumulador;
        break;
      case JMP:
        i++;
        i = comando[i];
        i--;
        break;
      case JN:
        i++;
        if(negativo( comando[comando[i]]) )
          i = comando[i];
          i--;
        break;
      case JZ:
        if(
					acumulador == 0){
          i++;

          i = comando[i];
          i--;
        }
        break;
    }
    i++;
  }

  PC = i;

	printf("\nRESULTADO:");
  printf("\nValor do acumulador: %u\n", acumulador);
  printf("Valor do PC: %u\n", PC);
}

int main(int argc, char **argv) {
  ler_binario(argv[1]);
  verifica_bytes(argv[1]);
	
	return 0;
}