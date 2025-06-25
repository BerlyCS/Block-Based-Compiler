# Compilador de un lenguaje en español de arduino
- Orientado a estudiantes
- Lenguaje personalizado
- Lenguaje es parecido a pseudocodigo
- En español
- Muestra el codigo ensamblador al compilar el codigo
- Modulos como leds, botones y motores
- Salida digital (LOW, HIGH)

### Lenguaje

##### Tokens:

* ESPERAR
* ENTRADA
* SALIDA
* DEFINIR_PIN
* APAGAR
* PRENDER
* SI, MIENTRAS, ENTERO, DECIMAL, CADENA, BOOL
* SINO SI, SINO, HECHO
* Operadores --> '+', '-', '/', '*', '==', '!=', '<', '>'
* '(', ')'
* '1', '2', ..., números literales
* FUNCION, FIN
* VERDADERO, FALSO

##### Gramatica

PROGRAMA -> FUNCION PROGRAMA
         | BLOQUE PROGRAMA
         | ε

FUNCION -> "funcion" ID_FUNCION "(" ")" BLOQUE "fin"

ID_FUNCION -> "cualquier_id"

BLOQUE -> ε
       | BLOQUE BLOQUE
       | INSTRUCCION ";"
       | CONTROL

INSTRUCCION -> "definir_pin" "(" ID "," NUM ")"
             | "salida" "(" ID ")"
             | "entrada" "(" ID ")"
             | "prender" "(" ID ")"
             | "apagar" "(" ID ")"
             | "esperar" "(" NUM ")"
             | DECLARACION
             | ASIGNACION

DECLARACION -> TIPO ID "=" EXPRESION

TIPO -> "entero"
     | "decimal"
     | "cadena"
     | "booleano"

ASIGNACION -> ID "=" EXPRESION

EXPRESION -> EXPRESION OP EXPRESION
           | "leer" "(" ID ")"
           | NUM
           | ID
           | BOOL
           | STR

OP -> "+" | "-" | "*" | "/" | "==" | ">" | "<"

CONTROL -> MIENTRAS
         | SI
         | POR

MIENTRAS -> "mientras" "(" EXPRESION ")" BLOQUE "fin_mientras"

SI -> "si" "(" EXPRESION ")" BLOQUE SINO_LISTA

SINO_LISTA -> "sino si" "(" EXPRESION ")" BLOQUE SINO_LISTA
            | "sino" BLOQUE
            | ε

POR -> "por" "(" ID "=" EXPRESION ";" EXPRESION ";" ASIGNACION ")" BLOQUE "fin_por"
