//Project Name: Optimal Text Problem
//Created By: Krishna Prasath D

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cmath>
using namespace std;

//The paragraph is stored in pgh, which contains the words inserted in order.It is done to freely manipulate the paragraph, except for print function
//undostack and redostack are represented using 2-D vectors to store the elements which are added or deleted and the operation header add/delete
//undo vector is used to push elements to undostack


class Paragraph {
	// design your data structure here
    private:
        int wordlimit;
        int charlimit;
        vector<string> pgh;
        vector<vector<string>> undostack;
        vector<vector<string>> redostack;
    
    public:
        
        /* The exception Classes */
        class UndoNotPossibleException{};
        class WordLimitExceededException{};
        class NothingToDeleteException{};
        class RedoNotPossibleException{};
        class EmptyParagraphException{};
    
        Paragraph(int n, int c, int w, vector<string> sentence)/* Paragraph Initialisation */
        {
            vector<string> undo;
            undo.push_back("add");
            wordlimit=w;
            charlimit=c;
            for(int i=0;i<n;i++)
            {
                pgh.push_back(sentence[i]);
                undo.push_back(sentence[i]); /* Undostack is updated with operation header add */
            }
            undostack.push_back(undo);
        }
    
        void add_word(string word)
        {
            if(pgh.size()>=wordlimit)
            { throw WordLimitExceededException(); }
            if(!redostack.empty())
            {
                redostack.clear();   
            }
            vector<string> undo;
            undo.push_back("add");
            pgh.push_back(word);
            undo.push_back(word);
            undostack.push_back(undo);
        }
    
        void add_sentence(vector<string> sentence)
        {
            if(pgh.size()+sentence.size()>wordlimit)
            {
                throw WordLimitExceededException();
            }
            if(!redostack.empty())
            {
                redostack.clear();
            }
            vector<string> undo;
            undo.push_back("add");
            for(int i=0;i<sentence.size();i++)
            {
                pgh.push_back(sentence[i]);
                undo.push_back(sentence[i]);
            }
            undostack.push_back(undo);
        }
    
        void undo()
        {
            if(undostack.empty())
            {
                throw UndoNotPossibleException();
            }
            int fin=undostack.size()-1;
            redostack.push_back(undostack[fin]);
            if(undostack[fin][0]=="add")
            {
                for(int i=0;i<undostack[fin].size()-1;i++)
                {
                    pgh.pop_back();
                }
            }
            else 
            {
                for(int i=undostack[fin].size()-1;i>=1;i--)
                {
                    pgh.push_back(undostack[fin][i]);
                }
            }
            undostack.pop_back();
        }
    
        void redo()
        {
            if(redostack.empty())
            {
                throw RedoNotPossibleException();
            }
            int fin=redostack.size()-1;
            undostack.push_back(redostack[fin]);
            if(redostack[fin][0]=="add")
            {
                for(int i=1;i<redostack[fin].size();i++)
                {
                    pgh.push_back(redostack[fin][i]);
                }
            }
            else
            {
                for(int i=0;i<redostack[fin].size()-1;i++)
                {
                    pgh.pop_back();
                }
            }
            redostack.pop_back();
        }
    
