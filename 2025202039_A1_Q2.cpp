#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct node{
    node* charr[26];
    bool flag= false;

    node(){
        flag = false;
        for(int i = 0;i<26;i++){
            charr[i]=nullptr;
        }
    }

    bool containchar(char ch){
        return(charr[ch-'a']!=nullptr);
    }
    void put(char ch, node* node){
        charr[ch-'a']=node;
    }

    node* get(char ch){
        return charr[ch-'a'];
    }

    void setflag(){
        flag=true;
    }


};

    int levenstein(string input, string word){
        int in = input.size();
        int w = word.size();
        int dp[in+1][w+1];
        for(int i=0; i<=in;i++){
            dp[i][0]=i;
        }
        for(int j=0;j<=w; j++){
            dp[0][j]=j;
        }

        for(int i =1;i<=in;i++){
            for(int j =1;j<=w;j++){
                if(input[i-1]==word[j-1]){
                    dp[i][j]=dp[i-1][j-1];
                }else{

                    dp[i][j] = 1 + min( dp[i-1][j-1],  min(dp[i-1][j] , dp[i][j-1] ) );
                }
            }
        }

        return dp[in][w];

    }

class trie{
    public: node* root;
     
    trie(){
    root =  new node();
    }


    void insert(string input){
        node* pos = root;

        for(int i = 0; i<input.size();i++){
            if(!pos->containchar(input[i])){
                pos->put(input[i], new node);
            }
                pos = pos->get(input[i]);
            

        }
        pos->setflag();
    }

    bool spellcheck(string input){
        node* pos = root;
        for(int i=0;i<input.size();i++){
            if(!pos->containchar(input[i])){
                return false;
            }
            pos = pos->get(input[i]);

        }

        return pos->flag;
    }

    void autocomplete(string input){
        node* pos = root;
        for(int i = 0; i<input.size();i++){
            if(!pos->containchar(input[i])){
                cout << "0" << "\n";
                return ;
            }
            pos = pos->get(input[i]);
        }
        vector<string> word;
        getwords(pos,input,word);

       cout << word.size() << "\n";

        for(int i=0;i<word.size();i++){
            cout << word[i] << "\n";
        }
        
    

    
    }  

    void getwords(node* pos, string word,  vector<string>& allwords){
        if(pos->flag){
            allwords.push_back(word); 
            
        }
        for(int i =0;i<26;i++){
            if(pos->charr[i]!=nullptr){
                word.push_back('a'+i);
                
                getwords(pos->charr[i], word, allwords);
                word.pop_back();
            }

        }
    }

    void autocorrect(string input){
            /*use vector as 1<q<1000 */
        vector<string> allwords;
        string word;
        getwords(root, word, allwords); /*gets all of dictionary*/

        vector<string> correctword;
        int count=0;
        
        for(string s : allwords){
            if(levenstein(input,s)<=3){
                count++;
                correctword.push_back(s);
            }
        }
        cout  << count << "\n";

        for(string s : correctword){
            cout << s<< "\n";
        }
        
        return;

    }
};






struct query{
    int flag;
    string word;

};

int min(int a,int b){
    return (a<b?a:b);
}


int main(){
    int n,q;
    cin >> n >> q;
    trie t;
    char in[100];
    for(int i = 0;i<n;i++){
        cin >> in;
        t.insert(in);

    }

    vector<query> inq(q);        
    for(int i= 0;i<q;i++){                                  
        cin >> inq[i].flag >>  inq[i].word;
        
    }
        for(int j=0;j<q;j++){

        switch(inq[j].flag){

            case 1:{
                /*spell check*/
                bool yes = t.spellcheck(inq[j].word);
                cout << yes << "\n";
                break;
            }
            case 2:{

                /*auto complete*/
                t.autocomplete(inq[j].word);


                break;
            }
            case 3:{

                    /*auto correct*/
                    /*longest sub sting*/
                    t.autocorrect(inq[j].word);
                    

                break;
            }
          
        }
    }
    

    return 0;

}

