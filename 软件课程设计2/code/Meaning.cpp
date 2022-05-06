/*
��������ķ��Լ�token��������Ԫʽ

*/
#include<bits/stdc++.h>
#include"cpp.cpp"
using namespace std;
/*--------ȫ�ֱ����Լ��ṹ��-----------*/
int k=0;
struct Quaternary{
    string op;
    string arg1;
    string arg2;
    string result;
};
struct Token{
    int line;
    string word;
    int type;
};
struct SignTable{
    //���ű�
    string name="";//��������
    string type;
    string val;
};

//��ѯ���ű�����name��Ӧ�ļ�¼
int lookup(string name,vector<SignTable>signtable){
    for(int i=0;i<signtable.size();i++){
        if(name==signtable[i].name){
            return i;
        }
    }
    
    return -1;

}
//����һ���µ���ʱ����
string newtemp(){
    string p="t";
    p+=to_string(k);
    k++;
    return p;
}
//������Ԫʽ
Quaternary emit(string result,string arg1,string arg2,string op){
    
    Quaternary tquad;
    tquad.arg1=arg1;
    tquad.arg2=arg2;
    tquad.result=result;
    tquad.op=op;
    return tquad;
}
//��ʽ����
void ERROR(int type,string name){
    switch(type){
        case 0:
            //�ظ�����
            cout<<"\033[31m Failed\t:"<<name<<"\tis redefined\033[0m"<<endl;
            break;
        case 1:
            //δ����ֵ
            cout<<"\033[31m Failed\t:"<<name<<"\tis not assigned\033[0m"<<endl;
            break;
        case 2:
            //δ������ֵ�Ͳ�������
            cout<<"\033[31m Failed\t:"<<name<<"\tdoes not has a value\033[0m"<<endl;
            break;
        case 3:
            //δ������
            cout<<"\033[31m Failed\t:"<<name<<"\tis not defined\033[0m"<<endl;
            break;

    }
}
//�˷�
SignTable MUL(string a,string b){
    SignTable temp;
    temp.name= newtemp();
    string ans;
    //TODO:�����Ҫ�ӵ��ǲ�ͬ����֮��ĳ˷�
    cout<<"*"<<a<<" "<<b<<endl;
    ans=to_string(stoi(a)*stoi(b));
    temp.val=ans;
    return temp;
}
//�ӷ�
SignTable ADD(string a,string b){
    SignTable temp;
    temp.name= newtemp();
    string ans;
    //TODO:�����Ҫ�ӵ��ǲ�ͬ����֮��ļӷ�
    cout<<"+"<<a<<" "<<b<<endl;
    ans=to_string(stoi(a)+stoi(b));
    temp.val=ans;
    return temp;
    
}
Quaternary meaning_process(int id,stack<Token>&tokens,vector<SignTable>&signtable,bool &has_err){
    Quaternary temp;
    temp.op="";
    if(has_err){
        return temp;
    }
    switch(id){
        case 7:{
            //[FORMULA] --> ['int'][sign]�������
            tokens.pop();
            string name=tokens.top().word;
            tokens.pop();
            string type=tokens.top().word;
            cout<<"7:"<<name<<" "<<type<<endl;
            tokens.pop();
            int p=lookup(name,signtable);
            if(p!=-1){
                //˵��֮ǰ��ע�����
                has_err=true;
                ERROR(0,name);
            }else{
                //֮ǰû��ע�ᣬ����ע��
                SignTable ns;
                ns.name=name;
                ns.type=type;
                signtable.push_back(ns);
            }
            break;
        }
        case 12:{
            //��ֵ����е�һЩ��[N] --> [sign]
            
            break;
        }
        case 10:{
            //[E] --> [N]['*'][E]
            tokens.pop();
            Token arg1=tokens.top();
            tokens.pop();
            tokens.pop();
            Token arg2=tokens.top();
            tokens.pop();
            bool flag=true;
            if(arg1.type!=3){
                int p=lookup(arg1.word,signtable);
                if(p==-1){
                    //���û��ע��Ͳ�������
                    has_err=true;
                    ERROR(2,arg1.word);
                    flag=false;
                }else if(signtable[p].val==""){
                    //���ע���˵���û�и�ֵ�Ͳ�������
                    has_err=true;
                    ERROR(1,arg1.word);
                    flag=false;
                }

            }
            if(arg2.type!=3){
                int p=lookup(arg2.word,signtable);
                if(p==-1){
                    //���û��ע��Ͳ�������
                    has_err=true;
                    ERROR(2,arg2.word);
                    flag=false;
                }else if(signtable[p].val==""){
                    //���ע���˵���û�и�ֵ�Ͳ�������
                    has_err=true;
                    ERROR(1,arg2.word);
                    flag=false;
                }

            }
            if(flag){
                string a,b;
                if(arg1.type==3){
                    a=arg1.word;
                }else{
                    int p=lookup(arg1.word,signtable);
                    a=signtable[p].val;
                }
                if(arg2.type==3){
                    b=arg2.word;
                }else{
                    int p=lookup(arg2.word,signtable);
                    b=signtable[p].val;
                }
                SignTable res=MUL(a,b);
                signtable.push_back(res);
                Token nt;
                nt.word=res.name;
                nt.type=4;
                tokens.push(nt);
                temp=emit(res.name,arg1.word,arg2.word,"*");
            }

            break;
        }
        case 8:{
            //[F] --> [E]['+'][F]
            //TODO������е����⣬������Լ��ʱ�򣬲�����ֱ��ȥ����Լ����
            
            if(tokens.top().word==";"){
                tokens.pop();
            }
            Token arg1=tokens.top();
            tokens.pop();
            tokens.pop();
            Token arg2=tokens.top();
            tokens.pop();
            bool flag=true;
            if(arg1.type!=3){
                int p=lookup(arg1.word,signtable);
                if(p==-1){
                    //���û��ע��Ͳ�������
                    has_err=true;
                    ERROR(2,arg1.word);
                    flag=false;
                }else if(signtable[p].val==""){
                    //���ע���˵���û�и�ֵ�Ͳ�������
                    ERROR(1,arg1.word);
                    has_err=true;
                    flag=false;
                }

            }
            if(arg2.type!=3){
                int p=lookup(arg2.word,signtable);
                if(p==-1){
                    //���û��ע��Ͳ�������
                    has_err=true;
                    ERROR(2,arg2.word);
                    flag=false;
                }else if(signtable[p].val==""){
                    //���ע���˵���û�и�ֵ�Ͳ�������
                    has_err=true;
                    ERROR(1,arg2.word);
                    flag=false;
                }

            }
            if(flag){
                string a,b;
                if(arg1.type==3){
                    a=arg1.word;
                }else{
                    int p=lookup(arg1.word,signtable);
                    a=signtable[p].val;
                }
                if(arg2.type==3){
                    b=arg2.word;
                }else{
                    int p=lookup(arg2.word,signtable);
                    b=signtable[p].val;
                }
                
                SignTable res=ADD(a,b);
                signtable.push_back(res);
                Token nt;
                nt.word=res.name;
                nt.type=4;
                tokens.push(nt);
                temp=emit(res.name,arg1.word,arg2.word,"+");
            }


            break;
        }
        case 6:{
            //[FORMULA] --> [sign]['='][F]��ֵ���
            if(tokens.top().word==";"){
                tokens.pop();
            }
            
            string val=tokens.top().word;
            
            tokens.pop();
            string a=tokens.top().word;
            tokens.pop();
            string name=tokens.top().word;
            cout<<name<<"|"<<val<<"|"<<a<<endl;
            tokens.pop();
            int p=lookup(name,signtable);
            if(p==-1){
                has_err=true;
                ERROR(3,name);
            }else{
                //ע��ֵ���߸���ֵ
                signtable[p].val=val;
                temp=emit(val,name,"_","=");
            }
            break;
        }
        case 11:{
           //[E] --> [N]

            break;
        }
        case 9:{
            //[F] --> [E]
            
            break;
        }
        default:{
            //���﷨��grammarǰ���м����ʹ򼸸�7 12 10 8 6
            break;
        }
    }
    return temp;
}
