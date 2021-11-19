%{
	#include "proyecto.tab.h"
	#include <stdio.h>
	#include <stdlib.h>
%}

caracter 		[a-zA-Z]
digito 			[0-9]
alphanumerico	{caracter}|{digito}
print			[ -~]

ID 			{alpha}+{alnum}*
ICONST		"0"|[0-9]{digit}*
FCONST		"0"|{digit}*"."{digit}+
CCONST		(\'{print}\')|(\'\\[nftrbv]\')
STRING		\"{print}*\"

%%

[a-zA-Z]+				{return VARIABLE;}
[0-9]+					{yylval=atoi(yytext); return NUMERO;}
"+"					{ return OPADD; }
"-"					{ return OPSUBS; }
"*"					{ return OPMULT; }
"/"					{ return OPDIV; }
"%"					{ return OPMOD; }
"++"					{ return OPOPINCR; }
"--"					{ return OPDECR; }
"=="					{ return OPIGUL; }
"!="					{ return OPNIGUL; }
">"					{ return OPMA; }
"<"					{ return OPME; }
">="					{ return OPMAI; }
"<="					{ return OPMEI; }
"("					{ return IZQP; }
")"					{ return DERP; }
"["					{ return IZQC; }
"]"					{ return DERC; }
"{"					{ return IZQLL; }
"}"					{ return DERLL; }
";"					{ return PC; }
"."					{ return PUNTO; }
","					{ return COMA; }
"="					{ return ASIG; }
"+="					{ return OPADD2; }
"-="					{ return OPSUBS2; }
"/="					{ return OPDIV2; }
"*="					{ return OPMULT2; }
"%="					{ return OPMOD2; }
"AND"					{ return OPAND; }
"OR"					{ return OPOR; }
"int"					{ return INT; }
"float"					{ return FLOAT; }
"char"      				{ return CHAR; }
"if"					{ return IF; }
"else"					{ return ELSE; }
"while"					{ return WHILE; }
"for"					{ return FOR; }
"void"					{ return VOID; }
"return"				{ return RETURN; }
"break"					{ return BREAK; }
{ID} 					{ return ID; }
{ICONST} 				{ return ICONST; }
{FCONST} 				{ return FCONST; }
{CCONST} 				{ return CCONST; }
{STRING} 				{ return STRING; }

.
