#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#include"chtbl.h"
#include"lex.h"
#include"symbol.h"

Token lex(const char *istream, CHTbl *symbol)
{
    Token token;
    Symbol *symbol;
    int length, retval, i;

    if ((symbol = (Symbol *)malloc(sizeof(Symbol))) == NULL)
        return error;

    if ((symbol->lexeme = next_token(istream)) == NULL) {
        free(symbol);
        return lexit;
    } else {
        symbol->token = digit;
        length = strlen(symbol->lexeme);

        for (i = 0; i < length; i++) {
            if (!isdigit(symbol->lexeme[i]))
                symbol->token = other;
        }

        memcpy(&token, &symbol->token, sizeof(Token));

        if ((retval = chtbl_insert(symtbl, symbol)) < 0) {
            free(symbol);
            return error;
        } else if (retval == 1) {
            free(symbol);
        }
    }

    return token;
}
