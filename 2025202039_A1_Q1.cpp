#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


 Mat energy_matrix(Mat inputimg ){
    Mat img = inputimg;
    int r = img.rows;
    int c = img.cols;

    Mat energy(r,c,CV_32F,Scalar(0));



    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){

            int le =(j+c-1)%c;
            int rig =(j+1)%c;
            int t =(i+r-1)%r;
            int b =(i+1)%r;
            
            Vec3b left = img.at<Vec3b>(i,le);
            Vec3b right = img.at<Vec3b>(i,rig);
            Vec3b top = img.at<Vec3b>(t,j);
            Vec3b bottom = img.at<Vec3b>(b,j);

            float ri = (float)right[2] - (float)left[2];
            float gi = (float)right[1] - (float)left[1];
            float bi = (float)right[0] - (float)left[0];
            
            float rj = (float)bottom[2] - (float)top[2];
            float gj = (float)bottom[1] - (float)top[1];
            float bj = (float)bottom[0] - (float)top[0];
            
            float eng=sqrt(ri*ri + gi*gi + bi*bi + rj*rj + gj*gj + bj*bj);

            energy.at<float>(i,j) = eng;

         
        }
 

    }
    return energy;

}

float energy_pixel( Mat& img, int i, int j){
            int r = img.rows;
            int c = img.cols;

            int le =(j+c-1)%c;
            int rig =(j+1)%c;
            int t =(i+r-1)%r;
            int b =(i+1)%r;
            
            Vec3b left = img.at<Vec3b>(i,le);
            Vec3b right = img.at<Vec3b>(i,rig);
            Vec3b top = img.at<Vec3b>(t,j);
            Vec3b bottom = img.at<Vec3b>(b,j);

            float ri = (float)right[2] - (float)left[2];
            float gi = (float)right[1] - (float)left[1];
            float bi = (float)right[0] - (float)left[0];
            
            float rj = (float)bottom[2] - (float)top[2];
            float gj = (float)bottom[1] - (float)top[1];
            float bj = (float)bottom[0] - (float)top[0];

    float energy = sqrt(ri*ri + gi*gi + bi*bi + rj*rj + gj*gj + bj*bj);
    return energy;
}

Mat verticalcut(Mat& emat, Mat& inputmat){
int r = inputmat.rows;
int c = inputmat.cols;
Mat dparray(r,c,CV_32F);
Mat seam(r,c,CV_32S);
for(int i = 0;i<c;i++){
    dparray.at<float>(0,i)= emat.at<float>(0,i);
}



for(int i=1;i<r;i++){
    for(int j=0;j<c;j++){

        int k;
        int left =(j+c-1)%c;
        int right = (j+1)%c;

        k=left;
        float minVal = dparray.at<float>(i-1 ,left);
        if (dparray.at<float>(i-1,j) < minVal){
            minVal = dparray.at<float>(i-1,j);
            k=j;
        }
        if (dparray.at<float>(i-1,right) < minVal) {
            minVal = dparray.at<float>(i-1,right);
            k =right;
        }

        dparray.at<float>(i,j)= emat.at<float>(i,j)+minVal;
        seam.at<int>(i,j)=k;

    }
    
}
float seamend = dparray.at<float>(r-1,0);
int ed =0;
for(int i =1;i<c;i++){
     if(dparray.at<float>(r-1,i)<seamend){
        seamend=dparray.at<float>(r-1,i);
        ed=i;
     }
}
int path[r];
path[r-1]=ed;
for(int k=r-2;k>=0;k--){
    path[k]=seam.at<int>(k+1,path[k+1]);

}
Mat output(r,c - 1, CV_8UC3);
Mat Nenmat(r,c-1,CV_32F,Scalar(0));
for(int i=0;i<r;i++){
    for(int j=0;j<path[i];j++){
        output.at<Vec3b>(i, j) = inputmat.at<Vec3b>(i, j);
        
    }

    for(int j=path[i]+1;j<c;j++){
       output.at<Vec3b>(i, j-1) = inputmat.at<Vec3b>(i, j); 
    }
}

for(int i=0;i<r;i++){
    for(int j=0;j<path[i];j++){
       
       if(j==path[i]-1){
        Nenmat.at<float>(i,j) = energy_pixel(output, i , j );
       } else{
        Nenmat.at<float>(i,j) = emat.at<float>(i,j);
       }
        
    }

    for(int j=path[i]+1;j<c;j++){
        
       if(j==path[i]+1){
        Nenmat.at<float>(i,j-1) = energy_pixel(output, i , j );
       } else{
        Nenmat.at<float>(i,j-1) = emat.at<float>(i,j);
       }

    }
}

/*write energy replace for here*/
emat = Nenmat;

return output;

}

