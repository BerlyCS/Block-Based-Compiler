const TOKENS = {
  ESPERAR: 300, ASIGNAR_PIN: 301, PRENDER: 302, APAGAR: 303,
  MIENTRAS: 304, SI: 305, SINO: 306, SINO_SI: 307, HECHO: 308,
  ENTERO: 309, DECIMAL: 310, CADENA: 311, BYTE: 312,
  FUNCION: 313, FIN: 314, LED_INTERNO: 315, BOTONES: 316,
  MOTORES: 317, SALIDA: 318, ENTRADA: 319, CONFIGURACION: 320,
  PRINCIPAL: 321
};

function isDigit(c) {
  return /[0-9]/.test(c);
}
function isAlpha(c) {
  return /[A-Za-z_]/.test(c);
}
function isAlnum(c) {
  return /[A-Za-z0-9_]/.test(c);
}

function analizar() {
  const input = document.getElementById('sourceCode').value;
  let i = 0, tokens = [];

  while (i < input.length) {
    let c = input[i];
    // Espacios y saltos ||||||||||||||||||||||||||
    if (/\s/.test(c)) { i++; continue; }

    // Comentarios |||||||||||||||||||||||||||||
    if (c === '/' && input[i+1] === '/') {
      while (i < input.length && input[i] !== '\n') i++;
      continue;
    }

    // Números (enteros o decimales) |||||||||||||||||||
    if (isDigit(c) || (c === '-' && isDigit(input[i+1]))) {
      let start = i;
      if (c === '-') i++;
      while (i < input.length && isDigit(input[i])) i++;
      if (input[i] === '.') {
        i++;
        while (i < input.length && isDigit(input[i])) i++;
      }
      const lex = input.slice(start, i);
      tokens.push({ type: 'NUM', value: lex });
      continue;
    }

    // Identificadores y palabras clave||||||||||||||||
    if (isAlpha(c)) {
      let start = i;
      while (i < input.length && isAlnum(input[i])) i++;
      const lex = input.slice(start, i);
      const type = TOKENS[lex] ? lex : 'ID';
      tokens.push({ type, value: lex });
      continue;
    }

    // Símbolos y operadores|||||||||||||||||||||||||||||
    const two = input.substr(i, 2);
    if (['==','!=','<=','>='].includes(two)) {
      tokens.push({ type: two, value: two });
      i += 2;
      continue;
    }
    if ('+-*/(){};,=<>' .includes(c)) {
      tokens.push({ type: c, value: c });
      i++;
      continue;
    }

    // Desconocido||||||||||||||||||||||||||||||||||||||
    tokens.push({ type: 'UNK', value: c });
    i++;
  }

  // Mostrar resultado||||||||||||||||||||||||||||||||||||||||||||||
  const out = tokens.map(t => `${t.type.padEnd(12)} : ${t.value}`).join('\n');
  document.getElementById('output').textContent = out;
}

