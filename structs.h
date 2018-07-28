#ifndef STR_FILE
#define STR_FILE
#include <stdlib.h>

//Mensajes de interfaz
void prompt() { printf(" » "); }//Prompt de línea
void welcome() {	puts("\n----------\nFINAL v0.4\n----------\n"); }//Barra de bienvenida
void undeclared(char *var_name) { printf("\n!: Undeclared variable '%s'\n\n",var_name); } //Variable sin declarar
void incompatible() { printf("\n!: Incompatible types\n\n"); }//Variables incompatibles
void invalidOp() { printf("\n!: Invalid operation for given type\n\n"); }//Operación no disponible
void forbiddenOp() { puts("\n!: Forbidden operation\n"); }//Operación prohibida
void exists(char *var_name) { printf("\n!: Variable '%s' already in use\n\n",var_name); }//Variable ya existe
void outofrange() { puts("\n!: Variable is out of range\n"); }
void bye() { puts("\n -- ENDED --\n"); }//Fin de programa


//Proto
char *sumiCadeni(char *, char *);

//Crea el nodo
typedef struct node
{
	unsigned char t_dato;
	char * nombre;
	int * t_int;
	int arr_size;
	double * t_double;
	char * t_string;
	struct node * next;
	struct node ** array;
} Node;

//Crea la lista
typedef struct list { Node *head; } List;

//regresa el tamaño de una cadena
int len(char * str){ int i=0; while(str[i]!='\0') i++; return i; }

//Compara dos cadenas
int strcompare (char *s1, char *s2)
{
	int size_1 = len(s1), size_2 = len(s2);
	int diff = size_1-size_2;

	if(diff>=0)
		{
			int check=0;
			for(int i = 0;i<size_2;i++){ check+=s1[i]!=s2[i];}
			return check;
		}
	else {return -1;}
}

//Compara si un string es exactamente igual a otro. Retorna un 0 si son iguales.
int precisionCompare (char *s1, char * s2)
{
	int size_1 = 0, size_2 = 0;
	while(s1[size_1]!='\0')	{size_1 += 1;}
	while(s2[size_2]!='\0')	{size_2 += 1;}

	int diff = size_1-size_2;

	if(diff==0)
	{
		unsigned int check=0;
		for(int i = 0;i<size_2;i++) {	check+=s1[i]!=s2[i]; }
		return check;
	}
	else {return -1;}
}

//Añade un elemento nuevo en el último lugar de la lista
Node *listAppend(List *list,Node * node)
{
	if(list->head==NULL){ list->head = node; return list->head; }
	Node *sentinel = list->head;
	while (sentinel->next != NULL) { sentinel = sentinel->next; }
	sentinel->next = node;
	return node;
}

//Imprime la lista
void printList(List *list)
{
	Node * sentinel = list->head;
	while(sentinel != NULL)
		{
			printf("{%s}\n",sentinel->nombre);
			sentinel = sentinel->next;
		}
}

//Regresa un valor double a partir de un nodo de referencia
double parseToDouble(Node *node)
{
	if(node->t_dato==0) return ((double) *node->t_int);
	else return *node->t_double;
}

//Busca si existe una variable con el nombre de name en la lista
Node * searchVar(List *list,char * name)
{	
	Node * sentinel = list->head;
	
	while(sentinel != NULL)
		{
			if(!precisionCompare(sentinel->nombre,name)) { return sentinel;	}
			sentinel = sentinel->next;
		}
	//undeclared(name);
	return NULL;
}

//Inicializa la lista
List * listInit()
{
	List *list = malloc(sizeof(List));
	list->head = NULL;
	return list;
}

//Nombre de elementos en arreglo
char * arrElemName(char * arr,int index)
{
	char *elemname=malloc(sizeof(char)*10);
	sprintf(elemname,"[%d]",index);
	return sumiCadeni(arr,elemname);
}

