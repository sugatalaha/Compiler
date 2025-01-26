#include<bits/stdc++.h>
#include<iomanip>
using namespace std;

class TreeNode
{
    public:
    char value;
    TreeNode *left;
    TreeNode *right;
    TreeNode(char c)
    {
        value=c;
        left=NULL;
        right=NULL;
    }
    TreeNode()
    {
        value='#';
        left=NULL;
        right=NULL;
    }
};

class DFA
{
    public:
    int startState;
    vector<int> states;
    vector<int> finalStates;
    map<int,map<char,int>> transitions;
    vector<char> alphabet;
    DFA(int startState,vector<int>& states,vector<int> &finalStates,map<int,map<char,int>> &transitions,vector<char> &a)
    {
        this->startState=startState;
        this->states=states;
        this->finalStates=finalStates;
        this->transitions=transitions;
        alphabet=a;
    }
};

template<class T>
vector<T> unionSet(vector<T> v1, vector<T> v2) {
    set<T> uniqueSet(v1.begin(), v1.end());
    uniqueSet.insert(v2.begin(), v2.end());
    return vector<T>(uniqueSet.begin(), uniqueSet.end());
}

map<TreeNode *,int> LeavesIndex;
map<TreeNode *,vector<int>> lastposMap;
map<TreeNode *,vector<int>> firstposMap;
map<int,vector<int>> followposMap;

bool isOperator(char c)
{
    return (c=='.' || c==')' || c=='(' || c=='|' || c=='*' || c=='+');
}

void printTransitionTable(DFA &dfa) {
    cout << "Transition Table:\n";
    
    // Print the header row
    cout << setw(10) << "State";
    for (char symbol : dfa.alphabet) {
        cout << setw(10) << symbol;
    }
    cout << "\n";

    // Print each state's transitions
    for (int state : dfa.states) {
        cout << setw(10) << state;
        for (char symbol : dfa.alphabet) {
            if (dfa.transitions[state].count(symbol)) {
                cout << setw(10) << dfa.transitions[state][symbol];
            } else {
                cout << setw(10) << "-"; // No transition for this symbol
            }
        }
        cout << "\n";
    }
}


