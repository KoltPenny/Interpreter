%{
#include <stdio.h>
#include "structs.h"

int yylex(void);
void yyerror (char *);
int strsize (char *, char *);
List *lista;
%}
             
/* Declaraciones de BISON */
%union{
		int entero;
		double decimal;
		char *cadena;
		char *var;
		char *func;
		char *sym;
		struct node *node;
 }

%token	<entero>				ENTERO
%type		<entero>				exp_entera
%token	<decimal>				DECIMAL
%type		<decimal>				exp_decimal
%token	<cadena>				CADENA
%type		<cadena>				exp_cadena
%token	<var>						VAR INT FLT STR
%type		<var>						nom_var
%token	<node>					EXPRESSION ASSIGN
%type		<node>					exp_var exp_var_assign
%token	<func>					POW QUIT IF
%token	<sym>						EQ COMPARE

%printer { fprintf (yyoutput, "%s", $$); } CADENA exp_cadena;
%printer { fprintf (ttoutput, "%s", $$); } VAR nom_var;

%left										'+''-'
%left		  							'*''/''%'


/* Gramática */
%%

input:    /* cadena vacía */
				| 			input line             {prompt();}
								;

line:     '\n'
								//Expresiones
				|				exp_var_assign '\n' { if ($1) printVar($1); }
								//Control
				|				error '\n' {yyerrok;}
				|				QUIT ';' '\n' { bye(); exit(0); }
								;

//
nom_var: 				VAR { $$ = $1; }
				|				VAR '[' exp_entera ']' { $$=returnName($1,$3,lista); }
								;
//Declaraciones y operaciones con variables
exp_var_assign:	ASSIGN { $$ = $1; }
				|				exp_var ';' { $$ = $1; }
								//ENTEROS
				| 			INT VAR ';' { $$ = createAppend(0,$2,0,0,NULL,0,lista); }
				|				INT VAR EQ exp_var ';' { $$ = replicate(0,$2,$4,lista,0); }
				|				INT VAR '[' exp_entera ']' ';' { $$ = createAppend(3,$2,0,0,NULL,$4,lista); }
								
								//FLOTANTES
				| 			FLT VAR ';' { $$=createAppend(1,$2,0,0,NULL,0,lista); }
				|				FLT VAR EQ exp_var ';' { $$ = replicate(1,$2,$4,lista,0); }
				|				FLT VAR '[' exp_entera ']' ';' { $$ = createAppend(3,$2,1,0,NULL,$4,lista); }

								//CADENAS
				| 			STR VAR ';' { $$=createAppend(2,$2,0,0,"",0,lista); }
				|				STR VAR EQ exp_var ';' { $$ = replicate(2,$2,$4,lista,0); }

								//MISC
				|				nom_var EQ exp_var ';' { $$ = replicate(0,$1,$3,lista,1); }
//				|				VAR '[' exp_entera ']' EQ exp_var ';' { $$ = NULL; }
								;

exp_var:				EXPRESSION { $$ = $1; }
				|				nom_var { Node *n = searchVar(lista,$1); $$= n ? n:zombieNode($1); }
				|				'('exp_var')' { $$ = $2; }

								//Declaraciones
				|				exp_entera { $$ = create_node(0,"0_ANON",$1,0,NULL,0); }
				|				exp_decimal { $$ = create_node(1,"0_ANON",0,$1,NULL,0); }
				|				exp_cadena { $$ = create_node(2,"0_ANON",0,0,$1,0); }
								
								//Operaciones
				|				exp_var '+' exp_var { $$ = oneUseAnonNodePlus($1,$3); }
				|				exp_var '-' exp_var { $$ = oneUseAnonNodeMinus($1,$3); }
				|				exp_var '*' exp_var { $$ = oneUseAnonNodeMulti($1,$3); }
				|				exp_var '/' exp_var { $$ = oneUseAnonNodeDiv($1,$3); }
				|				POW '(' exp_var ',' exp_var ')' { $$=powParse($3,$5); }
								;

//Operaciones de enteros
exp_entera:     ENTERO	{ $$ = $1;}	;
				|				'-'exp_entera { $$ = $2*(-1); }

//Operaciones de punto flotante
exp_decimal:		DECIMAL	{ $$ = $1; } ;
				|				'-'exp_decimal { $$ = $2*(-1); }

//Operaciones de cadena
exp_cadena:     CADENA	{$$ = $1; }
				|				IF '(' exp_var COMPARE exp_var ')' { $$ = customIF($3,$5,lista,$4); }
								;

%%
;
int main() {
		//Inicialización de la lista
		lista = listInit();
		//Impresión de mensajes y de prompt
		/*MENSAJE DE INICIO */ welcome();
		/*FLECHA DE PROMPT »*/ prompt();
		
		yyparse();
		return 0;
}

void yyerror (char *s)
{
  printf ("\n--%s--\n\n", s);
}
            
int yywrap()  
{
  return 1;  
}
