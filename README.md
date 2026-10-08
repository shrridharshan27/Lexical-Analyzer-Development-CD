# Comprehensive Lexical Analyzer in C

[![Language](https://img.shields.io/badge/Language-C99%20%2F%20C11-00599C.svg?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Compiler](https://img.shields.io/badge/Compiler-GCC%20%2F%20Clang-green.svg)](https://gcc.gnu.org/)
[![Course](https://img.shields.io/badge/Course-BCSE306L%20Compiler%20Design-red.svg)](https://vit.ac.in)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Author Information
- **Author:** Shrri Dharshan D R
- **GitHub:** [@shrridharshan27](https://github.com/shrridharshan27)
- **Registration Number:** `23BPS1090`
- **Course:** Compiler Design Laboratory (`BCSE306L`)
- **Lab Slot:** `L23+L24`

---

## Overview
This repository contains a full-featured, robust **Lexical Analyzer** implemented in standard C. It performs the first critical phase of a compiler by converting raw stream characters into categorized lexical tokens, creating and updating a **Symbol Table**, eliminating single-line (`//`) and multi-line (`/* */`) comments, and generating detailed token frequency metrics.

### Key Features
- **Token Classification**: Categorizes Keywords, Identifiers, Integer/Float Literals, String Literals, Compound Operators, and Delimiters.
- **Symbol Table Management**: Tracks unique user-defined identifiers along with the line number of their first declaration.
- **Comment Stripper**: Automatically parses and bypasses single-line (`// ...`) and multi-line (`/* ... */`) comments with proper line count preservation.
- **Error Diagnostics**: Gracefully detects and reports illegal characters and unterminated string literals.
- **Summary Metrics**: Produces an end-of-scan statistical summary of all encountered token categories.

---

## Token Specifications
| Category | Pattern / Examples | Description |
| :--- | :--- | :--- |
| **Keywords** | `int`, `float`, `char`, `if`, `else`, `while`, `return`, `var` | Reserved language tokens |
| **Identifiers** | `[a-zA-Z_][a-zA-Z0-9_]*` | User variables and function symbols |
| **Integer Constants** | `[0-9]+` (e.g. `10`, `42`) | Integral numeric literals |
| **Float Constants** | `[0-9]+\.[0-9]+` (e.g. `3.1415`, `0.5`) | Floating-point decimal values |
| **String Literals** | `"..."` | Quoted character sequences |
| **Operators** | `+`, `-`, `*`, `/`, `=`, `==`, `!=`, `<=`, `>=`, `&&`, `||` | Arithmetic, relational, logical operators |
| **Delimiters** | `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,` | Punctuation and statement terminators |

---

## Compilation & Execution

### Using GCC (MinGW / Linux / macOS)
```bash
# Compile the lexical analyzer
gcc -std=c99 -Wall -Wextra src/lexer.c -o build/lexer

# Run on a sample test file
./build/lexer tests/input1.c

# Run interactively
./build/lexer
```

### Sample Output
```
(KEYWORD, int)
(IDENTIFIER, total)
(OPERATOR, =)
(IDENTIFIER, price)
(OPERATOR, *)
(IDENTIFIER, quantity)
(DELIMITER, ;)
(KEYWORD, float)
(IDENTIFIER, discount)
(OPERATOR, =)
(IDENTIFIER, total)
(OPERATOR, *)
(FLOAT CONSTANT, 0.10)
(DELIMITER, ;)
...

========================================
             SYMBOL TABLE               
========================================
  total                | First seen at Line 1
  price                | First seen at Line 1
  quantity             | First seen at Line 1
  discount             | First seen at Line 3

========================================
             TOKEN SUMMARY              
========================================
  Keywords           : 4
  Identifiers        : 7
  Operators          : 5
  Literals/Constants : 2
  Delimiters         : 4
  Lexical Errors     : 0
========================================
```

---

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