//Crea un nodo con los datos otorgados
Node *create_node(unsigned char dato, char *nombre, int t_int, double t_double, char *t_string,int arr_size)
{

	Node *newnode = malloc(sizeof(Node));
	newnode->next = NULL;

	int it = 0;
	
	newnode->nombre = (char *)malloc(sizeof(char)*(len(nombre)+1));
	
	while(nombre[it]!='\0')	{ newnode->nombre[it]=nombre[it]; it++; }
	newnode->nombre[it]=nombre[it];

	newnode->t_dato = dato;
	switch(dato)
		{
		case 0:			
			newnode->t_int = malloc(sizeof(int));
			*newnode->t_int = t_int;
			newnode->t_double = NULL;
			newnode->t_string = NULL;
			newnode->array = NULL;			
			break;
			
		case 1:
			
			newnode->t_double = malloc(sizeof(double));
			*newnode->t_double = t_double;
			newnode->t_int = NULL;
			newnode->t_string = NULL;
			newnode->array = NULL;
			break;
			
		case 2:
			
			it=0;
			
			newnode->t_string = (char *)malloc(sizeof(char)*(len(t_string)+1));
	
			while(t_string[it]!='\0')	{ newnode->t_string[it]=t_string[it]; it++; }
			newnode->t_string[it]=t_string[it];
			//printf("it: %d | last: %d",it,t_string[it]);

			newnode->t_int = NULL;
			newnode->t_double = NULL;
			newnode->array = NULL;
			break;
		case 3:
			newnode->t_int = NULL;
			newnode->t_double = NULL;
			newnode->t_string = NULL;
			newnode->arr_size = arr_size;
			newnode->array = malloc(sizeof(Node *)*arr_size);
			for(int i = 0; i<arr_size;i++)
				{
					newnode->array[i] = create_node(t_int,arrElemName(nombre,i),0,0,"",0);
				}
			
			break;
		}
	return newnode;
}

//Imprime un nodo de variable con formato
void printVar(Node *ret)
{
	//printf("\n#( name : %s , type : %d , value : ",ret->nombre,ret->t_dato);
	if(ret->t_dato==0) printf("\n #> %d\n\n",*ret->t_int);
	if(ret->t_dato==1) printf("\n #> %g\n\n",*ret->t_double);
	if(ret->t_dato==2) printf("\n #> \"%s\"\n\n",ret->t_string);
	if(ret->t_dato==3) printf("\n #> Array [%d]\n\n",ret->arr_size);
}

//Revisa si una variable se encuentra en la lista de variables
int varCheck(char *name,List *lista)
{
	Node *ret = searchVar(lista,name);
	if(ret!=NULL)
		{
			printVar(ret);
		} else { undeclared(name); return 1;}
}

//Crea y añade a la lista
Node *createAppend(unsigned char type, char *name, int t_int, double t_double, char *string,int arr_size,List *lista)
{
	Node *ret = searchVar(lista,name);
	if(ret==NULL)
		{
			ret = create_node(type,name,t_int,t_double,string,arr_size);
			listAppend(lista,ret);
			//return ret;
		}
	else {exists(name); return NULL;}

	if(type==3)
		{
			for(int i = 0; i < arr_size; i++)
				{
					listAppend(lista,ret->array[i]);
				}
		}

	return ret;
}

//Calcula la potencia para los números
double powDigit(double val, int pow)
{
  if(!pow && val) return 1;
	if(!pow && !val) { puts("\nEs un misterio sin resolver.\n\n"); }
	
	double exp = val;
	
	for(int i = 0; i<pow-1 ; i++)	{ exp *= val; }

	if (pow>0) return exp;
	else if (pow<0) return 1/exp;
	else return 1;
}

