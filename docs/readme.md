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

PROGRAM         -> FUNCTION FUNCTION_LIST

FUNCTION_LIST   -> FUNCTION FUNCTION_LIST
                | FUNCTION_LAST

FUNCTION_LAST   -> FUNCTION

FUNCTION        -> FUNC ID LPAREN RPAREN BLOCK END

BLOCK           -> LBRACE BODY RBRACE

BODY            -> DECL_SECTION STMT_SECTION

DECL_SECTION    -> INPUT_DECL OUTPUT_DECL VAR_DECLS PIN_ASSIGNMENTS

INPUT_DECL      -> INPUT COLON ID_LIST SEMICOLON
                | NO_INPUT

OUTPUT_DECL     -> OUTPUT COLON ID_LIST SEMICOLON
                | OUTPUT LPAREN ID_LIST RPAREN SEMICOLON
                | NO_OUTPUT

NO_INPUT        -> NO_INPUT_TOKEN
NO_OUTPUT       -> NO_OUTPUT_TOKEN

VAR_DECLS       -> VAR_DECL VAR_DECLS
                | NO_VAR_DECLS

VAR_DECL        -> TYPE ID SEMICOLON
                | TYPE ID ASSIGN EXPR SEMICOLON

NO_VAR_DECLS    -> NO_VARS_TOKEN

PIN_ASSIGNMENTS -> PIN_ASSIGN PIN_ASSIGNMENTS
                | NO_ASSIGNMENTS

PIN_ASSIGN      -> ASSIGN_PIN ID NUM SEMICOLON

NO_ASSIGNMENTS  -> NO_ASSIGN_TOKEN

ID_LIST         -> ID ID_LIST_TAIL

ID_LIST_TAIL    -> COMMA ID ID_LIST_TAIL
                | ID_LAST

ID_LAST         -> ID

STMT_SECTION    -> STMT STMT_SECTION
                | STMT_LAST

STMT_LAST       -> STMT

STMT            -> LED_CONTROL
                | WAIT_STMT
                | IF_STMT
                | WHILE_STMT
                | FOR_STMT
                | ASSIGNMENT
                | FUNC_CALL

LED_CONTROL     -> TURN_ON ID SEMICOLON
                | TURN_OFF ID SEMICOLON
                | TURN_ON LPAREN ID RPAREN SEMICOLON
                | TURN_OFF LPAREN ID RPAREN SEMICOLON

WAIT_STMT       -> WAIT NUM SEMICOLON

ASSIGNMENT      -> ID ASSIGN EXPR SEMICOLON

FUNC_CALL       -> ID LPAREN EXPR_LIST RPAREN SEMICOLON

EXPR_LIST       -> EXPR EXPR_LIST_TAIL
                | NO_EXPR_LIST

EXPR_LIST_TAIL  -> COMMA EXPR EXPR_LIST_TAIL
                | EXPR_LAST

EXPR_LAST       -> EXPR
NO_EXPR_LIST    -> NO_EXPR_LIST_TOKEN

EXPR            -> TERM EXPR_OP

EXPR_OP         -> PLUS TERM EXPR_OP
                | MINUS TERM EXPR_OP
                | NO_EXPR_OP

TERM            -> FACTOR TERM_OP

TERM_OP         -> MULT FACTOR TERM_OP
                | DIV FACTOR TERM_OP
                | NO_TERM_OP

FACTOR          -> NUM
                | ID
                | LPAREN EXPR RPAREN

IF_STMT         -> IF LPAREN COND RPAREN BLOCK IF_REST

IF_REST         -> ELSE_IF LPAREN COND RPAREN BLOCK IF_REST
                | ELSE BLOCK
                | DONE

COND            -> EXPR COMP_OP EXPR
                | ID

COMP_OP         -> EQ
                | NEQ
                | LT
                | GT
                | LTE
                | GTE

WHILE_STMT      -> WHILE LPAREN COND RPAREN BLOCK DONE

FOR_STMT        -> FOR LPAREN SIMPLE_ASSIGN COND SEMICOLON INCREMENT RPAREN BLOCK DONE

SIMPLE_ASSIGN   -> ID ASSIGN EXPR

INCREMENT       -> ID ASSIGN ID PLUS NUM
                | ID INCR
