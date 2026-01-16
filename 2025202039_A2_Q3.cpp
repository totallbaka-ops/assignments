#include<iostream>
using namespace std;

void fun1(){

    int n;
    cin >> n;
    int f[80];
    for (int i = 0; i < n; i++) cin >> f[i];

    int dp[80][80];
    int opt[80][80];
    int prefix[81];  // prefix sums

    prefix[0] = 0;
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + f[i];

    for (int i = 0; i < n; i++) {
        dp[i][i] = f[i];
        opt[i][i] = i;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] =1e9;

            int sum = prefix[j + 1] - prefix[i];

            // Knuth optimization — reduces k range
            int start = opt[i][j - 1];
            int end = opt[i + 1][j];
            if (start < i) start = i;
            if (end > j) end = j;

            for (int k = start; k <= end; k++) {
                int left = (k > i) ? dp[i][k - 1] : 0;
                int right = (k < j) ? dp[k + 1][j] : 0;
                int cost = left + right + sum;

                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    opt[i][j] = k;
                }
            }
        }
    }

    cout << dp[0][n - 1] << "\n";

}    
    





void fun2(){              // index are wrong check later f[1....n]

    int n;
    cin >> n;
    int f[81];
    for (int i=0;i<n;i++){
     cin >> f[i];         // f index is from 1 change later
    }

    // prefix sum matrix for frequency sums
    int sf[81]={0};
    for (int i=1;i<=n;i++) {
    
        
            sf[i]=sf[i-1]+f[i-1];      
        
    }

    int dp[81][81][81];                              // min cost to search interval [i..j] starting from pos p

    // base case: interval length 1
    for (int i=1;i<=n;i++) {                      // check  index later
        for (int p=0;p<=n;p++) {
            dp[i][i][p]=abs(p-i)*f[i-1];  // move from p to i, multiplied by frequency
        }
    }

    
    for (int book=2;book<=n;book++) {
        for (int i=1;i<=n-book+1;i++) {
            int j=i+book-1;
            for (int p=0;p<=n;p++) {
                int d=1e9;
                for (int k=i;k<=j;k++) {
                    int l=(k>i)?dp[i][k-1][k]:0;
                    int r=(k<j)?dp[k+1][j][k]:0;
                    int z;
                    z=sf[j]-sf[i-1];

                    int sum=l+r+z*abs(p-k);
                    if (sum<d){
                        d=sum;
                    }
                }
                dp[i][j][p]=d;
            }
        }
    }

    cout <<dp[1][n][0] << "\n";
}


void fun3(){

}








int main(){
while(1){
    
    int m;
    cin >>m;
    if(m==0){
        return 0;
    }
        switch(m) {
            case 1:{
                fun1();
                break;
            }
           case 2:{
                fun2();
                break;
            }
            /*case 3:{
                fun3();
                break;
            }*/
            default :{
                cout << "invalid command\n";

                
            }
        }
    }
}