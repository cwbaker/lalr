int yywrap()
{
	return(1);
}


void comment()
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}

int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}

extern char yytext[];
extern int column;
int yyparse(void);
int yylex (void);
void yyerror(const char *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}

typedef int YYINT;
static const int yylhs[] = {                           -1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    4,    4,    6,    6,    6,    6,    6,    6,
    7,    7,    7,    7,    7,    7,    8,    8,   10,   10,
   10,   10,   11,   11,   11,   12,   12,   12,   13,   13,
   13,   13,   13,   14,   14,   14,   15,   15,   16,   16,
   17,   17,   18,   18,   19,   19,   20,   20,    5,    5,
   21,   21,   21,   21,   21,   21,   21,   21,   21,   21,
   21,    2,    2,   22,   23,   23,   24,   24,   24,   24,
   24,   24,   25,   25,   29,   29,   26,   26,   26,   26,
   26,   27,   27,   27,   27,   27,   27,   27,   27,   27,
   27,   27,   27,   32,   32,   32,   34,   34,   35,   35,
   36,   37,   37,   37,   37,   38,   38,   39,   39,   39,
   33,   33,   33,   40,   40,   41,   41,   28,   28,   30,
   30,   43,   43,   43,   43,   43,   43,   43,   42,   42,
   42,   42,   46,   46,   44,   44,   47,   47,   48,   48,
   48,   45,   45,    9,    9,   49,   49,   49,   50,   50,
   50,   50,   50,   50,   50,   50,   50,   31,   31,   31,
   51,   51,   52,   52,   53,   53,   53,   54,   54,   54,
   56,   56,   56,   56,   55,   55,   55,   60,   60,   60,
   61,   61,   61,   62,   57,   57,   57,   57,   64,   64,
   63,   63,   58,   58,   59,   59,   59,   59,   59,    0,
    0,   65,   65,   66,   66,   66,   66,
};

static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,
0,0,0,0,0,0,0,"':'","';'","'<'","'='","'>'","'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"'{'","'|'","'}'","'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"IDENTIFIER",
"CONSTANT","STRING_LITERAL","SIZEOF","PTR_OP","INC_OP","DEC_OP","LEFT_OP",
"RIGHT_OP","LE_OP","GE_OP","EQ_OP","NE_OP","AND_OP","OR_OP","MUL_ASSIGN",
"DIV_ASSIGN","MOD_ASSIGN","ADD_ASSIGN","SUB_ASSIGN","LEFT_ASSIGN",
"RIGHT_ASSIGN","AND_ASSIGN","XOR_ASSIGN","OR_ASSIGN","TYPE_NAME","TYPEDEF",
"EXTERN","STATIC","AUTO","REGISTER","CHAR","SHORT","INT","LONG","SIGNED",
"UNSIGNED","FLOAT","DOUBLE","CONST","VOLATILE","VOID","STRUCT","UNION","ENUM",
"ELLIPSIS","CASE","DEFAULT","IF","ELSE","SWITCH","WHILE","DO","FOR","GOTO",
"CONTINUE","BREAK","RETURN",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,"illegal-symbol",
};

typedef struct {
    unsigned stacksize;
    int    *s_base;
    int    *s_mark;
    int    *s_last;
    int  *l_base;
    int  *l_mark;
} YYSTACKDATA;

void main()
{
  yyparse();
}