        void print()
        {
            int n=pgh.size();
            if(pgh.empty())
            {
                throw EmptyParagraphException();
            }
            
            /* Lengthword to store length of words */
            int lengthword[n];
            for(int i=0;i<n;i++)
            {
                lengthword[i]=pgh[i].size();
            }
            
            /* Calculating badness | infinity is replaced with -1 */
            int badness[n][n];
            for(int i=0;i<=n-1;i++)
            {
                for(int j=0;j<=n-1;j++)
                {
                    if(i>j)
                    {
                        badness[i][j]=-1;
                    }
                    else
                    {
                        int lengthsum=0;
                        for(int k=i;k<=j;k++)
                        {
                            lengthsum+=lengthword[k]+1;
                        }
                        lengthsum-=1;
                        if(lengthsum>charlimit)
                        {
                            badness[i][j]=-1;
                        }
                        else
                        {   
                             badness[i][j]=pow(charlimit-lengthsum,3);
                        }
                    }
                }
            }
            
            /* Justify values calculated */
            int justify[n+1];
            justify[n]=0;
            for(int i=n-1;i>=0;i--)
            {
                if(badness[i][n-1]!=-1)
                {
                    justify[i]=justify[n]+badness[i][n-1];
                }
                else
                {
                    justify[i]=-1;
                }
                int jtemp;
                for(int j=n-1;j>i;j--)
                {
                    if(badness[i][j-1]!=-1)
                    {
                        jtemp=justify[j]+badness[i][j-1];
                    }
                    else
                    {
                        jtemp=-1;
                    }
                    if(justify[i]==-1)
                    {
                        justify[i]=jtemp;
                    }
                    else 
                    {
                        if(jtemp<justify[i]&&jtemp!=-1)
                        {
                            justify[i]=jtemp;
                        }
                    }
                }
            }
            
            /* Parsing from justify zero to find out the set of words in each line */
            int fir=0;
            while(fir!=n)
            {
                int jtemp;
                for(int k=fir+1;k<=n;k++)
                {
                    jtemp=justify[k]+badness[fir][k-1];
                    if(jtemp==justify[fir])
                    {
                        if(k==n)
                        {
                            for(int j=fir;j<k-1;j++)
                            {
                                cout<<pgh[j];
                                cout<<" ";
                            }
                            cout<<pgh[n-1]<<endl;
                        }
                        else
                        {
                            int len=0;
                            for(int j=fir;j<k;j++)
                            {
                                len+=lengthword[j];
                            }
                            int spaces=charlimit-len;
                            int nw=k-fir-1;
                            for(int j=fir;j<k-1;j++)
                            {
                                cout<<pgh[j];
                                int sp=spaces/nw;
                                if(spaces%nw!=0)
                                {
                                    sp+=1;
                                    spaces-=1;
                                }
                                for(int i=0;i<sp;i++)
                                {
                                    cout<<" ";
                                }
                            }
                            cout<<pgh[k-1]<<endl;
                        }
                        fir=k;
                        break;
                    }
                }
            }
        }
    
        void _delete(int x)
        {
            if(pgh.size()<x)
            {
                throw NothingToDeleteException();
            }
            if(!redostack.empty())
            {
                redostack.clear();
            }
            vector<string> undo;
            undo.push_back("delete");
            for(int i=x;i>0;i--)
            {
                undo.push_back(pgh[pgh.size()-1]);
                pgh.pop_back();   
            }
            undostack.push_back(undo);
        }
};

int main() {
  	// your driver code here
    string temp,temp1;
    vector<string> firstsentence;
    int maxword,charlimit,firstline;
    cin>>temp>>firstline>>charlimit>>maxword;
    
    for(int i=0;i<firstline;i++)
    {
        cin>>temp1;
        firstsentence.push_back(temp1);
    }
    
    Paragraph p(firstline,charlimit,maxword,firstsentence);
    
    string command;
    cin>>command;
    while(command!="end")
    {
        if(command=="add_word")
        {
            try
            {
                string word;
                cin>>word;
                p.add_word(word);
            }
            catch(Paragraph::WordLimitExceededException)
            {
                cout<<"WordLimitExceededException"<<endl;
            }
        }
        else if(command=="add_sentence")
        {
            try
            {
                string word;
                int nw;
                cin>>nw;
                vector<string> sentence;
                for(int i=0;i<nw;i++)
                {
                    cin>>word;
                    sentence.push_back(word);
                }
                p.add_sentence(sentence);
            }
            catch(Paragraph::WordLimitExceededException)
            {
                cout<<"WordLimitExceededException"<<endl;
            }
        }
        else if(command=="undo")
        {
            try
            {
                p.undo();
            }
            catch(Paragraph::UndoNotPossibleException)
            {
                cout<<"UndoNotPossibleExceptionRaised"<<endl;
            }
        }
        else if(command=="redo")
        {
            try
            {
                p.redo();
            }
            catch(Paragraph::RedoNotPossibleException)
            {
                cout<<"RedoNotPossibleExceptionRaised"<<endl;
            }
        }
        else if(command=="print")
        {
            try
            {
               p.print();
            }
            catch(Paragraph::EmptyParagraphException)
            {
                cout<<"EmptyParagraphException"<<endl;
            }
        }
        else if(command=="delete")
        {
            try
            {
                int x;
                cin>>x;
                p._delete(x);
            }
            catch(Paragraph::NothingToDeleteException)
            {
                cout<<"NothingToDeleteException"<<endl;
            }
        }
        else
        {
            cout<<"InvalidCommand"<<endl;
        }
        cin>>command;
    }
	return 0;
}
