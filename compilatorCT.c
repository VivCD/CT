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
    SPACE,
    LINE_COMMENT,
    COMMENT,
    COMMA,
    SEMICOLON,
    LPAR,
    RPAR,
    LBRACKET,
    RBRACKET,
    LACC,
    RACC,
    ADD,
    SUB,
    MUL,
    DIV,
    DOT,
    AND,
    OR,
    NOT,
    EQUAL,
    ASSIGN,
    NOTEQ,
    LESSQ,
    LESS,
    GREATERQ,
    GREATER,
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
    const char *pStartCh;
    Token *tk;
    while(1){ // infinite loop
        ch= *input;
        switch(state){
            case 0 :
                if(isalpha(ch)||ch=='_'){
                pStartCh=input; // memorizes the beginning of the ID
                input++; // consume the character
                state=1; // set the new state
                }else if(ch==' '||ch=='\r'||ch=='\t'){
                    pStartCh=input; 
                    input++; 
                    state=28;
                }else if(ch=='\n'){ // handled separately in order to update the current line
                    line++;
                    pStartCh=input; 
                    input++; 
                    state=28;
                }else if(ch==0){ // the end of the input string
                addTk(END);
                return END;
                }else if(isdigit(ch) && ch >= '1' && ch <= '9'){
                    pStartCh=input; 
                    input++; 
                    state=3;
                }else if(ch=='0'){ // the end of the input string
                    pStartCh=input; 
                    input++; 
                    state=5;
                }else if(ch=='\''){
                    pStartCh=input;
                    input++;
                    state=14;
                }else if(memcmp(input, "\\", 2) == 0){
                    pStartCh=input;
                    input++;
                    state=19;
                }else if(ch=='/'){
                    pStartCh=input;
                    input++;
                    state=57;
                }else if(memcmp(input, "//", 2) == 0){
                    pStartCh=input;
                    input+=2;
                    state=29;
                }else if(ch=='\''){
                    pStartCh=input;
                    input++;
                    state=31;
                }else if(ch==';'){
                    pStartCh=input;
                    input++;
                    state=32;
                }else if(ch=='('){
                    pStartCh=input;
                    input++;
                    state=33;
                }else if(ch==')'){
                    pStartCh=input;
                    input++;
                    state=34;
                }else if(ch=='['){
                    pStartCh=input;
                    input++;
                    state=35;
                }else if(ch==']'){
                    pStartCh=input;
                    input++;
                    state=36;
                }else if(ch=='{'){
                    pStartCh=input;
                    input++;
                    state=37;
                }else if(ch=='}'){
                    pStartCh=input;
                    input++;
                    state=38;
                }else if(ch=='+'){
                    pStartCh=input;
                    input++;
                    state=39;
                }else if(ch=='-'){
                    pStartCh=input;
                    input++;
                    state=40;
                }else if(ch=='*'){
                    pStartCh=input;
                    input++;
                    state=41;
                }else if(ch=='/'){
                    pStartCh=input;
                    input++;
                    state=42;
                }else if(ch=='.'){
                    pStartCh=input;
                    input++;
                    state=43;
                }else if(memcmp(input, "&&", 2) == 0){
                    pStartCh=input;
                    input+=2;
                    state=44;
                }else if(memcmp(input, "||", 2) == 0){
                    pStartCh=input;
                    input+=2;
                    state=45;
                }else if(ch=='!'){
                    pStartCh=input;
                    input++;
                    state=46;
                }else if(ch=='='){
                    pStartCh=input;
                    input++;
                    state=47;
                }else if(memcmp(input, "!=", 2) == 0){
                    pStartCh=input;
                    input++;
                    state=50;
                }else if(ch=='<'){
                    pStartCh=input;
                    input++;
                    state=51;
                }else if(ch=='>'){
                    pStartCh=input;
                    input++;
                    state=54;
                }
                else tkerr(addTk(END),"invalid character");
            break;

            case 1 :
                if(isalnum(ch)||ch=='_'){
                    input++;
                    
                }
                else {

                    state=2;
                }
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
                tk = addTk(ID);
                tk->text = strndup(pStartCh, nCh);
                
            }
            return tk->code; 

           case 3:
               if(isdigit(ch))
                   input++;
                else if(ch=='e'||ch=='E'){
                    input++;
                    state=10;
                }
                else if(ch=='.'){
                    input++;
                    state=8;
                }
                else 
                    state=4;
            break;

            case 4: 
                tk = addTk(CT_INT);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;   

            case 5:
                if(isdigit(ch) && ch <= '7')
                   input++;
                else if(ch=='8'||ch=='9')
                    state=3;
                else if(ch == 'x'){
                    input++;
                    state=6;
                }
                else if(ch=='.'){
                    input++;
                    state=8;
                }
                else if(ch=='e'||ch=='E'){
                    input++;
                    state=10;
                }
                else 
                    state=4;
             break;

            case 6:
                if ((ch >= '0' && ch <= '9') || ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))){
                    input++;
                    state=7;
                }else {
                    // Handle invalid character after '0x'
                    tkerr(addTk(END), "invalid hexadecimal digit after '0x'");
                    return -1; // Return error code
                }
            break;

            case 7:
                 if ((ch >= '0' && ch <= '9') || ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))){
                    input++; 
                }
                else {

                    state=4;
                }
            break;

            case 8:
                if(isdigit(ch)){
                    input++;
                    state=9;
                }
                else
                  state=13;
            break;

            case 9:
                if(isdigit(ch))
                   input++;
                else if(ch=='e'||ch=='E'){
                    input++;
                    state=10;
                }
            break;

            case 10:
                if(isdigit(ch)){
                    input++;
                    state=12;
                }
                else if(ch=='-'||ch=='+'){
                    input++;
                    state=11;
                }
            break;

            case 11:
                if(isdigit(ch)){
                    input++;
                    state=12;
                }
            break;
                
            case 12:
                if(isdigit(ch))
                   input++;
                else 
                    state=13;
            break;

            case 13:
                tk = addTk(CT_REAL);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code; 

            case 14:
                if(memcmp(input, "//", 2) == 0){
                    input+=2;
                    state=15;
                }
                else if(ch=='^'||ch=='\''){
                    input++;
                    state=18;
                }else if(ch=='\\'){
                    input+=2;
                    state=18;
                }
            break;
            
            case 15:
                if(ch=='a'||ch=='b'||ch=='f'||ch=='m'||ch=='n'||ch=='t'||ch=='v'||ch=='\''||ch=='?'||ch=='"'||ch=='0'){
                    input++;
                    state=16;
                }else if(ch=='\\'){
                    input+=2;
                    state=16;
                }
            break;

            case 16:
                if(ch=='\''){
                    input++;
                    state=17;
                }
            break;

            case 17:
               tk = addTk(CT_CHAR);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 18:
                 if(ch=='\''){
                    input++;
                    state=17;
                }
            break;

            case 19:
                 if(ch=='a'||ch=='b'||ch=='f'||ch=='m'||ch=='n'||ch=='t'||ch=='v'||ch=='\''||ch=='?'||ch=='"'||ch=='0'){
                    input++;
                    state=20;
                }else if(ch=='\\'){
                    input+=2;
                    state=20;
                }else if(ch=='/^'||ch=='"'){
                    input++;
                    state=22;
                }
            break;

            case 20:
                if(ch=='a'||ch=='b'||ch=='f'||ch=='m'||ch=='n'||ch=='t'||ch=='v'||ch=='\''||ch=='?'||ch=='"'||ch=='0'){
                    input++;
                }else if(ch=='"'){
                    input++;
                    state=21;
                }
            break;

            case 21:
               tk = addTk(CT_STRING);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 22:
                if(ch=='/^'||ch=='"'){
                    input++;
                }else if(ch=='"'){
                    input++;
                    state=21;
                }
            break;

            case 23:
                if(ch=='*'){
                    input++;
                    state=24;
                }else if (ch=='/^*'){
                    input+=2;
                    state=25;
                }else{
                    input++;
                    state=25;
                }
            break;

            case 24:
                if(ch=='*'){
                    input++;
                }else if(ch=='/^'||ch=='/'){
                    input++;
                    state=25;
                }
            break;

            case 25:
                if(ch=="*"){
                    input++;
                    state=26;
                }else{
                    input++;
                    state=23;
                }
            break;

            case 26:
                if(ch=='*'){
                    input++;
                }else if(ch=='/'){
                    input++;
                    state=27;
                }
            break;

            case 27:
               tk = addTk(COMMENT);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 28:
               tk = addTk(SPACE);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 29:
                if(ch=='/^'||ch=='\n'||ch=='\r'||ch=='\0'){
                    input++;
                    
                }
                else {

                    state=30;
                }
            break;

            case 30:
                tk = addTk(LINE_COMMENT);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 31:
                tk = addTk(COMMA);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 32:
                tk = addTk(SEMICOLON);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;
            
            case 33:
                tk = addTk(LPAR);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 34:
                tk = addTk(RPAR);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 35:
                tk = addTk(LBRACKET);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 36:
                tk = addTk(RBRACKET);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 37:
                tk = addTk(LACC);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 38:
                tk = addTk(RACC);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 39:
                tk = addTk(ADD);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 40:
                tk = addTk(SUB);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 41:
                tk = addTk(MUL);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 42:
                tk = addTk(DIV);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 43:
                tk = addTk(DOT);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 44:
                tk = addTk(AND);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 45:
                tk = addTk(OR);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 46:
                tk = addTk(NOT);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 47:
                if(ch=='='){
                    state=48;
                }
                else{
                    state=49;

                }
            break;

            case 48:
                tk = addTk(EQUAL);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;    

            case 49:
                tk = addTk(ASSIGN);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 50:
                tk = addTk(NOTEQ);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 51:
                if(ch=='='){
                    state=52;
                }
                else{
                    state=53;

                }
            break;

            case 52:
                tk = addTk(LESSQ);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 53:
                tk = addTk(LESS);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;
            
            case 54:
                if(ch=='='){
                    state=55;
                }
                else{
                    state=56;

                }
            break;

            case 55:
                tk = addTk(GREATERQ);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;

            case 56:
                tk = addTk(GREATER);
                tk->text = strndup(pStartCh, nCh);    
            return tk->code;  

            case 57:
                if(ch=='*'){
                    input++;
                    state=23;
                }
            break;
        }
    }

}