//Calcula la potencia para las expresiones tipo cadena
char *powString (char *exp_cadena, int exp_int)
{
	int size=len(exp_cadena);
	size_t tsz = (sizeof(char)*exp_int*size)+1;

	char *temp = malloc(tsz);
	char *back = malloc(sizeof(char)*size+1);
	
	for(int i = size-1,j=0; i>=0; i--,j++)
		{ back[j]=exp_cadena[i];	}
	
	back[size]='\0';
	
	if(exp_int>0)
		{
			for(int i = 0, j = 0; i<(int)tsz ; i++, j++)
				{
					if (j==size) j = 0;
					temp[i] = exp_cadena[j];
				}
			temp[(int)tsz-1]='\0';
		}
	else if(exp_int<0)
		{
			size = len(back); //while(back[size]!='\0')	{size += 1;}
			tsz = (sizeof(char)*(-1*exp_int)*size)+1;
			free(temp);
			temp = malloc(tsz);
			for(int i = 0, j = 0; i<(int)tsz ; i++, j++)
				{
					if (j==size) j = 0;
					temp[i] = back[j];
				}
			temp[(int)tsz-1]='\0';
		}
	return temp;
}

Node *powParse(Node *base, Node *exponent)
{
	if(exponent->t_dato)
		{
			invalidOp();
			return NULL;
		}
	Node *node;
	switch(base->t_dato)
		{
		case 0: node = create_node(1,"0_ANON",0,powDigit(parseToDouble(base),*exponent->t_int),NULL,0); break;
		case 1: node = create_node(1,"0_ANON",0,powDigit(*base->t_double,*exponent->t_int),NULL,0); break;
		case 2: node = create_node(2,"0_ANON",0,0,powString(base->t_string,*exponent->t_int),0); break;
		default: return NULL;
		}
	return node;
}

//Suma de cadenas
char *sumiCadeni(char *cad1, char *cad2)
{
	int size_1 = len(cad1), size_2 = len(cad2);
	
	char *c = malloc(size_1+size_2);
	int i = 0;
	while(cad1[i]!='\0'){c[i]=cad1[i]; i++;}
	while(cad2[i-size_1]!='\0'){c[i]=cad2[i-size_1]; i++;}
	
	return c;
}

//Resta de cadenas
char *restiCadeni(char *string, char *substring)
{
	if(!precisionCompare(string,substring))	{return "";}
	
	int size = len(substring);
	char *a=string, *b=substring;
	
	while (*a)
		{
			if(!strcompare(a,b))
				{
					while(*a) {	*a++=*(a+size);	} //Copia 'a' mientras el valor en 'a' no sea 0;
					*a++=*(a+size);
					break;
				}
			else {a++;}
		}
	return string;	
}

//Reemplaza el valor de una variable
Node *changeVal(unsigned char type, char *name,void *data,List *lista)
{
	Node *node = searchVar(lista,name);
	if(node!=NULL)
		{
			if(node->t_dato!=type) { incompatible(); return NULL;}
			switch(type)
				{
				case 0: *node->t_int = *(int *)data; break;
				case 1: *node->t_double = *(double *)data; break;
				case 2: node->t_string = (char *)data; break;
				}
			return node;
		}
	else { undeclared(name); }
	return NULL;
}

//Retorna un nodo deshechable cuando un valor es nulo
Node *zombieNode(char *name) { return create_node(255,name,0,0,NULL,0); }

Node *oneUseAnonNodePlus(Node *n1, Node *n2)
{
	
	if(!n1) { undeclared(""); return NULL; }
	if(!n2) { undeclared(""); return NULL; }
	if(n1->t_dato==n2->t_dato)
		{
			Node *node = create_node(n1->t_dato,"0_ANON",0,0,"",0);
			switch(node->t_dato)
				{
				case 0: *node->t_int = *n1->t_int + *n2->t_int; break;
				case 1: *node->t_double = *n1->t_double + *n2->t_double; break;
				case 2: node->t_string = sumiCadeni(n1->t_string,n2->t_string); break;
				}
			return node;
		}
	if(n1->t_dato<=1 && n2->t_dato<=1)
		{
			Node *node = create_node(1,"0_ANON",0,0,"",0);
			*node->t_double = parseToDouble(n1) + parseToDouble(n2);
			return node;
		}
	incompatible();
	return NULL;
}

