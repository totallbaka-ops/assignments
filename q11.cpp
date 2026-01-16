#include<iostream>
#include<string>

using namespace std;
string rev (string a){
    int n = a.size();
    string b;
    for(int i =n-1;i>=0;i--){
        b.push_back(a[i]);
    }
    return b;
}
//add function
string add_s(string a, string b) {
    if (a.size() < b.size()) swap(a, b);
    a = rev(a);
    b=rev(b);
    
    string res = "";
    int carry = 0;
    for (int i = 0; i < a.size(); i++) {
        int sum = (a[i]-'0') + carry + (i < b.size() ? (b[i]-'0') : 0);
        res.push_back((sum % 10)+'0');     //asciii o f 0 + sum
        carry = sum / 10;
    }
    if (carry) res.push_back(carry + '0');
    res = rev(res);
    return res;
}


//sub function
string sub_s(string a, string b) {
    if (a.size() < b.size()) swap(a, b);
    a = rev(a);
    b=rev(b);
    
    string res = "";
    int borr = 0;
    for (int i = 0; i < a.size(); i++) {
        int diff = (a[i]-'0') - borr - (i < b.size() ? (b[i]-'0') : 0);
        if (diff<0){
            diff+=10;
            borr = 1;
        }else {borr = 0;}
        res.push_back(diff+'0');
    }
    while (res.size() > 1 && res.back() == '0')
        res.pop_back();
    res = rev(res);
    return res;
   
}
//mul 
string mul_s(string a , string b){
    int res[6005]={0};
    int n,m;
    n=a.size();
    m=b.size();
    for(int i =n-1;i>=0;i--){
        for(int j=m-1;j>=0;j--){
            int prod =(a[i]-'0')*(b[j]-'0');
            res[i+j+1]+=prod;
        }
    }
    for(int k= m+n-1;k>=0;k--){
        if (res[k]>=10){
            res[k-1]+=res[k]/10;
            res[k]%=10;
        }
    }

    string s = "";     // remove leading 0s
    int i = 0;
    while (i < n + m - 1 && res[i] == 0) i++;
    for (; i < n + m; i++) s.push_back(res[i] + '0');
    return s.empty() ? "0" : s;
}

//div
int compare(string a, string b) {
    if (a.size() > b.size()) return 1;
    if (a.size() < b.size()) return -1;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}
string removeLeadingZeros(string s) {
    int i = 0;
    while (i < s.size() - 1 && s[i] == '0') i++;
    return s.substr(i);
}

string div_s(string a, string b) {
    a = removeLeadingZeros(a);
    b = removeLeadingZeros(b);

    // Handle edge cases
    if (compare(b, "0") == 0) return "0"; // division by 0 ignored
    if (compare(a, b) < 0) return "0";   // if a < b, quotient is 0

    string quotient = "";
    string curr = "";  // like the current part of dividend

    for (int i = 0; i < a.size(); i++) {
        curr += a[i];                 // bring down next digit
        curr = removeLeadingZeros(curr);

        int count = 0;
        while (compare(curr, b) >= 0) { // subtract divisor as many times as possible
            curr = sub_s(curr, b);
            count++;
        }

        quotient.push_back(count + '0'); // store quotient digit
    }

    quotient = removeLeadingZeros(quotient);
    return quotient;
}



//expo
string expo(string e, long long x){
    string res = "1";
    while(x>0){
        if(x%2==1){
            res = mul_s(res, e);
        }
        e=mul_s(e,e);
        x/=2;
    }
    return res;
}


//gcd
string mod_s(string a, string b){
    if (compare(b, "0") == 0) return "0"; 
    if (compare(a, b) < 0) return a;      

    // quotient = div_s(a, b)
    string q = div_s(a, b);

    // product = b * quotient
    string prod = mul_s(b, q);

    // remainder = a - product
    string res = sub_s(a, prod);
    res = removeLeadingZeros(res);

    return res;
}
string gcds(string a ,string b){
    if (a == "0") return b;
    if (b == "0") return a;
    string s = "1";
    while (b != "0") {
        string r = mod_s(a, b); // remainder
        a = b;
        b = r;
    }
    return a;
}


//factorial
string fact(string x){
    string out = "1";
    string one = "1";
    while(x!="1"){
    out = mul_s(x,out);
    x = sub_s(x,one);
    }
    return out;

}

int precedence(char k){
    if(k=='x' ||k=='/') return 2;
    if(k=='-' ||k=='+') return 1;
    return 0;
}


string postfix(string in){

    char stack[1000];
    int top = -1;
    string output = "";
    for(int i =0;i<in.size();i++){
        char c = in[i];
        if(isdigit(c)){
            while(isdigit(in[i]) && i<in.size()){
                output +=in[i];
                i++;
            }
            output+= ' ';
            i--;
        }else if( c== 'x' || c== '/' || c== '+'|| c=='-'){
            while(top!=-1 && precedence(stack[top])>=precedence(c)){
                output+=stack[top];
                top--;
                output+=' ';
            } 
            stack[++top]= c;
        } 
    }
    while(top!=-1){
        output+=stack[top--];
        output+= ' ';
    }
    cout << output << "\n";
    return output;
}

//solve postfix

string solve(string post){
   // cout << post << "\n";
    string nums;
    string stack[1000];
    int top = -1;
    for(int i =0;i<post.size();i++){
        char c = post[i];
        if(isdigit(c)){
            nums+=c;
        }
        else if(c == ' ' && !nums.empty()){
            stack[++top]=nums;
            nums="";
        }
        else if(c=='+' || c=='/' || c=='x' || c=='-'){
            string b = stack[top--];
            string a = stack[top--];
            string r;
            if(c=='+'){
             r=add_s(a, b);
            }else if(c=='x'){
                r=mul_s(a, b);
            }else if(c=='-'){
                r=sub_s(a, b);
            }else {
                r = div_s(a, b);
            }
            stack[++top]=r;
        }
    }
 return stack[top];
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;

    switch(n){

        case 1 : {
            // parse string + - * /
            //do required operations
            //print results
            string in, post;
            cin >> in;
            post = postfix(in);
            string r;
            r=solve(post);
            cout << r << endl;

            break;
        }
        case 2: {
            string e,r;
            long long x;
            cin >> e >> x;
            // do e^x
            //print result
            r=expo(e,x);
            cout << r << endl;
            break;
        }
        case 3:{
            string a,b, r;
            cin >> a >> b;
            //do gcd of a,b
            r = gcds(a,b);
            cout << r << endl;
            break;
        }
        case 4:{
            string x,r;
            cin >> x;
            //do x!
            r=fact(x);
            cout << r << endl;
            break;
            
        }


    }
    return 0;


}