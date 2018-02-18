/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_PARSER_H
#define BULB_PARSER_H
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

/* Description
   ===========
   The parser generates a syntatic tree by the source code of the program.
   This process is subdivided in two parts:
    - Lexical analysis: the text is composed of tokens, which are selected by
                        the lexer.
    - Syntatical analysis: the tokens are analysed to compose the AST.  
   Example:  
   `(def a (+ 1 2)) => ["def", "a", ["+", 1, 2]]`  
   
   Token types
   -----------
   These types identify the tokens generated by the lexer. */
enum {
    BULB_TOK_NIL = 0, 
    BULB_TOK_OPEN_BLOCK, 
    BULB_TOK_CLOSE_BLOCK, 
    BULB_TOK_OTHER, 
};

/* bulbParse
   --------
   Acquires the AST from the string, the AST is made by cons. */
bulbObj *bulbParse(char *exp);

/* bulbIsInt
   ---------
   Verifies if the string can be converted to an integer (doesn't accept 
   decimals). */
bool bulbIsInt(char *exp, unsigned len);

/* bulbIsFloat
   -----------
   Verifies if the string can be converted to a float (decimal part is 
   needed). */
bool bulbIsFloat(char *exp, unsigned len);

/* bulbGenAtom
   -----------
   Converts the string to an atom object. */
bulbObj *bulbGenAtom(char *exp, unsigned len);

/* bulbLex
   -------
   Returns the type of the token contained in said string starting from its 
   offset. This only works if the conversion is possible, therefore if it's not 
   a parentesis, gets retured. */
unsigned bulbLex(char *exp, unsigned len, unsigned *offset, bulbObj **out);

/* bulbGenAst
   ----------
   Generates an AST starting from the provided offset. It also 
   checks if every open bracket matches a closed bracket. */
bulbObj *bulbGenAst(char *exp, unsigned len, unsigned *offset, bool open);

/* Exceptions
   ---------- */
void bulb_err_missing_close_block();

void bulb_err_missing_open_block();

#endif /* BULB_PARSER_H */