Node *oneUseAnonNodeMinus(Node *n1, Node *n2)
{
	
	if(!n1) { undeclared(""); return NULL; }
	if(!n2) { undeclared(""); return NULL; }
	if(n1->t_dato==n2->t_dato)
		{
			Node *node = create_node(n1->t_dato,"0_ANON",0,0,"",0);
			switch(node->t_dato)
				{
				case 0: *node->t_int = *n1->t_int - *n2->t_int; break;
				case 1: *node->t_double = *n1->t_double - *n2->t_double; break;
				case 2: node->t_string = restiCadeni(n1->t_string,n2->t_string); break;
				}
			return node;
		}
	if(n1->t_dato<=1 && n2->t_dato<=1)
		{
			Node *node = create_node(1,"0_ANON",0,0,"",0);
			*node->t_double = parseToDouble(n1) - parseToDouble(n2);
			return node;
		}
	incompatible();
	return NULL;
}

Node *oneUseAnonNodeMulti(Node *n1, Node *n2)
{
	
	if(!n1) { undeclared(""); return NULL; }
	if(!n2) { undeclared(""); return NULL; }
	
	if(n1->t_dato==n2->t_dato)
		{
			Node *node = create_node(n1->t_dato,"0_ANON",0,0,"",0);
			switch(node->t_dato)
				{
				case 0: *node->t_int = *n1->t_int * *n2->t_int; break;
				case 1: *node->t_double = *n1->t_double * *n2->t_double; break;
				case 2: invalidOp(); break;
				}
			return node;
		}
	if(n1->t_dato<=1 && n2->t_dato<=1)
		{
			Node *node = create_node(1,"0_ANON",0,0,"",0);
			*node->t_double = parseToDouble(n1) * parseToDouble(n2);
			return node;
		}
	incompatible();
	return NULL;
}

Node *oneUseAnonNodeDiv(Node *n1, Node *n2)
{
	
	if(!n1) { undeclared(""); return NULL; }
	if(!n2) { undeclared(""); return NULL; }
	
	if(n1->t_dato==n2->t_dato)
		{
			Node *node = create_node(n1->t_dato,"0_ANON",0,0,"",0);
			switch(node->t_dato)
				{
				case 0:
					if(*n2->t_int)
						{
							*node->t_int = *n1->t_int / *n2->t_int;
						}
					else { forbiddenOp(); return NULL; }
					break;
				case 1: *node->t_double = *n1->t_double / *n2->t_double; break;
				case 2: invalidOp(); break;
				}
			return node;
		}
	if(n1->t_dato<=1 && n2->t_dato<=1)
		{
			Node *node = create_node(1,"0_ANON",0,0,"",0);
			double dn1 = parseToDouble(n1);
			double dn2 = parseToDouble(n2);
			if(!dn2) { forbiddenOp(); return NULL; }
			*node->t_double = parseToDouble(n1) / parseToDouble(n2);
			return node;
		}
	incompatible();
	return NULL;
}

Node *copyVar(unsigned char type, char * name,char *oldname,List *lista,unsigned char assigned)
{
	if(!assigned)
		{	
			Node *oldnode =searchVar(lista,oldname);
			if(!oldnode)
				{
					undeclared(oldname);
					return NULL;
				}
			if(type==oldnode->t_dato)
				{
					Node *newnode = createAppend(type,name,*oldnode->t_int,*oldnode->t_double,oldnode->t_string,0,lista);
					return newnode;
				}
			incompatible();
			return NULL;
	}
	Node *n1 = searchVar(lista,name);
	Node *n2 = searchVar(lista,oldname);

	if(!n1) { undeclared(name);return NULL; }
	if(!n2) { undeclared(oldname);return NULL; }

	if(n1->t_dato==n2->t_dato)
		{
			switch(n1->t_dato)
				{	
				case 0: *n1->t_int=*n2->t_int; break;
				case 1: *n1->t_double=*n2->t_double; break;
				case 2: n1->t_string=n2->t_string; break;
				}
			return n1;
		}
	incompatible();
	return NULL;
}

