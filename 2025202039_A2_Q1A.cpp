#include<iostream>
#include<ctime>
using namespace std;

template <typename A> 

class deque{

    public: 
    A *array;
    int capacity, size, start;

    void dycap(){                                             // resize fun
        int newc = (capacity ==0? 1 : 2*capacity);
        A *narray = new A[newc];

        for(int i =0;i<size;i++){
            narray[i] = array[(start+i)%capacity]; 
        }

        delete[] array;
        array=narray;
        capacity=newc;
        start=0;
    }


    deque(int n, A k){                  // 3
        capacity =n;
        size=n;
        start =0;
        array = new A[capacity];

        for(int i=0;i<n;i++){
            array[i]=k;
        }
        
    }

    deque(int n) {                      //  2
        capacity =n;
        size=n;
        start=0;
        array = new A[capacity];

        for(int i=0;i<n;i++){
            array[i]= A();
        }
    }

    deque() {                      // 1
        capacity =0;
        size=0;
        start=0;
        array = nullptr;            // dont use A[capacity] as A[0] is not good

    }

    void push_back(A k){        // 4
        if(size==capacity){         // dynamically resize use resize function here
            dycap();
        }

        int back= (start+size)%capacity;
        array[back] = k;
        size++;
    }

    void pop_back() {           // 5
        if(size==0){
            cerr << "deque is empty" << endl;
            return ;
        }
        size--;

    }

    void push_front(A k){        //6

        if(size==capacity){              // dyna resize use resixe fns here
          dycap();
        }
        start = (start-1+capacity)%capacity;
        array[start]=k;
        size++;
    }

    void pop_front(){                  //7         
        if(size==0){
            cerr << "deque is empty" << endl;
            return ;
        }

        start=(start+1)%capacity;
        size--;
    }

    A front() {                // 8
        if(size==0){
            return A();
        }

        return array[start];
    }

    A back() {                 // 9 
        if(size==0){
            return A();
        }

        return array[(start+size-1)%capacity];
    }

    // do 10 later


    bool empty(){                       // 11              
        return size==0;
    }

    int Size() {
        return  size;                     //12
    }

    void resize(int n) {                    //13

        while(n>=capacity){      // capacity can be too small and n can be big so 2x maynot cover it
            dycap();
        }

        if(n>size){
            for(int i =size;i<n;i++){
                array[(start+i)%capacity] = A();
            }
            size = n;
        }else{
            size=n;
        }

    }


    void resize(int n, A d) {               //14

        while(n>=capacity){      // capacity can be too small and n can be big so 2x maynot cover it
            dycap();
        }

        if(n>size){
            for(int i =size;i<n;i++){
                array[(start+i)%capacity] = d;
            }
            size = n;
        }else{
            size=n;
        }

    }

    void reserve(int n){                   //15
        if(n>capacity){
        int newc = n;
        A *narray = new A[newc];

        for(int i =0;i<size;i++){
            narray[i] = array[(start+i)%capacity]; 
        }

        delete[] array;
        array=narray;
        capacity=newc;
        start=0;
        }
    }


    void shrink_to_fit(){            // 16    

        if(size == 0){   
        delete[] array;
        capacity = 0;
        start = 0;
        array = nullptr;
        return;
    }

        int newc = size;
        A *narray = new A[newc];               // if array is empty againA{0} case

        for(int i =0;i<size;i++){
            narray[i] = array[(start+i)%capacity]; 
        }

        delete[] array;
        array=narray;
        capacity=newc;
        start=0;
        }
    

    void clear(){       // 17
        size=0;
        start=0; 
    }

    int Capacity(){                    // 18
        return capacity;
    }

   const A& operator[](int n) const{            //10 .a      this is read only
    static const A def = A();
        if(n>=0){
            if(n>=size){
                return def;
            }
            return array[(start+n)%capacity];
        }else {
            if(-n > size){
                return def;
            }
            return array[(start+size+n)%capacity];
        }
    }
     A& operator[](int n) {            //10.b       this is assignable
        static A def = A();
        if(n>=0){
            if(n>=size){
                return def;
            }
            return array[(start+n)%capacity];
        }else {
            if(-n > size){
                return def;
            }
            return array[(start+size+n)%capacity];
        }
    }
 
    ~deque() {
        delete[] array;     // to prevent leaks 
    }
};

int main(){
    deque<int> qd(10);               //CHANGE DATA TYPE HERE
    while(1){
        cout << "\033[36muse 1-18 for operations and 0 to exit :-->\033[0m\n";  // show in cyan
        
        int n;
        cin >> n;
        if(n==0){
            cout << "\033[31mExiting\033[0m\n";
            break;
        }
        
        switch(n){
            case 1: {
               qd.clear();
               break;
            }
            case 2: {
                int m;
                cout << "provide size :" << "\n";
                cin >> m ;
                qd.clear();                                  
              qd.resize(m);
              break;

            }
            case 3: {
                
                int m,x ;
                cout << "provide size and value :\n";
                cin >> m >> x; 
                qd.clear();
                qd.resize(m,x);
                
                break;
            
            }
            case 4 :{
                cout << "provide value to push back:\n";
                int x;
                cin >> x;
                qd.push_back(x);
                break;
            }
            case 5 : {
            
                qd.pop_back();
                break;
            }
            case 6 : {
                cout << "provide value to push front: \n";
                int x;
                cin >> x;
                qd.push_front(x); break;
            }
            case 7 : {

                qd.pop_front(); break;
            }
            case 8 : {
                cout << "front :"<< qd.front() << "\n"; break;
            }
            case 9 : {
                cout << "back :" << qd.back() << "\n"; break;

            }
            case 10 : {
                int n;
                cin >> n;
                cout << qd[n] << "\n"; break;
                
            }
            case 11 : {
                bool m;
                m = qd.empty();
                string s;
                (m==1)? s= "deque is empty" : s= "not empty";
                cout << s << "\n"; break;
            }
            case 12 : {
                int s ;
                s = qd.Size();

                cout <<"deque size is :"<< s << "\n"; break;
            }
            case 13 : {
                cout << "give no. for resize :" << "\n";
                int n ;
                cin >> n;
                qd.resize(n); break;

            }
            case 14 : {
                cout << "give no. and value :" << "\n";
                int n, d;
                cin >> n >> d;
                qd.resize(n,d); break;
            }
            case 15 : {
                cout << "no of spaces to reserve :" << "\n";
                int n;
                cin >> n;
                qd.reserve(n); break;
            }
            case 16 : {
                qd.shrink_to_fit(); break;
            }
            case 17 : {
                qd.clear(); break;
            }
            case 18 : {
                int s = qd.Capacity();
                cout << s << "\n"; break;
            }
            default : {
                cout << "invalid input...use  1-18 for operations and 0 to exit!\n"; break;
            }

        }

    }
    return 0;
}