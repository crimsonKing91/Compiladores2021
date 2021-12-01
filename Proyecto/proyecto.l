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
")"					{ return DERP; }
"["					{ return IZQC; }
"]"					{ return DERC; }
"{"					{ return IZQLL; }
"}"					{ return DERLL; }
"="					{ return ASIG; }


.