Node *replicate(unsigned char type, char * name, Node *node, List *lista,unsigned char assigned)
{
	if(node->t_dato==255) { undeclared(node->nombre); free(node); return NULL; }
	if(!assigned)
		{
			if(searchVar(lista,name)) { exists(name); return NULL; }
			Node *newnode;
			if(node && node->t_dato==type)
				{
					switch(type)
						{	
						case 0:	newnode = create_node(0,name,*node->t_int,0,NULL,0); break;
						case 1:	newnode = create_node(1,name,0,*node->t_double,NULL,0); break;
						case 2:	newnode = create_node(2,name,0,0,node->t_string,0); break;
						}
					return listAppend(lista,newnode);
				}
			if( type == 1 && node->t_dato<=1 )
				{
					newnode = create_node(1,name,0,parseToDouble(node),NULL,0);
					return listAppend(lista,newnode);
				}
			incompatible();
			return NULL;
		}
	
	Node *old = searchVar(lista,name);
	if(old && node && old->t_dato==node->t_dato)
		{
			switch(old->t_dato)
				{	
				case 0: *old->t_int=*node->t_int; break;
				case 1: *old->t_double=*node->t_double; break;
				case 2: old->t_string=node->t_string; break;
				}
			return old;
		}
	if(!old) { puts("No existe."); undeclared(name); return NULL; }
	if(old->t_dato!=node->t_dato) { incompatible(); return NULL; }
	return NULL;
}

char *customIF(Node *n1, Node *n2,List *lista,char * symbol)
{
	
	if(n1 && precisionCompare(n1->nombre,"0_ANON"))
		{
			if(!searchVar(lista,n1->nombre)){	undeclared(n1->nombre);return NULL;}
		}

	if(n2 && precisionCompare(n2->nombre,"0_ANON"))
		{
			if(!searchVar(lista,n2->nombre))	{undeclared(n2->nombre);return NULL;}
		}
	
	if(n1->t_dato<=1 && n2->t_dato <= 1)
		{
			if(!strcompare(symbol,"<")) { if(parseToDouble(n1) < parseToDouble(n2)) return "true"; else return "false"; } 
			if(!strcompare(symbol,">")) { if(parseToDouble(n1) > parseToDouble(n2)) return "true"; else return "false"; }
			if(!strcompare(symbol,"==")) { if(parseToDouble(n1) == parseToDouble(n2)) return "true"; else return "false"; }
		}
	else if(n1->t_dato==n2->t_dato && n1->t_dato == 2)
		{
			if(!precisionCompare(symbol,"<"))
				{
					if(strcompare(n1->t_string,n2->t_string) && len(n1->t_string) < len(n2->t_string)) return "true";
					else return "false";
				} 
			if(!precisionCompare(symbol,">"))
				{
					if(strcompare(n1->t_string,n2->t_string) && len(n1->t_string) > len(n2->t_string)) return "true";
					else return "false";
				}
			if(!strcompare(symbol,"=="))
				{
					if(!precisionCompare(n1->t_string,n2->t_string)) return "true";
					else return "false";
				}
		}
	else return "";
}

char * returnName(char *varname,int index,List *lista)
{
	Node *node = searchVar(lista,varname);
	if(node)
		{
			if(node->arr_size>index)
				{
					return arrElemName(varname,index);
				}
			else
				{
					outofrange();
					return varname;
				}
		}
	else
		{
			return varname;
		}
}

int nodeVerify(Node *node, List *lista)
{
	if(!node)
		{
			if(precisionCompare(node->nombre,"") && precisionCompare(node->nombre,"0_ANON")) { varCheck(node->nombre,lista); }
			else if (!precisionCompare(node->nombre,"0_ANON")) { printVar(node); }
			return 1;
		}
	return 0;
}

#endif
