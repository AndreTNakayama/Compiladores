#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX  10000
#define MIN -10000

char *palavras[MAX];
int qtd_palavras = 0;

// Criação dos Tokens
typedef enum {
  NUMERO,
  INVALIDO,
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
  char *valor;
} Info;

int verifica_caracter(char caracter) {
  if (isdigit(caracter) == 0 && caracter != '.' && caracter != ',') {
    return 0;
  }
  return 1;
}

void separa_palavra(FILE * file) {
  char caracter;
  char * numero = malloc(1000 * sizeof(char));
  char * simbolo = malloc(1000 * sizeof(char));
  int i = 0, j = 0;
  int flag_numero = 0;
  int flag_simbolo_duplo = 0; // Flag para verificar se o símbolo é duplo

  while ((caracter = fgetc(file)) != EOF) {
    int retorno = verifica_caracter(caracter);
    if (retorno == 0) {
      if (flag_numero) {
        flag_numero = 0;
        numero[j] = '\0';
        palavras[qtd_palavras] = malloc((strlen(numero) + 1) * sizeof(char));
        strcpy(palavras[qtd_palavras], numero);
        qtd_palavras++;
        j = 0;
      }
      
      if (caracter != '.' && caracter != ',' && caracter != ' ') {
        if (i == 0) {
          simbolo[i] = caracter;
          i++;
        } else {
          simbolo[i] = caracter;
          simbolo[i+1] = '\0';
          palavras[qtd_palavras] = malloc((strlen(simbolo) + 1) * sizeof(char));
          strcpy(palavras[qtd_palavras], simbolo);
          qtd_palavras++;
          i = 0;
          flag_simbolo_duplo = 1;
        }
      }
    }
    else {
      if (!flag_numero) {
        flag_numero = 1;
        if (i > 0) {
          simbolo[i] = '\0';
          palavras[qtd_palavras] = malloc((strlen(simbolo) + 1) * sizeof(char));
          strcpy(palavras[qtd_palavras], simbolo);
          qtd_palavras++;
          i = 0;
        }
        flag_simbolo_duplo = 0;
      }
      if (flag_simbolo_duplo) {
        continue;
      }
      numero[j++] = caracter;
    }
  }

  if (flag_numero) {
    numero[j] = '\0';
    palavras[qtd_palavras] = malloc((strlen(numero) + 1) * sizeof(char));
    strcpy(palavras[qtd_palavras], numero);
    qtd_palavras++;
  }

  free(numero);
  free(simbolo);
}

// Função utilizada para verificar se é um número válido ou uma operação
int verificacao(char *palavra) {
  int v_Ponto = 0;
  int v_Virgula = 0;

  char numeros[12] = {'0', '1', '2', '3', '4', '5',
                      '6', '7', '8', '9', '.', ','};
  char simbolos[4] = {'+', '-', '*', '/'};

  for (int i = 0; i < strlen(palavra); i++) {
    char v_Numero = 'F';

    if (palavra[i] == '.') {
      v_Ponto++;
    }

    if (palavra[i] == ',') {
      v_Virgula++;
    }

    // Enquanto v_Numero for igual a 'T' é um número válido
    for (int j = 0; j < 12; j++) {
      if (palavra[i] == numeros[j]) {
        v_Numero = 'T';
      }
    }

    if (v_Numero == 'T') {
      continue;
    } else {
      // Nao é um numero, então vamos verificar se é uma operação
      for (int j = 0; j < 4; j++) {
        if (palavra[i] == simbolos[j])
          return 2;
      }

      return 0;
    }
  }

  // Se ele for um número, verifica se tem mais de 1 ponto ou mais de 1 vírgula
  if (v_Ponto <= 1 && v_Virgula <= 1)
    return 1;
  else
    return 0;
}

