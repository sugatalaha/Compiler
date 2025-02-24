%{
    #include<stdio.h>
    int char_a=0,char_b=0;
%}

%%
a {char_a++;}
b {char_b++;}
\n {printf("%d\n",char_a==char_b);char_a=0;char_b=0;}
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