Mat horizontalcut(Mat& emat, Mat& inputmat){
int r = inputmat.rows;
int c = inputmat.cols;
Mat dparray(r,c,CV_32F);
Mat seam(r,c,CV_32S);
for(int i = 0;i<r;i++){
    dparray.at<float>(i,0)= emat.at<float>(i,0);
}



for(int i=0;i<r;i++){
    for(int j=1;j<c;j++){

        int k;
        int top =(i+r-1)%r;
        int bottom = (i+1)%r;

        k=top;
        float minVal = dparray.at<float>(top,j-1);
        if (dparray.at<float>(i,j-1) < minVal){
            minVal = dparray.at<float>(i,j-1);
            k=i;
        }
        if (dparray.at<float>(bottom,j-1) < minVal) {
            minVal = dparray.at<float>(bottom,j-1);
            k =bottom;
        }

        dparray.at<float>(i,j)= emat.at<float>(i,j)+minVal;
        seam.at<int>(i,j)=k;

    }
    
}
float seamend = dparray.at<float>(0,c-1);
int ed =0;
for(int i =1;i<r;i++){
     if(dparray.at<float>(i,c-1)<seamend){
        seamend=dparray.at<float>(i,c-1);
        ed=i;
     }
}
int path[c];
path[c-1]=ed;
for(int k=c-2;k>=0;k--){
    path[k]=seam.at<int>(path[k+1],k+1);

}
Mat output(r-1, c , CV_8UC3);
Mat Nenmat(r,c-1,CV_32F,Scalar(0)); 

for(int j=0;j<c;j++){
    for(int i=0;i<path[j];i++){
        output.at<Vec3b>(i, j) = inputmat.at<Vec3b>(i, j);

    }

    for(int i=path[j]+1;i<r;i++){
       output.at<Vec3b>(i-1, j) = inputmat.at<Vec3b>(i, j); 

    }
}
for(int j=0;j<c;j++){
    for(int i=0;i<path[j];i++){
       
       if(i==path[j]-1){
        Nenmat.at<float>(i,j) = energy_pixel(output, i , j );
       } else{
        Nenmat.at<float>(i,j) = emat.at<float>(i,j);
       }
        
    }

    for(int i=path[j]+1;i<r;i++){
        
       if(i==path[j]+1){
        Nenmat.at<float>(i-1,j) = energy_pixel(output, i , j );
       } else{
        Nenmat.at<float>(i-1,j) = emat.at<float>(i,j);
       }

    }
}
emat = Nenmat;

return output;

}




int main(int a, char* arg[]){
    if(a!=4){
        cout << "incorrect input format use " << arg[0] << " img path" << " height" << " width"<< "\n";
        return 1;
    }
    int h=0;
    int w=0;
    const char*height = arg[2];
    const char*width = arg[3];

    int i=0,j=0;
    while(height[i]!='\0'){
        h=h*10 +(height[i]-'0');
        i++;
    }
        while(width[j]!='\0'){
        w=w*10 +(width[j]-'0');
        j++;
    }

    Mat inimg = imread(arg[1], IMREAD_COLOR);
    if(inimg.empty()){
        cout << "input file not found"<< "\n";
        return 1;
    }
    int r = inimg.rows;
    int c = inimg.cols;

    if(h>r||w>c){
        cout << "invalid height, width" << "\n";
    }

    /*1. make energy matrix that takes arg[0] as input
    
    2.  apply seamcarving algo on energy matrix and remove low energy pixels, resize image till required size
    
    3.  output final image

    */
    int newh=r-h;
    int neww=c-w;
    Mat outimg =inimg;
    Mat eng = energy_matrix(outimg);
    while(neww>0){
        
        outimg = verticalcut(eng,outimg);
        neww--;
    }
    while(newh>0){
        
        outimg = horizontalcut(eng,outimg);
        newh--;
    }        
    
    
    imwrite("final.jpeg", outimg);
    cout << "done" << "\n";
    return 0;

}