TreeNode *createSyntaxTree(vector<char> &tokens)
{
    stack<char> operatorStack;
    vector<char> rpn;
    for(auto token:tokens)
    {
        if(!isOperator(token))
        {
            rpn.push_back(token);
        }
        else
        {
            if(token=='(')
            {
                operatorStack.push(token);
            }
            else if(token==')')
            {
                while(!operatorStack.empty() && operatorStack.top()!='(')
                {
                    rpn.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                if(operatorStack.size() && operatorStack.top()=='(')
                {
                    operatorStack.pop();
                }
                else
                {
                    printf("Invalid regex!");
                    exit(1);
                }
            }
            else
            {
                while(!operatorStack.empty())
                {   
                    if(operatorStack.top()=='(')
                    {
                        break;
                    }
                    else
                    {
                        rpn.push_back(operatorStack.top());
                        operatorStack.pop();
                    }
                }
                operatorStack.push(token);
            }
        }
    }
    while(!operatorStack.empty())
    {
        rpn.push_back(operatorStack.top());
        operatorStack.pop();
    }
    cout<<"RPN:"<<endl;
    for(auto it:rpn){
        cout<<it<<" ";
    }
    cout<<endl;
    stack<TreeNode *> operandStack;
    for(auto ele:rpn)
    {
        if(!isOperator(ele))
        {
            TreeNode *leaf=new TreeNode(ele);
            operandStack.push(leaf);
        }
        else
        {
            if(ele=='*' || ele=='+')
            {
                if(operandStack.empty())
                {
                    printf("Invalid regex!");
                    exit(1);
                }
                auto t=operandStack.top();
                operandStack.pop();
                TreeNode *operatorNode=new TreeNode(ele);
                operatorNode->left=t;
                // cout<<operatorNode->value<<" "<<t->value<<endl;
                operandStack.push(operatorNode);
            }
            else
            {
                if(operandStack.empty())
                {
                    printf("Invalid regex!");
                    exit(1);
                }
                auto t=operandStack.top();
                operandStack.pop();
                if(operandStack.empty())
                {
                    printf("Invalid regex!");
                    exit(1);
                }
                auto t2=operandStack.top();
                operandStack.pop();
                TreeNode *operandNode=new TreeNode(ele);
                operandNode->left=t2;
                operandNode->right=t;
                // cout<<operandNode->value<<" "<<t2->value<<" "<<t->value<<endl;
                operandStack.push(operandNode);
            }
        }
    }
    if(operandStack.empty())
    {
        printf("Invalid regex!");
        exit(1);
    } 
    return operandStack.top();
}

// Function: nullable
bool nullable(TreeNode *node) {
    if (!isOperator(node->value)) {
        return node->value == 'e'; // Leaf is nullable if it's epsilon
    }
    if (node->value == '|') {
        return nullable(node->left) || nullable(node->right);
    }
    if (node->value == '.') {
        return nullable(node->left) && nullable(node->right);
    }
    if (node->value == '*') {
        return true; // Kleene star is always nullable
    }
    return false;
}

// Function: firstPos
vector<int> firstPos(TreeNode *node) {
    if (node->value == 'e') {
        return firstposMap[node] = {};
    }
    if (!isOperator(node->value)) {
        return firstposMap[node] = {LeavesIndex[node]};
    }
    if (node->value == '|') {
        return firstposMap[node] = unionSet(firstPos(node->left), firstPos(node->right));
    }
    if (node->value == '.') {
        if (!nullable(node->left)) {
            return firstposMap[node] = firstPos(node->left);
        }
        return firstposMap[node] = unionSet(firstPos(node->left), firstPos(node->right));
    }
    if (node->value == '*') {
        return firstposMap[node] = firstPos(node->left);
    }
}

// Function: lastPos
vector<int> lastPos(TreeNode *node) {
    if (node->value == 'e') {
        return lastposMap[node] = {};
    }
    if (!isOperator(node->value)) {
        return lastposMap[node] = {LeavesIndex[node]};
    }
    if (node->value == '|') {
        return lastposMap[node] = unionSet(lastPos(node->left), lastPos(node->right));
    }
    if (node->value == '.') {
        if (!nullable(node->right)) {
            return lastposMap[node] = lastPos(node->right);
        }
        return lastposMap[node] = unionSet(lastPos(node->left), lastPos(node->right));
    }
    if (node->value == '*') {
        return lastposMap[node] = lastPos(node->left);
    }
    return {};
}

void traverseFirstPos(TreeNode *node)
{
    if(node==NULL)return;
    firstPos(node);
    traverseFirstPos(node->left);
    traverseFirstPos(node->right);
}

void traverseLastPos(TreeNode *node)
{
    if(node==NULL)return;
    lastPos(node);
    traverseLastPos(node->left);
    traverseLastPos(node->right);
}

void printVector(vector<int> &v)
{
    for(auto it:v)
    {
        cout<<it<<" ";
    }
    cout<<endl;
}

// Function: followPos
void followPos(TreeNode *node) {
    if (!node) return;

    if (node->value == '.') {
        // For concat node, update followpos for lastPos(left) with firstPos(right)
        for (auto n : lastposMap[node->left]) {
            followposMap[n] = unionSet(followposMap[n], firstposMap[node->right]);
        }
    }
    if (node->value == '*') {
        // For Kleene star, update followpos for lastPos(node) with firstPos(node)
        for (auto n : lastPos(node)) {
            followposMap[n] = unionSet(followposMap[n], firstposMap[node]);
        }
    }

    // Recur for left and right subtrees
    followPos(node->left);
    followPos(node->right);
}

// Function: Debugging helper to print a tree (preorder)
void printTree(TreeNode *node) {
    if (!node) return;
    cout << node->value << " ";
    printTree(node->left);
    printTree(node->right);
}


void enumerateLeaves(TreeNode *root,int &index,map<TreeNode *,int> &leavesIndex)
{
    if(root->left==NULL && root->right==NULL)
    {
        leavesIndex[root]=(index++);
    }
    else
    {
        if(root->value=='.' || root->value=='|')
        {
            enumerateLeaves(root->left,index,leavesIndex);
            enumerateLeaves(root->right,index,leavesIndex);
        }
        else
        {
            (root->left)?enumerateLeaves(root->left,index,leavesIndex):enumerateLeaves(root->right,index,leavesIndex);
        }
    }
}

DFA regextoDFA(string &s,vector<char> &alphabet)
{
    string t=s;
    t+="#";
    vector<char> tokens;
    for(int i=0;i<t.length();i++)
    {
        
        if(isOperator(t[i]))
        {
            if(t[i]=='(')
            {
                if(tokens.empty())
                {
                    tokens.push_back('(');
                }
                else
                {
                    tokens.push_back('.');
                    tokens.push_back('(');
                }
            }
            else
            {
                tokens.push_back(t[i]);
            }
        }
        else
        {
            if(!tokens.empty() && !(tokens.back()=='.' || tokens.back()=='|' || tokens.back()=='('))tokens.push_back('.');
            tokens.push_back(t[i]);
        }
    }
    TreeNode *root=createSyntaxTree(tokens);
    cout<<"Syntax tree:"<<endl;
    printTree(root);
    cout<<endl;
    int index=1;
    enumerateLeaves(root,index,LeavesIndex);
    traverseFirstPos(root);
    traverseLastPos(root);
    followPos(root);
    followposMap[LeavesIndex.size()]={};
    cout<<"Leaves to Index mapping:"<<endl;
    for(auto it:LeavesIndex)
    {
        cout<<it.first->value<<" "<<it.second<<endl;
    }
    cout<<"Firstpos:"<<endl;
    for(auto it:firstposMap)
    {
        cout<<it.first->value<<"->";
        for(auto t:it.second)
        {
            cout<<t<<" ";
        }
        cout<<endl;
    }
    cout<<"Lastpos:"<<endl;
    for(auto it:firstposMap)
    {
        cout<<it.first->value<<"->";
        for(auto t:it.second)
        {
            cout<<t<<" ";
        }
        cout<<endl;
    }
    cout<<"Followpos:"<<endl;
    for(auto it:followposMap)
    {
        cout<<it.first<<":";
        for(auto t:it.second)
        {
            cout<<t<<" ";
        }
        cout<<endl;
    }
    map<vector<int>,map<char,vector<int>>> temp;
    vector<int> s0=firstPos(root);
    queue<vector<int>> q;
    set<vector<int>> marked;
    q.push(s0);
    while(!q.empty())
    {
        vector<int> T=q.front();
        q.pop();
        marked.insert(T);
        for(char c:alphabet)
        {
            vector<int> U;
            for(auto p:T)
            {
                for(auto it:LeavesIndex)
                {
                    if(it.second==p && it.first->value==c)
                    {
                        U=unionSet(U,followposMap[p]);
                    }
                }
            }
            if(!U.empty() && marked.find(U)==marked.end())
            {
                q.push(U);
            }
            temp[T][c]=U;
        }
    }
    vector<vector<int>> states;
    for(auto state:marked)
    {
        states.push_back(state);
    }
    map<vector<int>,int> vectorInt;
    for(int i=0;i<states.size();i++)
    {
        vectorInt[states[i]]=i;
    }
    vector<int> numStates;
    for(int i=0;i<states.size();i++)
    {
        numStates.push_back(i);
    }
    vector<int> finalStates;
    for(auto state:states)
    {
        if(find(state.begin(),state.end(),LeavesIndex.size())!=state.end())
        {
            finalStates.push_back(vectorInt[state]);
        }
    }
    map<int,map<char,int>> transitions;
    for(auto it:temp)
    {
        for(auto t:it.second){
            transitions[vectorInt[it.first]][t.first]=vectorInt[t.second];
        }
    }
    return DFA(vectorInt[s0],numStates,finalStates,transitions,alphabet);
}

int main()
{
    int n;
    cout<<"Enter the number of alphabets"<<endl;
    cin>>n;
    vector<char> alphabet(n);
    cout<<"Enter the characters in alphabet"<<endl;
    for(int i=0;i<n;i++)
    {
        cin>>alphabet[i];
    }
    string s;
    cout<<"Enter your regex"<<endl;
    cin>>s;
    DFA d=regextoDFA(s,alphabet);
    cout<<"Final states"<<":"<<endl;
    vector<int> finalStates=d.finalStates;
    printVector(finalStates);
    printTransitionTable(d);
}