// parser.js
class Parser {
  constructor(tokens) { this.tokens = tokens; this.pos = 0; }
  peek(offset = 0) { return this.tokens[this.pos + offset] || { type: 'EOF' }; }
  consume(type) {
    const tk = this.peek();
    if ((Array.isArray(type) && type.includes(tk.type)) || tk.type === type) {
      this.pos++; return tk;
    }
    throw new SyntaxError(`Se esperaba '${Array.isArray(type) ? type.join("' o '") : type}' pero se obtuvo '${tk.type}' en posición ${this.pos}`);
  }

  // Entry point
  parseProgram() {
    const functions = [];
    while (this.peek().type !== 'EOF') {
      functions.push(this.parseFunction());
    }
    return { type: 'Program', functions };
  }

  // Allow CONFIGURACION and PRINCIPAL as names
  parseFunction() {
    this.consume('FUNCION');
    const nameToken = this.consume(['ID', 'CONFIGURACION', 'PRINCIPAL']);
    const name = nameToken.value;
    this.consume('(');
    this.consume(')');
    const body = this.parseBlock();
    this.consume('FIN');
    return { type: 'FunctionDeclaration', name, body };
  }

  parseBlock() {
    const stmts = [];
    while (!['FIN', 'EOF'].includes(this.peek().type)) {
      stmts.push(this.parseStatement());
    }
    return stmts;
  }

  parseStatement() {
    const tk = this.peek();
    if (['PRENDER', 'APAGAR'].includes(tk.type)) return this.parseLedControl();
    if (tk.type === 'ESPERAR') return this.parseWait();
    if (['ENTERO', 'DECIMAL', 'BYTE', 'CADENA'].includes(tk.type)) return this.parseDeclaration();
    if (tk.type === 'ID') {
      return this.peek(1).type === '=' ? this.parseAssignment() : this.parseFuncCall();
    }
    throw new SyntaxError(`Sentencia inesperada '${tk.type}' en posición ${this.pos}`);
  }

  parseLedControl() {
    const action = this.consume(['PRENDER', 'APAGAR']).type;
    this.consume('(');
    const pin = this.consume('ID').value;
    this.consume(')');
    this.consume(';');
    return { type: 'LedControl', action, pin };
  }

  parseWait() {
    this.consume('ESPERAR');
    this.consume('(');
    const val = this.consume('NUM').value;
    this.consume(')');
    this.consume(';');
    return { type: 'Wait', value: Number(val) };
  }

  parseDeclaration() {
    const varType = this.consume(['ENTERO', 'DECIMAL', 'BYTE', 'CADENA']).type;
    const name = this.consume('ID').value;
    let expr = null;
    if (this.peek().type === '=') {
      this.consume('=');
      expr = this.parseExpression();
    }
    this.consume(';');
    return { type: 'VariableDeclaration', varType, name, expr };
  }

  parseAssignment() {
    const name = this.consume('ID').value;
    this.consume('=');
    const expr = this.parseExpression();
    this.consume(';');
    return { type: 'Assignment', name, expr };
  }

  parseFuncCall() {
    const name = this.consume('ID').value;
    this.consume('(');
    const args = [];
    if (this.peek().type !== ')') {
      args.push(this.parseExpression());
      while (this.peek().type === ',') {
        this.consume(',');
        args.push(this.parseExpression());
      }
    }
    this.consume(')');
    this.consume(';');
    return { type: 'FunctionCall', name, args };
  }

  parseExpression() {
    let node = this.parseTerm();
    while (['+', '-'].includes(this.peek().type)) {
      const op = this.consume(['+', '-']).type;
      const right = this.parseTerm();
      node = { type: 'BinaryExpression', operator: op, left: node, right };
    }
    return node;
  }

  parseTerm() {
    let node = this.parseFactor();
    while (['*', '/'].includes(this.peek().type)) {
      const op = this.consume(['*', '/']).type;
      const right = this.parseFactor();
      node = { type: 'BinaryExpression', operator: op, left: node, right };
    }
    return node;
  }

  parseFactor() {
    const tk = this.peek();
    if (tk.type === 'NUM') {
      this.consume('NUM');
      return { type: 'Literal', value: Number(tk.value) };
    }
    if (tk.type === 'ID') {
      this.consume('ID');
      return { type: 'Identifier', name: tk.value };
    }
    if (tk.type === '(') {
      this.consume('(');
      const expr = this.parseExpression();
      this.consume(')');
      return expr;
    }
    throw new SyntaxError(`Factor inesperado '${tk.type}' en posición ${this.pos}`);
  }
}

// Exponer en global
window.Parser = Parser;
