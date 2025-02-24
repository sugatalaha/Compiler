%{
    #include<stdio.h>
    int op=0;
    float a,b;
    void digit()
    {
        if(op==0)
        {
            a=atof(yytext);
        }
        else
        {
            b=atof(yytext);
        }
    }
    
    void calculate()
    {
        switch(op)
        {
            case 1:printf("Answer is %f\n",a+b);break;
            case 2:printf("Answer is %f\n",a-b);break;
            case 3:printf("Answer is %f\n",a*b);break;
            case 4:printf("Answer is %f\n",a/b);break;
            case 5:
                float p=1.0;
                for(int i=0;i<b;i++)
                {
                    p*=a;
                }
                printf("%f\n",p);
                break;
            case 6:
                int x=a;
                int y=b;
                printf("Answer is %d\n",x%y);
                break;
            default: break;
        }
        op=0;
    }
%}

%%
[0-9]+|[0-9]*"."[0-9]+ {digit();}
"+" {op=1;}
"-" {op=2;}
"*" {op=3;}
"/" {op=4;}
"^" {op=5;}
"%" {op=6;}
"\n" {calculate();}
%%



int main()
{
    printf("Enter an expression to evaluate\n");
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}