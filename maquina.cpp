#include <stdio.h>
#include <cctype>
#include <string.h>
#include <iostream>


enum tipo_De_toke{
 AND=256,
 BREAK=257,
 DO=258,
 ELSEIF=259,
 END=260,
 FALSE =261,
 FOR =262,
 FUNCTION =263,
 IN  =264,
 LOCAL =265,
 NIL =266,
 NOT =267,
 OR =268,
 REPEAT =269,
 RETURN =270,
 TRUE =271,
 UNTIL =272,
 IF  =273,
 THEN  =274,
 ELSE  =275,
 RELOP  =276,
 ID  =277,
 NUM  =278,
 WHILE =279,

 //TOKEN DE DELIMITACAO
 L_PAREN =281,
 R_PAREN =282,
 L_CURLY =283,
 R_CURLY =284,
 L_BRACK =285,
 R_BRACK =286,
 SEMICOLON =287,
 COLON =288,
 COMMA =289,
 DOT =290,
 TWO_DOT =291,
 //TOKEN DE ARITMETICA
 ASSIGN =292,
 ADD =293,
 SUB =294,
 MUL =295,
 DIV =296,
 POW =297,
 LT =298,
 LE =299,
 EQ =300,
 NE =301,
 GT =302,
 GE =303,
 STRING=304
};


using namespace std;


struct Token{
 int nome_token;
 int atributo;
};
int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;


char *readFile(const char *fileName)
{
	FILE *file = fopen(fileName, "r");
	char *code;
	int n = 0;
	int c;

	if(file == NULL){return NULL;}
	
	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	code = new char (f_size+1);
	
	while ((c = fgetc(file))!= EOF)
	{
		code[n++] = (char) c;
	}
	
	code[n++] = (char)'\0';
	fclose(file);
	return code;
}


int isdelimination(char c){
    switch(c){
        case '(':{return 1;}
        case ')':{return 1;}
        case '[':{return 1;}
        case ']':{return 1;}
        case '{':{return 1;}
        case '}':{return 1;}
        case ';':{return 1;}
        case '.':{return 1;}
        case ',':{return 1;}
		case ':':{return 1;}
        default:{return 0;}
    }
}
int isoperation(char c)
{
    switch(c){
        case '=':{return 1;break;}
        case '+':{return 1;break;}
        case '-':{return 1;break;}
        case '*':{return 1;break;}
        case '/':{return 1;break;}
        case '^':{return 1;break;}
        case '>':{return 1;break;}
        case '<':{return 1;break;}
        default:{return 0;break;}
    }
}

int falhar()
{
	switch(estado)
	{
	case 0: partida = 11; break;
	case 11: partida = 42; break;
	case 42: partida = 44; break;
	case 44: partida = 51; break;
	case 51: partida = 64; break;
	case 64: partida = 66; break;

	case 120:
		//retornar msg de erro
		printf("Erro encontrado no codigo\n");
		cont_sim_lido++;
		partida=68;
		break;

	default: printf("Erro do compilador");
	}
	return(partida);
}

