### Block Based Arduino Compiler
- Orientado a estudiantes
- Lenguaje personalizado
    - Lenguaje es parecido a pseudocodigo
    - En español
- Muestra el codigo ensamblador al compilar el codigo
- Hecho con blockly
- Ya preconfigurado las entradas y salidas
- Modulos como leds, botones y pantallas LCD, lectura de voltaje, parpadeo
- Salida digital (LOW, HIGH)

##### Lenguaje

- Tokens
    * ESPERAR
    * ENTRADA : INPUTS
    * SALIDA : OUTPUTS
    * ASIGNAR_PIN
    * APAGAR
    * PRENDER
    * SI, MIENTRAS, POR, ENTERO, DECIMAL, CADENA, BYTE
    * SINO SI, SINO, HECHO
    * LED_INTERNO, BOTONES, MOTORES
    * '+', '-', '/' ...
    * '{', '}'
    * '(', ')'
    * '1', '2', ...
    * FUNCION, FIN
    * VERDADERO, FALSO

##### Gramatica

<!-- LINE -> EXPR DELIM -->
<!-- EXPR -> EXPR OPERATOR EXPR -->
<!-- EXPR -> (EXPR) -->
<!-- DELIM -> ; -->
<!-- OPERATOR -> -->

PROGRAM -> FUNCTION FUNCTION_LIST

FUNCTION_LIST -> FUNCTION FUNCTION_LIST
FUNCTION_LIST -> FUNCTION_LAST

FUNCTION_LAST -> FUNCTION

FUNCTION -> func id lparen rparen BLOCK end

BLOCK -> lbrace BODY rbrace

BODY -> DECL_SECTION STMT_SECTION

DECL_SECTION -> INPUT_DECL OUTPUT_DECL VAR_DECLS PIN_ASSIGNMENTS

INPUT_DECL -> input colon ID_LIST semicolon
INPUT_DECL -> NO_INPUT

OUTPUT_DECL -> output colon ID_LIST semicolon
OUTPUT_DECL -> output lparen ID_LIST rparen semicolon
OUTPUT_DECL -> NO_OUTPUT

NO_INPUT -> no_input_token

NO_OUTPUT -> no_output_token

VAR_DECLS -> VAR_DECL VAR_DECLS
VAR_DECLS -> NO_VAR_DECLS

VAR_DECL -> type id semicolon
VAR_DECL -> type id assign EXPR semicolon

NO_VAR_DECLS -> no_vars_token

PIN_ASSIGNMENTS -> PIN_ASSIGN PIN_ASSIGNMENTS
PIN_ASSIGNMENTS -> NO_ASSIGNMENTS

PIN_ASSIGN -> assign_pin id num semicolon

NO_ASSIGNMENTS -> no_assign_token

ID_LIST -> id ID_LIST_TAIL

ID_LIST_TAIL -> comma id ID_LIST_TAIL
ID_LIST_TAIL -> ID_LAST

ID_LAST -> id

STMT_SECTION -> STMT STMT_SECTION
STMT_SECTION -> STMT_LAST

STMT_LAST -> STMT

STMT -> LED_CONTROL
STMT -> WAIT_STMT
STMT -> IF_STMT
STMT -> WHILE_STMT
STMT -> FOR_STMT
STMT -> ASSIGNMENT
STMT -> FUNC_CALL

LED_CONTROL -> turn_on id semicolon
LED_CONTROL -> turn_off id semicolon
LED_CONTROL -> turn_on lparen id rparen semicolon
LED_CONTROL -> turn_off lparen id rparen semicolon

WAIT_STMT -> wait num semicolon

ASSIGNMENT -> id assign EXPR semicolon

FUNC_CALL -> id lparen EXPR_LIST rparen semicolon

EXPR_LIST -> EXPR EXPR_LIST_TAIL
EXPR_LIST -> NO_EXPR_LIST

EXPR_LIST_TAIL -> comma EXPR EXPR_LIST_TAIL
EXPR_LIST_TAIL -> EXPR_LAST

EXPR_LAST -> EXPR

NO_EXPR_LIST -> no_expr_list_token

EXPR -> TERM EXPR_OP

EXPR_OP -> plus TERM EXPR_OP
EXPR_OP -> minus TERM EXPR_OP
EXPR_OP -> no_expr_op

TERM -> FACTOR TERM_OP

TERM_OP -> mult FACTOR TERM_OP
TERM_OP -> div FACTOR TERM_OP
TERM_OP -> no_term_op

FACTOR -> num
FACTOR -> id
FACTOR -> lparen EXPR rparen

IF_STMT -> if lparen COND rparen BLOCK IF_REST

IF_REST -> else_if lparen COND rparen BLOCK IF_REST
IF_REST -> else BLOCK
IF_REST -> done

COND -> EXPR COMP_OP EXPR
COND -> id

COMP_OP -> eq
COMP_OP -> neq
COMP_OP -> lt
COMP_OP -> gt
COMP_OP -> lte
COMP_OP -> gte

WHILE_STMT -> while lparen COND rparen BLOCK done

FOR_STMT -> for lparen SIMPLE_ASSIGN COND semicolon INCREMENT rparen BLOCK done

SIMPLE_ASSIGN -> id assign EXPR

INCREMENT -> id assign id plus num
INCREMENT -> id incr
