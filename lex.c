#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1

#define ID 256
#define NUM 257

#define ESPERAR 300
#define ASIGNAR_PIN 301
#define PRENDER 302
#define APAGAR 303
#define MIENTRAS 304
#define SI 305
#define SINO 306
#define SINO_SI 307
#define HECHO 308
#define ENTERO 309
#define DECIMAL 310
#define CADENA 311
#define BYTE 312
#define FUNCION 313
#define FIN 314
#define LED_INTERNO 315
#define BOTONES 316
#define MOTORES 317
#define SALIDA 318
#define ENTRADA 319
// #define CONFIGURACION 320
// #define PRINCIPAL 321

#define POR            322
#define COLON          ':'
#define INCREMENTO     351

#define ARITHM 350

#define LPAREN '('
#define RPAREN ')'
#define LBRACKET '['
#define RBRACKET ']'
#define LBRACE '{'
#define RBRACE '}'
#define COMMA ','
#define SEMICOLON ';'
#define ASSIGN '='

#define EQ 400 /* == */
#define NE 401 /* != */
#define LE 402 /* <= */
#define GE 403 /* >= */
#define LT '<'
#define GT '>'

int scaner();
void show(int);
int isKeyWord(int *);
void jumpComment();
void jumpBlockComment();

FILE *f;
char lexema[80];

// int main(int argc, char *argv[]) {
//   int token;
//   f = stdin;
//   if (argc == 2) {
//     f = fopen(argv[1], "rt");
//     if (f == NULL) {
//       f = stdin;
//     }
//   }
//   if (f == stdin)
//     printf("Insertar texto. Presiona Ctrl+D para terminar.\n");

//   while (true) {
//     token = scaner();
//     if (token == EOF)
//       break;
//     show(token);
//   }

//   if (f != stdin)
//     fclose(f);
//   return 0;
// }

int scaner() {
  int c, i;
  do {
    c = fgetc(f);
    if (c == EOF)
      return EOF;
  } while (isspace(c));

  if (c == '-') {
    int next = fgetc(f);
    if (isdigit(next)) {
      i = 0;
      lexema[i++] = '-';
      c = next;
      do {
        lexema[i++] = c;
        c = fgetc(f);
      } while (isdigit(c));
      if (c == '.') {
        lexema[i++] = c;
        c = fgetc(f);
        while (isdigit(c)) {
          lexema[i++] = c;
          c = fgetc(f);
        }
      }
      lexema[i] = '\0';
      ungetc(c, f);
      return NUM;
    } else {
      ungetc(next, f);
      return ARITHM;
    }
  }
  if (isdigit(c)) {
    i = 0;
    do {
      lexema[i++] = c;
      c = fgetc(f);
    } while (isdigit(c));
    if (c == '.') {
      lexema[i++] = c;
      c = fgetc(f);
      while (isdigit(c)) {
        lexema[i++] = c;
        c = fgetc(f);
      }
    }
    lexema[i] = '\0';
    ungetc(c, f);
    return NUM;
  }

  if (isalpha(c)) {
    i = 0;
    do {
      lexema[i++] = c;
      c = fgetc(f);
    } while (isalnum(c) || c == '_');
    lexema[i] = '\0';
    ungetc(c, f);

    if (isKeyWord(&i))
      return i;
    else
      return ID;
  }

  if (c == '+' || c == '-' || c == '*' || c == '/') {
    int next = fgetc(f);
    if (c == '+') {
      return INCREMENTO;
    }
    if (c == '/' && next == '/') {
      jumpComment();
      return scaner();
    } else if (c == '/' && next == '*') {
      jumpBlockComment();
      return scaner();
    } else {
      ungetc(next, f);
      return ARITHM;
    }
  }

  if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
      c == ',' || c == ';') {
    return c;
  }

  if (c == '<' || c == '>' || c == '=' || c == '!') {
    int next = fgetc(f);
    if (next == '=') {
      if (c == '<')
        return LE; /* <= */
      if (c == '>')
        return GE; /* >= */
      if (c == '=')
        return EQ; /* == */
      if (c == '!')
        return NE; /* != */
    } else {
      ungetc(next, f);
      if (c == '<' || c == '>')
        return c;
      if (c == '=')
        return ASSIGN;
      if (c == '!')
        return NE;
    }
  }
  if (c == ':' ) {
    return COLON;
  }


  return c;
}

