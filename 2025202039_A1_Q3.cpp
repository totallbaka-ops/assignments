#include<iostream>

using namespace std;
int sil[10000][2];                                 /*golbal array*/
int bsize=0;

int getidx(int k, int start, int end){
    
    if(start>end){
        return start;
    }
    int mid = (start+end)/2;           /*binary search to find correct postion to insert k*/

    if(sil[mid][0]==k){
        return mid;
    }else if(sil[mid][0]>k){ 
        return getidx(k, start, mid-1);
    }else {
        return getidx(k,mid+1,end);
    }


}

void banner(int x, int h){
    if(bsize==0){
        sil[0][0]=x;
        sil[0][1]=h;
        bsize++;
        return;
    }
    int idx = getidx(x, 0, bsize-1);
        
    if(idx < bsize && sil[idx][0] == x){  /*if exists update height */
        sil[idx][1] = h;
        return;
    }
    for(int i =bsize;i>idx;i--){
        sil[i][0]= sil[i-1][0];
        sil[i][1]=sil[i-1][1];

    }
    bsize++;
    sil[idx][0]=x;
    sil[idx][1]=h;
    
}

void skyline(int a, int b, int y){
    if(bsize ==0){
        banner(a,y);
        banner(b,0);
        return;
    }

    int ida=getidx(a, 0,bsize-1);
    int basea=(ida>0)? sil[ida-1][1] : 0;
    banner(a,basea);
    
    int idb=getidx(b, 0,bsize-1);
    int baseb=(idb>0)? sil[idb-1][1] : 0;
    banner(b,baseb);


    int x1=getidx(a,0,bsize-1);
    int x2=getidx(b,0,bsize-1);    /*find if a ,b were added before as banner*/
    

    for(int i =x1;i<x2;i++){
        
            sil[i][1]=(sil[i][1]>=y)?sil[i][1]:y;         /*update 2nd point after updating height of x2-1 as banner                                              fn changes x2*/
        

    }
        /*if update caused any dupilcate heights in sil skip those points*/
        int i =1;
        for(int j=1;j<bsize;j++){
            if(sil[j][1]!=sil[i-1][1]){
                sil[i][0]=sil[j][0];
                sil[i][1]=sil[j][1];
                i++;
            }
        }
        bsize = i;
    
}
 void printsil(){
    for(int i =0;i<bsize;i++){
        cout << sil[i][0] << " " << sil[i][1] << "\n";
    }
 }



int main(){
    int n;
    cin >> n;

   
    /* 0 ≤ li < ri ≤ 10^9*//*0 ≤ hi ≤ 10^9*/ /*store only height change points*/                                                     
    for(int i =0; i<n;i++){         
        int inp;
        cin >> inp;

        if(inp==0){
            int x1, x2, h;
            cin >> x1 >> x2 >> h;
            skyline(x1, x2, h);      /*add points to banner*/
        } else{
            printsil();              /*prints banner*/

        }
    }

    return 0;


}