#include<bits/stdc++.h>
#include<regex>
using namespace std;

class Token
{
    string tokenValue;
    string type;
    int row,col;
    public:
    Token(string &t,string &id,int r,int c)
    {
        tokenValue=t;
        type=id;
        row=r;
        col=c;
    }
    string getTokenValue() const
    {
        return tokenValue;
    }
    string getType() const
    {
        return type;
    }
    int getRow() const
    {
        return row;
    }
    int getCol() const
    {
        return col;
    }
};

bool checkStr(string &s)
{
    if(s[0]=='"' && s[s.length()-1]=='"' && s.length()>1)return true;
    return false;
}

string removeSpaces(string &s)
{
    string ans="";
    for(auto c:s)
    {
        if(c!=' ')
        {
            ans+=c;
        }
    }
    return ans;
}

string classifyToken(string &str)
{   
    std::regex keyword(R"(\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|for|goto|if|inline|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\b)");
    regex numeric("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");
    std::regex string_regex(R"("\"[^\"]*\"")");
    regex char_regex("'([^']|\\.)'");
    if(regex_match(str,keyword))
    {
        string id="keyword";
        return id;
    }
    else
    {
        if(regex_match(str,numeric))
        {
            string id="numeric";
            return id;
        }
        else if(regex_match(str,string_regex) || checkStr(str))
        {
            string id="string";
            return id;
        }
        else if(regex_match(str,char_regex))
        {
            string id="character";
            return id;
        }
        else
        {
            string id="identifier";
            return id;
        }
    }
}

vector<Token> extractTokens(string &line,int row)
{
    string str;
    vector<Token> tokens;
    regex operator_regex("(\\+\\+|--|==|!=|<=|>=|&&|\\|\\||<<|>>|->|\\+=|-=|\\*=|/=|%=|&=|\\|=|\\^=|<<=|>>=|::|->\\*|~=|&|\\*|\\+|-|/|%|<|>|=|!|\\||\\^|~|\\?|:|\\.)");
    regex braces_match("(\\{|\\(|\\[|\\}|\\]|\\))");
    regex punctuators(R"([;,])");
    bool hasStrStarted=false;
    string var="";
    for(int i=0;i<line.length();i++)
    {
        char c=line[i];
        if(hasStrStarted)
        {
            var+=c;
        }
        if(i+1<line.length() && line[i]=='/' && line[i+1]=='/')
        {
            return tokens;
        }
        if(c==' ')
        {
            str=removeSpaces(str);
            string id=classifyToken(str);
            if(str!="")tokens.push_back(Token(str,id,row,i-str.length()));
            str="";
        }
        else
        {
            string x="";
            x+=c;
            if(i+1<line.length())
            {
                if(regex_match(x+line[i+1],operator_regex))
                {
                    str=removeSpaces(str);
                    string id=classifyToken(str);
                    tokens.push_back(Token(str,id,row,i-str.length()));
                    str="";
                    string op=x+line[i+1];
                    string id2="operator";
                    tokens.push_back(Token(op,id2,row,i-op.length()));
                    continue;
                }
            }
            if(x=="\"")
            {
                if(!hasStrStarted)
                {
                    hasStrStarted=true;
                    var+=x;
                }
                else
                {
                    hasStrStarted=false;
                    string id="string_literal";
                    tokens.push_back(Token(var,id,row,i-var.length()));
                    // cout<<var<<endl;
                    var="";
                    
                }
                // cout<<hasStrStarted<<endl;
                continue;
            }
            if(!hasStrStarted && regex_match(x,punctuators))
            {
                string id="punctuators";
                str=removeSpaces(str);
                string idStr=classifyToken(str);
                if(str!="")tokens.push_back(Token(str,idStr,row,i-str.length()));
                tokens.push_back(Token(x,id,row,i));
                str="";
            }
            else if(!hasStrStarted && regex_match(x,operator_regex))
            {
                string id="operator";
                str=removeSpaces(str);
                string idStr=classifyToken(str);
                if(str!="")tokens.push_back(Token(str,idStr,row,i-str.length()));
                tokens.push_back(Token(x,id,row,i));
                str="";
            }
            else if(!hasStrStarted && regex_match(x,braces_match))
            {
                string id="brackets";
                str=removeSpaces(str);
                string idStr=classifyToken(str);
                if(str!="")tokens.push_back(Token(str,idStr,row,i-str.length()));
                tokens.push_back(Token(x,id,row,i));
                str="";
            }
            else if(!hasStrStarted)
            {
                str+=c;
            }
        }
    }
    int i=line.length();
    if(str!="")
    {
        string id=classifyToken(str);
        tokens.push_back(Token(str,id,row,i-str.length()));
    }
    return tokens;
}

int main()
{
    string filename;
    cout<<"Enter the filename"<<endl;
    cin>>filename;
    ifstream file(filename);
    if(!file)
    {
        cout<<"Error in opening file!"<<endl;
        exit(1);
    }
    string line;
    vector<vector<Token>> tokens;
    int row=0;
    while(getline(file,line))
    {
        vector<Token> tokensPerLine=extractTokens(line,row);
        tokens.push_back(tokensPerLine);
        row++;
    } 
    int maxLen=0;
    for(auto tokenVector:tokens)
    {
        for(auto token:tokenVector)
        {
            string t=token.getType();
            maxLen=max(maxLen,(int)t.length());
        }
    }
    for(auto tokenVector:tokens)
    {
        for(auto token:tokenVector)
        {
            cout<<token.getType()<<" "<<token.getTokenValue()<<" "<<token.getRow()<<" "<<token.getCol()<<endl;
        }
    }
    return 0;
}