int isKeyWord(int *i) {
  if (strcmp(lexema, "ESPERAR") == 0)
    *i = ESPERAR;
  else if (strcmp(lexema, "ASIGNAR_PIN") == 0)
    *i = ASIGNAR_PIN;
  else if (strcmp(lexema, "PRENDER") == 0)
    *i = PRENDER;
  else if (strcmp(lexema, "APAGAR") == 0)
    *i = APAGAR;
  else if (strcmp(lexema, "MIENTRAS") == 0)
    *i = MIENTRAS;
  else if (strcmp(lexema, "SI") == 0)
    *i = SI;
  else if (strcmp(lexema, "SINO") == 0)
    *i = SINO;
  else if (strcmp(lexema, "SINO_SI") == 0)
    *i = SINO_SI;
  else if (strcmp(lexema, "HECHO") == 0)
    *i = HECHO;
  else if (strcmp(lexema, "ENTERO") == 0)
    *i = ENTERO;
  else if (strcmp(lexema, "DECIMAL") == 0)
    *i = DECIMAL;
  else if (strcmp(lexema, "CADENA") == 0)
    *i = CADENA;
  else if (strcmp(lexema, "BYTE") == 0)
    *i = BYTE;
  else if (strcmp(lexema, "FUNCION") == 0)
    *i = FUNCION;
  else if (strcmp(lexema, "FIN") == 0)
    *i = FIN;
  else if (strcmp(lexema, "LED_INTERNO") == 0)
    *i = LED_INTERNO;
  else if (strcmp(lexema, "BOTONES") == 0)
    *i = BOTONES;
  else if (strcmp(lexema, "MOTORES") == 0)
    *i = MOTORES;
  else if (strcmp(lexema, "SALIDA") == 0)
    *i = SALIDA;
  else if (strcmp(lexema, "ENTRADA") == 0)
    *i = ENTRADA;
  // else if (strcmp(lexema, "CONFIGURACION") == 0)
  //   *i = CONFIGURACION;
  // else if (strcmp(lexema, "PRINCIPAL") == 0)
  //   *i = PRINCIPAL;
  else if (strcmp(lexema, "POR") == 0)
    *i = POR;
  else
    return 0;

  return 1;
}

