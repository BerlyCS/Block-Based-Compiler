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
PROGRAMA -> BLOQUE PROGRAMA
PROGRAMA -> æ

FUNCION -> funcion ID_FUNCION () BLOQUE fin

BLOQUE -> æ
BLOQUE -> BLOQUE BLOQUE
BLOQUE -> INSTRUCCION;

INSTRUCCION -> DEFINIR_PIN(ID, NUM)
INSTRUCCION -> SALIDA(ID)
INSTRUCCION -> ENTRADA(ID)
INSTRUCCION -> LEER(ID)


