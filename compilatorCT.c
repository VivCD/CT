#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define SAFEALLOC(var, Type) \
    if ((var = (Type *)malloc(sizeof(Type))) == NULL) \
        err("not enough memory");

enum 
{
    ID,
    END,
    CT_INT,
    CT_REAL,
    CT_CHAR,
    CT_STRING,
    RETURN,
    DOUBLE,
    STRUCT,
    WHILE,
    BREAK,
    ELSE,
    VOID,
    CHAR,
    FOR,
    INT,
    IF,

    // Add more token codes as needed
}; 

typedef struct _Token{
    int code; // code (name)
    union{
        char *text; // used for ID, CT_STRING (dynamically allocated)
        long int i; // used for CT_INT, CT_CHAR
        double r; // used for CT_REAL
        };
    int line; // the input file line
    struct _Token *next; // link to the next token
}Token;

Token *tokens = NULL;    // Global variable to store tokens
Token *lastToken = NULL; // Global variable to store last token
int line = 1;            // Global variable to track current line number


Token *addTk(int code){
    Token *tk;
    SAFEALLOC(tk,Token)
    tk->code=code;
    tk->line=line;
    tk->next=NULL;
    if(lastToken){
        lastToken->next=tk;
    }else{
        tokens=tk;
    }
lastToken=tk;
return tk;
}

void err(const char *fmt,...){
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error: ");
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}

void tkerr(const Token *tk,const char *fmt,...){
    va_list va;
    va_start(va,fmt);
    fprintf(stderr,"error in line %d: ",tk->line);
    vfprintf(stderr,fmt,va);
    fputc('\n',stderr);
    va_end(va);
    exit(-1);
}

int getNextToken(const char *input){
    int state=0,nCh;
    char ch;
    const char *pStartCh = input;
    Token *tk;
    while(1){ // infinite loop
        ch= *input;
        switch(state){
            case 0 :
                if(isalpha(ch)||ch=='_'){
                    pStartCh=input; // memorizes the beginning of the ID
                    input++; // consume the character
                    state=1; // set the new state
                }

            case 1 :
                if(isalnum(ch)||ch=='_')
                    input++;
                else 
                    state=2;
                break;
            case 2:
                nCh=input-pStartCh; // the id length
                // keywords tests
                 if (nCh == 2 && !memcmp(pStartCh, "if", 2))
                addTk(IF);
            else if (nCh == 3 && !memcmp(pStartCh, "int", 3))
                addTk(INT);
            else if (nCh == 3 && !memcmp(pStartCh, "for", 3))
                addTk(FOR);
            else if (nCh == 4 && !memcmp(pStartCh, "char", 4))
                addTk(CHAR);
            else if (nCh == 4 && !memcmp(pStartCh, "void", 4))
                addTk(VOID);
            else if (nCh == 4 && !memcmp(pStartCh, "else", 4))
                addTk(ELSE);
            else if (nCh == 5 && !memcmp(pStartCh, "break", 5))
                addTk(BREAK);
            else if (nCh == 5 && !memcmp(pStartCh, "while", 5))
                addTk(WHILE);
            else if (nCh == 6 && !memcmp(pStartCh, "struct", 6))
                addTk(STRUCT);
            else if (nCh == 6 && !memcmp(pStartCh, "double", 6))
                addTk(DOUBLE);
            else if (nCh == 6 && !memcmp(pStartCh, "struct", 6))
                addTk(RETURN);
                
            else 
            {
                // if no keyword, then it is an ID
                addTk(ID);
                SAFEALLOC(tk->text, char);
                tk->text = strndup(pStartCh, nCh);
            }
            return lastToken->code;
        }
         return tk->code;


        }


}


void printTokens(Token *tokens) 
{
    while (tokens != NULL) 
    {
        printf("Token: %d", tokens->code);
        if (tokens->text != NULL) 
        {
            printf(" (%s)", tokens->text);
        }
        printf("\n");
        tokens = tokens->next;
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) 
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *input = (char *)malloc(size + 1);
    fread(input, sizeof(char), size, file);
    input[size] = '\0';

    fclose(file);

    int token;
    while ((token = getNextToken(input)) != END) 
    {
        // Process tokens if needed
    }

    printTokens(tokens);

    free(input);
    return 0;
}