// Função responsável por retornar o Token com os dados
Info *lexer(FILE *file, int *count) {
  // Criação da variavel para controle dos Tokens
  Info *tokens = malloc(sizeof(Info));
  int tokens_count = 0;

  separa_palavra(file);
  
  // Enquanto existir palavras para serem lidas dentro do arquivo de entrada
  for (int i = 0; i < qtd_palavras; i++) {
    char palavra[MAX];
    int retorno;

    // Passa a palavra lida dentro do arquivo do tipo FILE para um vetor do tipo
    // char
    // int scan = fscanf(file, "%s", palavra);
    strcpy(palavra, palavras[i]);

    retorno = verificacao(palavra);

    // Se o retorno é 1 significa que é um número
    if (retorno == 1) {
      // Contador de Token recebe +1, pois será adicionado um novo Token
      tokens_count++;
      // Realocamos memório pois temos um novo elemento a ser inserido
      tokens = realloc(tokens, sizeof(Info) * (tokens_count + 1));
      // Como o controle começa com a variável 0, temos que fazer o tokens_count
      // - 1 para colocar o novo elemento na posição correta
      tokens[tokens_count - 1].tipo = NUMERO;
      tokens[tokens_count - 1].valor =
      malloc(sizeof(char) * (strlen(palavra) + 1));
      // Strcpy é utilizado para copiar strings
      strcpy(tokens[tokens_count - 1].valor, palavra);
    }

    // Se o retorno é 2, significa que é uma operação
    // No código abaixo vamos verificar a quantidade de asterisco (*), para
    // validarmos se é uma operação de potenciação
    if (retorno == 2) {
      int tamanho = strlen(palavra);
      int cont_asterisco = 0;

      // Se chegarmos no final da string, verificamos a quantidade de *
      for (int i = 0; i < tamanho; i++) {
        if (palavra[i] == '*') {
          cont_asterisco++;

          if (cont_asterisco == tamanho) {
            // A mesma lógica utilizada quando o retorno é 1
            tokens_count++;
            tokens = realloc(tokens, sizeof(Info) * (tokens_count + 1));

            // Switch para definir qual operação equivale a quantidade de *
            if (cont_asterisco != 0) {
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

        else if (palavra[i] == ' ') {
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

    if (retorno == 0) {
      tokens_count++;
      tokens = realloc(tokens, sizeof(Info) * (tokens_count + 1));
      tokens[tokens_count - 1].tipo = INVALIDO;
      tokens[tokens_count - 1].valor =
          malloc(sizeof(char) * (strlen(palavra) + 1));
      strcpy(tokens[tokens_count - 1].valor, palavra);
    }
  }

  // Retorno o contador de tokens e o Tokens com as informações
  *count = tokens_count;
  return tokens;
}

// Parser
double parser(Info *tokens, int *count) {
  double expressao = MIN;

  // Se existe algum Token
  if (count != 0) {
    // Para cada Token
    for (int i = 0; i < *count; i++) {
      switch (tokens[i].tipo) {
      case SOMA:
        if(tokens[i+1].tipo == NUMERO){
          expressao += atof(tokens[i + 1].valor);
        }
        break;
      case SUBTRACAO:
        if(tokens[i+1].tipo == NUMERO){
          expressao -= atof(tokens[i + 1].valor);
        }
        break;
      case DIVISAO:
        if(tokens[i+1].tipo == NUMERO){
          expressao /= atof(tokens[i + 1].valor);
        }
        break;
      case MULTIPLICACAO:
        if(tokens[i+1].tipo == NUMERO){
          expressao *= atof(tokens[i + 1].valor);
        }
        break;
      case POTENCIA:
        if(tokens[i+1].tipo == NUMERO){
           expressao = (pow(expressao, atof(tokens[i + 1].valor)));
        }
        break;
      case NUMERO:
        if(expressao == MIN){
          expressao = atof(tokens[i].valor);
        }
        break;
      case INVALIDO:
        return 0;
      case ERRO:
        return 0;
      case INDETERMINADO:
        printf("Arquivo invalido\n");
        break;
      }
    }
  }
  return expressao;
}

int main(int argc, char **argv) {
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
  // Com base nas quantidades de tokens retornados, pegamos as informações de
  // cada um e printamos na tela
  for (int i = 0; i < tokens_count; i++) {
    switch (tokens[i].tipo) {
    case INVALIDO:
      printf("Numero inválido: %s\n", tokens[i].valor);
      break;
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

  // ----------------------------------------------------- //

  double expressao;

  // Chama a verificação do Parser
  expressao = parser(tokens, &tokens_count);

  printf("\n");

  if (expressao == 0)
    printf("Expressão vazia\n");
  else
    printf("Entrada válida\nResultado: %.2f\n", expressao);

  free(tokens);

  return 0;
}