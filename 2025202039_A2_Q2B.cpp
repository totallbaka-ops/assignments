#include<iostream>
#include<utility>                          // for pairs only
using namespace std;


        // qp() this is function, qp this is object
        template <typename P>
class priority_queue{
    
    public:

    P *array;
    int cap;                //capacity
    int sz;                 //size;
    bool max;                 // this can be use to do max or min heap (0 for min, 1 for max )

    void dycap(){                                             // resize fun (dynamic capacity)
        int newc = (cap ==0? 1 : 2*cap);                        //new capacity
        P *narray = new P[newc];

        for(int i =0;i<sz;i++){
            narray[i] = array[i]; 
        }

        delete[] array;
        array=narray;
        cap=newc;
    }

    bool deterheap (P p , P c){            //condi helper funtion
        return max? p<c : p>c;

    }
    void swap (P &a, P &b){
        P c = a;
        a = b;
        b =c;
    }
    

    void heapup(int n ){                    // takes a node
         while(n>0){
            int p = (n-1)/2;
            if(deterheap(array[p],array[n])){
                swap(array[p],array[n]);
                n=p;
            }else{
                break;
            }
         }
    }

    void heapdown(int n){
        while(1){
            int l = 2*n+1;
            int r = 2*n+2;
            int x=n;
                                  
            if(l<sz && deterheap(array[x], array[l])){
                x= l;
            }
            if(r<sz && deterheap(array[x], array[r])){
                x= r;
            }
            if(x!=n){
                swap(array[n],array[x]);
                n=x;
            }else {
                break;
            }

        }
    }
    priority_queue(bool m){    //1
        cap = 10;                         // change later
        sz = 0;
        max = m;
        array = new P[cap];                
    }

    int size(){    //2
        return sz;
    }
    bool empty(){            //6        // 1 for empty 0 for not empty
        return sz==0;
    }

    void push(P x){   //3
        if(sz==cap){
            dycap();
        }
        array[sz]=x;
        heapup(sz);
        sz++;
    }

    void pop(){            //5
        if(sz==0){
            cout << "priority queue is empty\n";
            return;
        }
        swap(array[0],array[sz-1]);
        sz--;
        heapdown(0);
    }

    P top(){
       
        return array[0];
    }
    
    ~priority_queue() {
        delete[] array;                        // to prevent leaks 
    }
    void clear(){
        sz=0;
    }


};

template<typename K>
    void bal(priority_queue<K> &p, priority_queue<K> &q ){      // for balancing max and min heaps, 
        if(p.size()>q.size()+1){
            K x = p.top();
            p.pop();
            q.push(x);
        }else if(q.size()>p.size()){
            K x = q.top();
            q.pop();
            p.push(x);
        }
    }
  
    template<typename K>
    int getmedian(priority_queue<K> &p, priority_queue<K> &q){
        if(p.size() == q.size()){
            return (p.top()+q.top())/2;
        }else {
            return (p.top()>q.top())?p.top():q.top();
        }
    }
    template<typename K>
    int getmedian_p(priority_queue<K> &p, priority_queue<K> &q){
        if(p.size() == q.size()){
            return (p.top().first+q.top().first)/2;
        }else {
            return (p.size()>q.size())?p.top().first:q.top().first;
        }
    }






int main(){
    int n,d;
    cin >> n >> d;
    int input[n]={0};
    for(int i =0;i<n;i++){
        int m;
        cin >> m;
        input[i]=m;
    }
    // [0...d-1] -> no calc so [d...n-1]
    // previous [0...i-1]  i>=d    getmed = m1   first i
    // window [i-d.....i-1]     metmid = m2                  input[i]>m1+m2

    bool passed_day[n]={0};
    int count =0;
    priority_queue<int> max_all(1), min_all(0); 
    priority_queue<pair<int,int>> trail_max(1), trail_min(0);           //pair to store index, value so  that random deletion is easy



    for(int i =0;i<n;i++){
    if(i<d){                                           //initialise both P_queue
        if(max_all.empty() || input[i]<= max_all.top()){
            max_all.push(input[i]);
        }else{
            min_all.push(input[i]);
        }
        bal(max_all, min_all);

        pair<int, int > val = {input[i], i};
        if(trail_max.empty() || val.first <= trail_min.top().first){
                trail_max.push(val);
        }else{
                trail_min.push(val);
        }        
        bal(trail_max, trail_min);
    }else{
        int p_day = i-d;
        passed_day[p_day]= 1;

        pair<int, int > val = {input[i], i};
        if(trail_max.empty() || val.first <= trail_min.top().first){
                trail_max.push(val);
        }else{
                trail_min.push(val);
        }        
        bal(trail_max, trail_min);

        while(!trail_max.empty() && passed_day[trail_max.top().second]){    //remove passed days from window
            trail_max.pop();
        }
        while(!trail_min.empty() && passed_day[trail_min.top().second]){    
            trail_min.pop();
        }
        bal(trail_max, trail_min);

        int m1= getmedian_p(trail_min, trail_max);
        int m2=getmedian(max_all, min_all);

        if(input[i]>=m1+m2){
           // cout << i << "\n";
            count++;
        }
        
    }
    
    }

    cout << count <<"\n";




    return 0;
}