void show(int token) {
  switch (token) {
  case ID:
    printf("token = ID            [%s]\n", lexema);
    break;
  case NUM:
    printf("token = NUM           [%s]\n", lexema);
    break;

  case ESPERAR:
    printf("token = ESPERAR       [%s]\n", lexema);
    break;
  case ASIGNAR_PIN:
    printf("token = ASIGNAR_PIN   [%s]\n", lexema);
    break;
  case PRENDER:
    printf("token = PRENDER       [%s]\n", lexema);
    break;
  case APAGAR:
    printf("token = APAGAR        [%s]\n", lexema);
    break;
  case MIENTRAS:
    printf("token = MIENTRAS      [%s]\n", lexema);
    break;
  case SI:
    printf("token = SI            [%s]\n", lexema);
    break;
  case SINO:
    printf("token = SINO          [%s]\n", lexema);
    break;
  case SINO_SI:
    printf("token = SINO_SI       [%s]\n", lexema);
    break;
  case HECHO:
    printf("token = HECHO         [%s]\n", lexema);
    break;
  case ENTERO:
    printf("token = ENTERO        [%s]\n", lexema);
    break;
  case DECIMAL:
    printf("token = DECIMAL       [%s]\n", lexema);
    break;
  case CADENA:
    printf("token = CADENA        [%s]\n", lexema);
    break;
  case BYTE:
    printf("token = BYTE          [%s]\n", lexema);
    break;
  case FUNCION:
    printf("token = FUNCION       [%s]\n", lexema);
    break;
  case FIN:
    printf("token = FIN           [%s]\n", lexema);
    break;
  case LED_INTERNO:
    printf("token = LED_INTERNO   [%s]\n", lexema);
    break;
  case BOTONES:
    printf("token = BOTONES       [%s]\n", lexema);
    break;
  case MOTORES:
    printf("token = MOTORES       [%s]\n", lexema);
    break;

  case ARITHM:
    printf("token = ARITHM        [ %c ]\n", lexema[0]);
    break;

  case EQ:
    printf("token = EQ            [==]\n");
    break;
  case NE:
    printf("token = NE            [!=]\n");
    break;
  case LE:
    printf("token = LE            [<=]\n");
    break;
  case GE:
    printf("token = GE            [>=]\n");
    break;
  case LT:
    printf("token = LT            [<]\n");
    break;
  case GT:
    printf("token = GT            [>]\n");
    break;
  case ASSIGN:
    printf("token = ASSIGN        [=]\n");
    break;

  case LPAREN:
  case RPAREN:
  case LBRACKET:
  case RBRACKET:
  case LBRACE:
  case RBRACE:
  case COMMA:
  case SEMICOLON:
    printf("token = '%c'      [%c]\n", token, token);
    break;
  case SALIDA:
    printf("token = SALIDA    [%s]\n", lexema);
    break;

  case ENTRADA:
    printf("token = ENTRADA   [%s]\n", lexema);
    break;

  // case CONFIGURACION:
  //   printf("token = CONFIGURACION   [%s]\n", lexema);
  //   break;
  // case PRINCIPAL:
  //   printf("token = PRINCIPAL       [%s]\n", lexema);
  //   break;
  case POR:
    printf("token = POR             [%s]\n", lexema);
    break;
  case COLON:
    printf("token = COLON           [:]\n");
    break;
  case INCREMENTO:
    printf("token = INCREMENTO      [++]\n");
    break;
  default:
    printf("token = DESCONOCIDO   [%d]\n", token);
    break;
  }
}

void jumpComment() {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n')
      break;
  }
}

void jumpBlockComment() {
  int c, d;
  while ((c = fgetc(f)) != EOF) {
    if (c == '*') {
      d = fgetc(f);
      if (d == '/')
        break;
      else
        ungetc(d, f);
    }
  }
}

int lookahead;

typedef struct AST {
    char *label;
    struct AST **children;
    int child_count;
} AST;

AST* new_node(const char *label) {
    AST *node = malloc(sizeof(AST));
    node->label = strdup(label);
    node->children = NULL;
    node->child_count = 0;
    return node;
}

// Add child to node
void add_child(AST *parent, AST *child) {
    parent->children = realloc(parent->children, sizeof(AST*) * (parent->child_count+1));
    parent->children[parent->child_count++] = child;
}

void print_ast(AST *node, int depth) {
    for(int i=0; i<depth; i++) putchar(' ');
    printf("%s\n", node->label);
    for(int i=0; i<node->child_count; i++) print_ast(node->children[i], depth+2);
}

void parse_programa() {

}

int main(int argc, char *argv[]) {
  int token;
  f = stdin;
  if (argc == 2) {
    f = fopen(argv[1], "rt");
    if (f == NULL) {
      f = stdin;
    }
  }
  if (f == stdin)
    printf("Insertar texto. Presiona Ctrl+D para terminar.\n");

  while (true) {
    token = scaner();
    if (token == EOF)
      break;
    show(token);
  }

  lookahead = scaner();

  // AST *ast = parsePrograma();
  // print_ast(ast, 0);
  // return 0;
  if (f != stdin)
    fclose(f);
}

