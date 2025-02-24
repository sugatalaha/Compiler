%{
    #include<stdio.h>
%}

%%

^[A-Z][a-z0-9]+#\n {printf("Valid string:%s\n",yytext);}
^.*\n {printf("Invalid string: %s\n",yytext);}
%%
int main()
{
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}