void printTokens(Token *tokens){
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
    if (argc <2) 
    {
        printf("Usage: %s <input_string>\n", argv[0]);
        return 1;
    }

   /* FILE *file = fopen(argv[1], "r");
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
    */
    int token;
    
    for (int i = 1; i<argc; i++){
    token = getNextToken(argv[i]);
        switch (token) 
        {
            case ID:
                printf("Identifier: %s\n", lastToken->text);
                break;
            case CT_INT:
                printf("INT number: %s\n", lastToken->text);
                break;
            case CT_REAL:
                printf("REAL number: %s\n", lastToken->text);
                break;
            case CT_CHAR:
                printf("CHAR: %s\n", lastToken->text);
                break;
            case BREAK:
                printf("space: %s\n", lastToken->text);
                break;
            case COMMA:
                printf("COMMA: %s\n", lastToken->text);
                break;
             case SEMICOLON:
                printf("SEMICOLON: %s\n", lastToken->text);
                break;
            case LPAR:
                printf("LPAR: %s\n", lastToken->text);
                break;    
            case RPAR:
                printf("RPAR: %s\n", lastToken->text);
                break;
            case LBRACKET:
                printf("LBRACKET: %s\n", lastToken->text);
                break;  
            case RBRACKET:
                printf("RBRACKET: %s\n", lastToken->text);
                break;
            case LACC:
                printf("LACC: %s\n", lastToken->text);
                break; 
            case RACC:
                printf("RACC: %s\n", lastToken->text);
                break;
            case ADD:
                printf("ADD: %s\n", lastToken->text);
                break; 
            case SUB:
                printf("SUB: %s\n", lastToken->text);
                break; 
            case MUL:
                printf("MUL: %s\n", lastToken->text);
                break;
            case DIV:
                printf("DIV: %s\n", lastToken->text);
                break;
            case DOT:
                printf("DOT: %s\n", lastToken->text);
                break;
            case AND:
                printf("AND: %s\n", lastToken->text);
                break;
            case OR:
                printf("OR: %s\n", lastToken->text);
                break;
            case NOT:
                printf("NOT: %s\n", lastToken->text);
                break;
            case NOTEQ:
                printf("NOTEQ: %s\n", lastToken->text);
                break;
            case LINE_COMMENT:
                printf("LINE_COMMENT: %s\n", lastToken->text);
                break; 
            case EQUAL:
                printf("EQUAL: %s\n", lastToken->text);
                break;  
            case ASSIGN:
                printf("ASSIGN: %s\n", lastToken->text);
                break;
            case LESSQ:
                printf("LESSQ: %s\n", lastToken->text);
                break;
            case LESS:
                printf("LESS: %s\n", lastToken->text);
                break;
            case GREATERQ:
                printf("GREATERQ: %s\n", lastToken->text);
                break;
            case GREATER:
                printf("GREATER: %s\n", lastToken->text);
                break;
            case COMMENT:
                printf("OMMENT: %s\n", lastToken->text);
                break;                        
            default:
                printf("Token code: %d\n", token);
        }
    }

    printTokens(tokens);

    //free(input);
    return 0;
}