Token proximo_token()
{
	Token token;
	char c=code[cont_sim_lido];
	
	
	while(c!='\0')
    {
		
       switch(estado)
       {
        	case 0:
			{
				c = code[cont_sim_lido];
				while((c == ' ')||(c == '\n'))
				{
					
					estado = 0;	
					cont_sim_lido++;
					c = code[cont_sim_lido];
				}
				if(c=='\0') return token;
				else if(c == '<') estado = 1;
				else if(c == '=') estado = 5;
				else if(c == '>') estado = 6;
				else if(c=='~') estado =3;
				else
					{
					 estado = falhar();
					}
				break;
			}
			case 1:
				cont_sim_lido++;
				c = code[cont_sim_lido];

				if(c == '=') estado = 2;
				else if(c == '>') estado = 3;
				else estado = 4;
				break;

			case 2:

				
				cont_sim_lido++;
				printf("<relop, LE>\n");
				token.nome_token = RELOP;
				token.atributo = LE;
				estado = 0;
				return(token);
				break;

			case 3:
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='=')
				{
				cont_sim_lido++;	
				printf("<relop, NE>\n");
				token.nome_token = RELOP;
				token.atributo = NE;
				estado = 0;
				return(token);
				}
				
				else{ estado =120; estado = falhar();}
				break;

			case 4:
				cont_sim_lido++;
				printf("<relop, LT>\n");
				token.nome_token = RELOP;
				token.atributo = LT;
				estado = 0;
				return(token);
				break;

			case 5:
			{
				cont_sim_lido++;
				c = code[cont_sim_lido];
				
				if(c == '=') estado = 9;
				
				else {estado=10;}
				break;
			}
			case 6:
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c == '=') estado = 7;
				else estado = 8;
				break;

			case 7:
				cont_sim_lido++;
				printf("<relop, GE>\n");
				token.nome_token = RELOP;
				token.atributo = GE;
				estado = 0;
				return(token);
				break;

			case 8:
				cont_sim_lido++;
				printf("<relop, GT>\n");
				token.nome_token = RELOP;
				token.atributo = GT;
				//ATENÇÃO - CORREÇÃO: foi acrescentado o comando "estado = 0;"
				estado = 0;
				return(token);
				break;
			case 9:
			{
				cont_sim_lido++;
				printf("<relop, EQ>\n");
				token.nome_token = RELOP;
				token.atributo = EQ;
				estado = 0;
				return(token);
				break;
			}

			case 10:
			{
				
				printf("<ASSIGN, EQ>\n");
				token.nome_token = ASSIGN;
				token.atributo = ASSIGN;
				estado = 0;
				return(token);
				break;
			}
			case 11:
			{
				if(isdigit(c)!=0 || isalpha(c)==0){ estado = falhar();}
				
				else{ estado =12;}
				
				break;
			}

			case 12:
			{
				switch(c)
				{
					case 'i':{estado = 13; break;}//feito
					case 'a':{estado = 16; break;}//feito
					case 'b':{estado = 69; break;}
					case 'd':{estado = 17; break;}//feito
					case 'e':{estado = 19; break;}//feito
					case 'f':{estado = 22; break;}//feito
					case 'l':{estado = 26; break;}//feito
					case 'n':{estado = 27; break;}//feito
					case 'o':{estado = 30; break;}//feito
					case 'r':{estado = 32; break;}//feito
					case 't':{estado = 36; break;}//feito
					case 'u':{estado = 39; break;}//feito
					case 'w':{estado = 40; break;}//feito
					default: {estado = 41;break;}//feito
				}
				break;
			}
			case 13:
			{	
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if (c=='f'){estado=14;}
				else if( c=='n'){ estado =15;}
				else{estado = 41;}
				
				break;
			}
			case 14:
			{
				
				if(isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{	cont_sim_lido++;
					token.atributo=IF;
					printf("<IF>\n");
					estado =0;
					return token;
					
				}
				else{
					estado=41;
				}
				break;
			}
			case 15:
			{
				if(isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;	
					token.atributo=IN;
					printf("<IN>\n");
					estado =0;
					return token;
				}
				else
				{
					estado=41;
				}	
				break;
			}
			case 16:
			{
				bool eh_and=true;
				char * comparator;
				comparator= new char[3];
				strcpy(comparator,"nd");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i]){
						eh_and=false;
						break;
					}
				}
				delete(comparator);
				if(eh_and==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado =0;
					printf("<AND>\n");
					token.atributo=AND;
					return(token);
					break;
				}
				else{estado=41;}
				break;
			}
			case 17:
			{
				
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c=='o'){ estado =18;}
				else{ estado =41;}
				break;
			}
			case 18:
			{
				if(isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado=0;
					printf("<DO>\n");
					token.atributo=DO;
					return(token);
				}
				else{estado =41;}	
				break;
			}
			case 19:
			{
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c=='l'){estado =20;}
				else if(c=='n'){ estado= 21;}
				else{ estado=41;}
				break;
			}
			case 20:
			{
				bool eh_else=true;
				char * comparator;
				comparator= new char[3];
				strcpy(comparator,"se");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_else =false;
					}
				}
				delete(comparator);

				if(eh_else==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado = 0;
					printf("<ELSE>\n");
					token.atributo=ELSE;
					return (token);
					
				}
				
				
				else if(eh_else==true && code[cont_sim_lido+1]=='i' && code[cont_sim_lido+2]=='f' &&
				 isalnum(code[cont_sim_lido+3])==0 &&code[cont_sim_lido+3]!='_')
				{
					cont_sim_lido++;
					cont_sim_lido++;
					cont_sim_lido++;
					estado =0;
					printf("<ELSEIF>\n");
					token.atributo=ELSEIF;
					return (token);
					
				}
				else
				{
					estado = 41;
				}
				break;
			}
			case 21:
			{
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c=='d' && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=END;
					estado =0;
					printf("<END>\n");
					return (token);
					
				}
				else{estado = 41;}
				break;
			}
			case 22:
			{
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c=='o'){ estado = 23;}
				else if(c=='u'){estado=24;}
				else if(c=='a'){estado=25;}
				else{ estado = 41;}
				break;
			}
			case 23:
			{
				cont_sim_lido++;
				c = code[cont_sim_lido];
				if(c=='r' && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo = FOR;
					printf("<FOR>\n");
					estado =0;
					return token;
					
				}
				else{ estado=41;}
				break;
			}
			case 24:
			{
				bool eh_function=true;
				char* comparator;
				comparator= new char[7];
				strcpy(comparator,"nction");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					
					if(c!=comparator[i])
					{
						eh_function=false;
					}
				}
					delete(comparator);
					
					if(eh_function==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
					{
						cont_sim_lido++;
						estado =0;
						printf("<FUNCTION>\n");
						token.atributo=FUNCTION;
						return (token);
						
					}
					else{estado=41;}
					break;
			}
	
			case 25:
			{
				bool eh_false=true;
				char * comparator;
				comparator= new char[4];
				strcpy(comparator,"lse");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_false=false;
					}
				}
				
				delete(comparator);
				
				if(eh_false==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado =0;
					printf("<FALSE>\n");
					token.atributo=FALSE;
					return (token);
					break;
				}
				else{estado = 41;}
				break;
			}
			
			case 26:
			{
				bool eh_local = true;
				char * comparator;
				comparator = new char[5];
				strcpy(comparator,"ocal");
				for(int i=0; i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_local=false;
					}
				}
				delete(comparator);
			
				if(eh_local==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					
					cont_sim_lido++;
					token.atributo=LOCAL;
					
					printf("<LOCAL>\n");
					estado=0;
					return token;
					
				}
				else {estado = 41;}
				break;
			}
			case 27:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='o'){estado=28;}
				else if(c=='i'){estado=29;}
				else{estado=41;}
				break;
			}

			case 28:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='t'&& isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=NOT;
					printf("<NOT>\n");
					estado=0;
					return token;
				}
				else{estado=41;}
				break;
			}	
			case 29:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='l'&& isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=NIL;
					printf("<NIL>\n");
					estado=0;
					return token;
				}
				else{estado=41;}
				break;
			}
			case 30:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='r'){estado=31;}
				
				else{estado=41;}
				
				break;
			}
			case 31:
			{
				if(isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;	
					token.atributo=OR;
					printf("<OR>\n");
					estado =0;
					break;
					return token;
				}
				else{estado=41;}
				break;
			}
			case 32:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='e'){estado=33;}
				else{estado=41;}
				break;
			}
			case 33:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='p'){estado=34;}
				else if(c=='t'){estado=35;}
				else{estado=41;}
				break;
			}
			case 34:
			{
				bool eh_repeat=true;
				char * comparator;
				comparator = new char[4];
				strcpy(comparator,"eat");
				for(int i=0; i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_repeat=false;
					}
				}
				delete(comparator);
				if(eh_repeat==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=REPEAT;
					printf("<REPEAT>\n");
					estado=0;
					return token;
					break;
				}
				else{ estado = 41;}
				break;
			}
			case 35:
			{
				bool eh_return=true;
				char * comparator;
				comparator = new char[4];
				strcpy(comparator,"urn");
				for(int i=0; i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_return=false;
					}
				}
				delete(comparator);
				if(eh_return==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=RETURN;
					printf("<RETURN>\n");
					estado=0;
					return token;
					break;
				}
				else{ estado = 41;}
				break;
			}
			case 36:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='r'){estado=37;}
				else if(c=='h'){estado=38;}
				else{estado=41;}
				break;
			}
			case 37:
			{
				bool eh_true=true;
				char * comparator;
				comparator= new char[3];
				strcpy(comparator,"ue");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i]){
						eh_true=false;
						break;
					}
				}
				delete(comparator);
				if(eh_true==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado =0;
					printf("<TRUE>\n");
					token.atributo=TRUE;
					return(token);
					
				}
				else{estado=41;}
				break;
			}
			case 38:
			{
				bool eh_then=true;
				char * comparator;
				comparator= new char[3];
				strcpy(comparator,"en");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i]){
						eh_then=false;
						break;
					}
				}
				delete(comparator);
				if(eh_then==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado =0;
					printf("<THEN>\n");
					token.atributo=THEN;
					return(token);
					
				}
				else{estado=41;}
				break;
			}
			case 39:
			{
				bool eh_until = true;
				char * comparator;
				comparator = new char[5];
				strcpy(comparator,"ntil");
				for(int i=0; i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_until=false;
						break;
					}
					
				}
				delete(comparator);
				if(eh_until==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=UNTIL;
					printf("<UNTIL>\n");
					estado=0;
					return token;
				}
				else{estado=41;}
				break;
			}
			case 40:
			{
				bool eh_while = true;
				char * comparator;
				comparator = new char[5];
				strcpy(comparator,"hile");
				for(int i=0; i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_while=false;
						break;
					}
					
				}
				delete(comparator);
				if(eh_while==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					token.atributo=WHILE;
					printf("<WHILE>\n");
					estado=0;
					return token;
					break;
				}
				else{estado=41;}
			}
			case 41:
			{
				
				while( (isalnum(c)>0) || (c=='_'))
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
			
				}
				printf("<ID>\n");
				token.atributo=ID;
				estado =0;
				return token;
				break;
			}
			case 42:
			{
				if(isdigit(c)==0){estado= falhar ();}
				else{estado =43;}
				break;
			}
			case 43:
			{
				while(isdigit(c)!=0)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
				}
				
				printf("<NUM>\n");
				token.atributo=NUM;
				estado =0;
				return token;
				break;
			}
			case 44:
			{
				if(isoperation(c)==0){estado=falhar();}
				else{estado=45;}
			}
			case 45:
			{
				switch(c)
				{
					case '+':{estado=46;break;}
					case '-':{estado=47;break;}
					case '*':{estado=48;break;}
					case '/':{estado=49;break;}
					case '^':{estado=50;break;}
					
				}
				break;
			}
			case 46:
			{
				cont_sim_lido++;
				printf("<ADD>\n");
				token.atributo = ADD;
				estado = 0;
				return token;
				break;
			}
			case 47:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				if(c=='-')
				{
					estado=66;
					
				}	
				else{	
					
					printf("<SUB>\n");
					token.atributo = SUB;
					estado = 0;
					return token;
					
				}
				break;
			}
			case 48:
			{
				cont_sim_lido++;
				printf("<MUL>\n");
				token.atributo = MUL;
				estado = 0;
				return token;
				break;
			}
			case 49:
			{
				cont_sim_lido++;
				printf("<DIV>\n");
				token.atributo = DIV;
				estado = 0;
				return token;
				break;
			}
			case 50:
			{
				cont_sim_lido++;
				printf("<POW>\n");
				token.atributo = POW;
				estado = 0;
				return token;
				break;
			}
			case 51:
			{
				if(isdelimination(c)==0){estado=falhar();}
				else{estado = 52;}
				break;
			}
			case 52:
			{
				switch(c)
				{
					case '(':{estado = 53;break;}
					case ')':{estado = 54;break;}
					case '{':{estado = 55;break;}
					case '}':{estado = 56; break;}
					case '[':{estado = 57; break;}
					case ']':{estado = 58; break;}
					case '.':{estado = 59;break;}
					case ',':{estado = 61;break;}
					case ';':{estado = 62;break;}
					case ':':{estado = 63;break;}
					
				}

				break;
			}
			case 53:
			{
				cont_sim_lido++;
				printf("<(>\n");
				token.atributo = L_PAREN;
				estado = 0;
				return token;
				break;
			}
			case 54:
			{
				cont_sim_lido++;
				printf("<)>\n");
				token.atributo = R_PAREN;
				estado = 0;
				return token;
				break;
			}
			case 55:
			{
				cont_sim_lido++;
				printf("<{>\n");
				token.atributo = L_CURLY;
				estado = 0;
				return token;
				break;
			}
			case 56:
			{
				cont_sim_lido++;
				printf("<}>\n");
				token.atributo = R_CURLY;
				estado = 0;
				return token;
				break;
			}
			case 57:
			{
				cont_sim_lido++;
					
				printf("<[>\n");
				token.atributo = L_BRACK;
				estado = 0;
				return token;
				break;
			}
			case 58:
			{
				cont_sim_lido++;
				printf("<]>\n");
				token.atributo = R_BRACK;
				estado = 0;
				return token;
				break;
			}
			case 59:
			{
				cont_sim_lido++;
				if(code[cont_sim_lido]=='.'){estado=60;}
				else
				{
					printf("<.>\n");
					token.atributo = DOT;
					estado = 0;
					return token;
					
				}
				break;
			}
			case 60:
			{
				cont_sim_lido++;
				printf("<..>\n");
				token.atributo = TWO_DOT;
				estado = 0;
				return token;
				break;	
			}
			case 61:
			{
				cont_sim_lido++;
				printf("<,>\n");
				token.atributo = COMMA;
				estado = 0;
				return token;
				break;
			}
			case 62:
			{
				cont_sim_lido++;
				printf("<;>\n");
				token.atributo = SEMICOLON;
				estado = 0;
				return token;
				break;
			}
			case 63:
			{
				cont_sim_lido++;
				printf("<:>\n");
				token.atributo = COLON;
				estado = 0;
				return token;
				break;
			}
			case 64:
			{
				if(c=='\"'){estado =65;}
				else {estado = falhar();}
				break;
			}
			case 65:
			{
				cont_sim_lido++;
				c=code[cont_sim_lido];
				
				while(c!='\"')
				{
					cont_sim_lido++;
					c=code[cont_sim_lido++];
				}
				token.atributo=STRING;
				estado=0;
				printf("<STRING>\n");
				
				return token;
				break;	
			}
			case 66:
			{
				if(code[cont_sim_lido+1]=='[' && code[cont_sim_lido+2]=='['){estado=67;}
				else{
					while(c!='\n')
					{
						cont_sim_lido++;
						c=code[cont_sim_lido];
					}
					estado=0;
				}
				break;
			}
			case 67:
			{
				while(code[cont_sim_lido]!=']' && code[cont_sim_lido+1]!=']')
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
				}
				cont_sim_lido++;
				cont_sim_lido++;
				cont_sim_lido++;
				estado=0;
				break;
			}
			case 68:{
				
				goto erro;
				break;
				
			}
			case 69:{
				bool eh_break=true;
				char * comparator;
				comparator= new char[5];
				strcpy(comparator,"reak");
				for(int i = 0;i<strlen(comparator);i++)
				{
					cont_sim_lido++;
					c=code[cont_sim_lido];
					if(c!=comparator[i])
					{
						eh_break=false;
						break;
					}
				}
				
				delete(comparator);
				
				if(eh_break==true && isalnum(code[cont_sim_lido+1])==0 && code[cont_sim_lido+1]!='_')
				{
					cont_sim_lido++;
					estado =0;
					printf("<BREAK>\n");
					token.atributo=BREAK;
					return (token);
					break;
				}
				else{estado = 41;}
				break;
			}
			
		
		}
	}

		
	erro:
	token.nome_token = EOF;
	token.atributo = -1;

	return token; 
}

