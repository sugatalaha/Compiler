%{
    #include<stdio.h>
%}
%%
^a+b+ {printf("Valid string :%s\n",yytext);}
^.* {printf("Invalid string :%s\n",yytext);}
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