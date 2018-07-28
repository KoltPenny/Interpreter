%{
#include "bison.tab.h"
%}

STR \"(\\.|[^"\\])*\"
ENT [0-9]+
DEC {ENT}*"."{ENT}
POW [Pp][Oo][Ww]
VAR [A-Za-z][A-Za-z0-9]*
IF [Ii][Ff]

%%


{ENT}       {
//								printf("Numero entero %s\n",yytext);
           			yylval.entero = atoi(yytext);
                return (ENTERO);
								}

{DEC}				{
                //printf("Numero decimal %s\n",yytext);
                yylval.decimal = atof(yytext);
                return (DECIMAL);
                }

"int"	 					{
            		//printf("Definición\n");
                return (INT);
                }

"str"	 					{
            		//printf("Definición\n");
                return (STR);
								}

"flt"	 					{
            		//printf("Definición\n");
                return (FLT);
                }

"quit()"				{
            		//printf("Definición\n");
                return (QUIT);
                }

"=" 						{
								yylval.sym = yytext;
								return(EQ);
								}

"+"|"-"|"*"|"/"|"%"|"("|")"|","|"\n"|";"|"["|"]"	 						{
  //          		printf("Símbolo\n");
                return (yytext[0]);
                }

"<"|">"|"=="|"<>" {
								//puts("COMPARADOR");
							  yylval.sym = yytext;
								return (COMPARE);
								}

{STR}						{
								int size = 0;
								while(yytext[size]!='\0')
								{
									size += 1;
								}
								char *c = malloc(size);
								for(int i = 1;i < size-1;i++)
								{
									c[i-1]=yytext[i];
								}
								c[size-1] = '\0';
								//printf("Cadena %s\n",c);
								yylval.cadena = c;
								return (CADENA);
								}								

{POW}						{
                //printf("Función POW\n");
								yylval.func = yytext;
								return (POW);
								}
								
{IF}						{
								//puts("IFFF");
								yylval.func = yytext;
								return(IF);
								}
								
{VAR}						{
								//printf("Variable\n");

								int size = 0;
								while(yytext[size]!=0){ size += 1; }
								
								char *c = malloc(size+1);
								for(int i = 0;i < size;i++)	{	c[i]=yytext[i];	}
								c[size] = '\0';
								yylval.var = c;
								return (VAR);
								}								

.								;
%%
