%{
    #include<stdio.h>
%}

%%
^\n 
1(1|0|.)*101 {printf("Valid string %s\n",yytext);}
.* {printf("Invalid string %s\n",yytext);}


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