void block(Token& token);
void statement(Token& token);
void exps(Token& token);
void expr(Token& token);
void exprLine(Token& token);
int binOp(Token& token);
void prefixExp(Token& token);
void prefixExpLine(Token& token);
void field(Token& token);
void vars(Token& token);
void var(Token& token);
void function(Token& token);
void functionBody(Token& token);
void Param(Token& token);
void names(Token& token);


void block(Token& token)
{
	
    statement(token);
	if(token.atributo==SEMICOLON)
	{
		token=proximo_token();
		block(token);
	}
	else
	{
		
	}
	
}

void statement(Token& token)
{
	
    switch(token.atributo)
    {
        case ID:
        {
            vars(token);
            if(token.atributo==ASSIGN)
            {
				token=proximo_token();
                exps(token);
            }
            break;
        }
        case DO:
        {
            token=proximo_token();
            block(token);
            if(token.atributo==END)
            {
				token=proximo_token();
            }
            else
            {
                cout<<"erro, bloco não finalizado";
				exit(EXIT_FAILURE);
            }
			break;
        }
		case WHILE:
		{
			token=proximo_token();
			expr(token);
			if(token.atributo==DO)
			{
				token=proximo_token();
				block(token);
				if(token.atributo==END)
				{
					token=proximo_token();
				}
				else
				{
					cout<<"Erro: Bloco não terminado em END";
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout<<"Erro: While sem DO";
				exit(EXIT_FAILURE);
			}
			break;
		}
		case RETURN:
		{
			token=proximo_token();
			exps(token);
			break;
		
		}
		case BREAK:
		{
			token=proximo_token();
			break;
		}
        case IF:
        {
            token=proximo_token();
            expr(token);
            if(token.atributo==THEN)
            {
                token=proximo_token();
                block(token);
                if(token.atributo==ELSEIF)
                {
                     token=proximo_token();
                    expr(token);
                    if(token.atributo==THEN)
                    {
                        token=proximo_token();
                        block(token);
                    }
                }
                else if(token.atributo==ELSE)
                {
                    token=proximo_token();
				    block(token);
                }
            }
            if(token.atributo==END)
            {

            }
            else
            {
                cout<<"erro: bloco não contem END";
				exit(0);
            }
            break;
        }
        case FOR:
        {
            token=proximo_token();
            if(token.atributo==ID)
            {
                token=proximo_token();
                
                if(token.atributo==IN)
                {
                    token=proximo_token();
					exps(token);
                    if(token.atributo==DO)
                    {
                        token=proximo_token();
                        block(token);
                        if(token.atributo==END)
                        {
							token=proximo_token();
                        }
                        else
                        {

                        }
                    }
                }
                else if(token.atributo==ASSIGN)
                {
                    int cont=0;
					token=proximo_token();
					expr(token);
					if(token.atributo==COMMA)
					{
						token=proximo_token();
						expr(token);
						if(token.atributo==COMMA)
						{
							token=proximo_token();
							expr(token);
						}
						else
						{

						}
					}
					
					if(token.atributo==DO)
					{
						token=proximo_token();
						block(token);
						if(token.atributo==END)
						{
							token=proximo_token();
						}
					}
                }
            }
            break;
        }
        case LOCAL:
        {
            token=proximo_token();
            if(token.atributo==FUNCTION)
            {
                goto função;
            }
            else if(token.atributo==ID)
            {
                names(token);
                if(token.atributo==ASSIGN)
                {
                    token=proximo_token();
                    exps(token);

                }
            }
			break;
        }
        case FUNCTION:
        {
            função:
            token=proximo_token();
            function(token);
            break;
        }
        default:
        {
            break;
        }
    }
}

void exps(Token& token)
{
    expr(token);
    if(token.atributo==COMMA)
    {
        token=proximo_token();
        exps(token);
    }
}

void expr(Token& token)
{
    switch (token.atributo)
    {
        case NOT:
        {
            token=proximo_token();
            expr(token);
            exprLine(token);
            break;
        }
        case SUB:
        {
            token=proximo_token();
            expr(token);
            exprLine(token);
            break;
        }
        case NUM:
        {
            token=proximo_token();
            exprLine(token);
            break;
        }
        case STRING:
        {
            token=proximo_token();
            exprLine(token);
            break;
            
        }
        case FUNCTION:
        {
            token=proximo_token();
            function(token);
            break;
        }
        case TRUE:
        {
            token=proximo_token();
            exprLine(token);
            break;
        }
        case FALSE:
        {
            token=proximo_token();
            exprLine(token);
            break;
        }
        case ID:
        {
            goto indoParaPrefixExp;
			break;
        }
		case L_PAREN:
		{
			indoParaPrefixExp:
            prefixExp(token);
            exprLine(token);
			break;

		}
        /*case L_BRACK:
        {
            indoParaPrefixExp:
            prefixExp(token);
            exprLine(token);

        }*/
        case L_CURLY:
        {
            token=proximo_token();
            field(token);
            while(token.atributo==COMMA)
            {
                token=proximo_token();
                field(token);
            }
            if(token.atributo==R_CURLY)
            {
                token=proximo_token();
            }
            break;
        }
        case COMMA:
        {
            break;
        }
        case NIL:
        {
            token=proximo_token();
            exprLine(token);
            break;
        }
        default:
            
            cout<<"erro";
            break;
    }
}

void exprLine(Token& token)
{
    
    if(binOp(token)==1){
        expr(token);
        exprLine(token);
    }
    else{}
}

int binOp(Token& token)
{   
    
    switch(token.atributo)
	{
		case OR:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case AND:
		{
			token=proximo_token();
			return 1;
            break;
		}	
		case LT:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case LE:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case GT:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case GE:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case NE:
		{
			token=proximo_token();
			return 1;
            break;
		}	
		case EQ:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case TWO_DOT:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case ADD:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case SUB:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case MUL:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case DIV:
		{
			token=proximo_token();
			return 1;
            break;
		}
		case POW:
		{
			token=proximo_token();
			return 1;
            break;
		}
		default:
		{
			return 0;
			break;
		}
    }
}

void prefixExp(Token& token)
{
    if(token.atributo==ID)
    {
        token=proximo_token();
        prefixExpLine(token);
    }
    if(token.atributo==L_PAREN)
    {
        token=proximo_token();
        exps(token);
        if(token.atributo==R_PAREN)
        {
            token=proximo_token();
            prefixExp(token);
        }
    }
    else
    {
        
    }
    
}

void prefixExpLine(Token& token)
{
    if(token.atributo==L_BRACK)
    {
        token=proximo_token();
        expr(token);
        if(token.atributo==R_BRACK)
        {
            prefixExpLine(token);
        }
    }
    else
    {

    }
}

void field(Token& token)
{
    if(token.atributo==L_BRACK)
    {
        token=proximo_token();
        expr(token);
        if(token.atributo==R_BRACK)
        {
            goto irParaAtribução;
        }

    }
    else if(token.atributo==ID)
    {
        irParaAtribução:
        token=proximo_token();
        if(token.atributo==ASSIGN)
        {
            token=proximo_token();
            expr(token);
        }
        else
        {
            cout<<"erro:";
        }
    }
    else
    {
        cout<<"Erro";
    }
}

void vars(Token& token)
{
    var(token);
	 if(token.atributo==COMMA)
    {
        token=proximo_token();
        vars(token);
    }
}

void var(Token& token)
{
    prefixExp(token);    
}

void function(Token& token)
{
    functionBody(token);
}

void functionBody(Token& token)
{
    if(token.atributo==ID)
    {
        token=proximo_token();
        if(token.atributo==L_PAREN)
        {
            token=proximo_token();
            if(token.atributo==ID)
            {
                Param(token);    
            }
            if(token.atributo==R_PAREN)
            {
                token=proximo_token();
                block(token);
                if(token.atributo==END)
                {
					token=proximo_token();
                }
                else
                {
                    cout<<"Erro:bloco sem finzalização";
					exit(EXIT_FAILURE);
                }

            }
            else
            {
                cout<<"Erro:Parametro nao fechado";
				exit(EXIT_FAILURE);
            }
            
        }
        else
        {

        }
    }
    else
    {
        cout<<"Erro: Função sem nome";
		exit(EXIT_FAILURE);
    }

}

void Param(Token& token)
{
    names(token);
}

void names(Token& token)
{
    if(token.atributo==ID)
    {
        token=proximo_token();
		if(token.atributo==COMMA)
		{
			token = proximo_token();
			if(token.atributo==ID)
            {
                names(token);
            }
            else
            {
                cout<<"Erro: Variavel não encontrada";
				exit(EXIT_FAILURE);
            }
		}
        
    }
}

int main ()
{
	Token token;
    code =  readFile("programa1.txt");
    
    if(code==NULL)
	{
		return 1;
	}
   else{
   
   token=proximo_token();
    while(code[cont_sim_lido] != '\0')
	{

    	block(token);
    	
    }
	cout <<"Compilado com sucesso";
	
	return 0;
	}
}
