#include "parseSymbols.h"
#include <filesystem>
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
using namespace cv;
typedef cv::Mat mat;
cv::Mat convertBGRToGray(cv::Mat img) {
    int height = img.rows;
    int width = img.cols;
    cv::Mat grayscaleImg(height, width, CV_8UC1); // в этой картинке мы сохраним черно-белую версию (оттенки серого)
    // давайте поймем что означает тип картинки CV_32FC1:
    //                                          CV = Computer Vision (библиотека в целом называетсяOpenCV)
    //                                             32F = 32-битное floating число, т.е. вещественное число типа float
    //                                                С1 = 1 channel = один канал - вещественное число описывающее насколько этот пиксель яркий:
    //                                                                                                            (0.0 - черный, 255.0 = белый)

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec3b color = img.at<cv::Vec3b>(j, i);
            int blue = color[0];
            int green = color[1];
            int red = color[2];

            //  реализуйте усреднение яркости чтобы получить серый цвет
            //  - обратите внимание что если складывать unsigned char - сумма может переполниться, поэтому перед сложением их стоит преобразовать в int или float
            //  - загуглите "RGB to grayscale formula" - окажется что правильно это делать не усреднением в равных пропорциях, а с другими коэффициентами
            float grayIntensity = ((float) red)*0.299+((float) green)*0.587+((float) blue)*0.114;

            grayscaleImg.at<uchar>(j, i) = ((int) grayIntensity);
        }
    }

    return grayscaleImg;
}
int N=100;int M=100;
vector <pair<string,mat> > v;
vector <string> have;
int dist(mat img1,mat img2)
{
    //cout<<" dist "<<endl;
    assert(img1.rows==img2.rows);assert(img1.cols==img2.cols);
    int n=img1.rows;int m=img2.cols;
    int ans=0;
    for(int i=0;i<n;++i) for(int j=0;j<m;++j)
        {
        bool ok1=((img1.at<uchar>(i,j))>=127);bool ok2=((img2.at<uchar>(i,j))>=127);
        if(ok1!=ok2) ++ans;
        }
    return ans;
}
string h1;
mat norm(mat img) {
    //cout<<img.rows<<' '<<img.cols<<" img "<<endl;
    imwrite("lesson11/resultsData/debug/"+h1+"1.jpg",img);
    int n=img.rows;int m=img.cols;
    //cout<<n<<" n "<<m<<" m "<<endl;
    //cout<<" norm "<<endl;
    mat res = Mat::zeros(N, M, CV_8UC1);
    //cout<<" ok0 "<<endl;
    //cout<<" ok1 "<<endl;
    //for(int i=0;i<n;++i) {for(int j=0;j<m;++j) cout<<(img.at<uchar>(i,j)>=127); cout<<endl;}
    for (int i = 0; i < M; ++i) for (int j = 0; j < N; ++j)
        {
        //cout<<i<<" i "<<j<<" j "<<(i*n)/N<<' '<<(j*m)/M<<endl;
        uchar o=img.at<uchar>((i*n)/N,(j*m)/M);
         res.at<uchar>(i,j)=o;
        }
    //assert(sum!=0);cout<<sum<<" sum "<<endl;
    //cout<<" norm2 "<<endl;
   // exit(0);
    return res;
}
void generatev()
{
    //cout<<" generatev "<<endl;
    for(string h:have) {
        h1=h;
        string path = "lesson11/data/base/" + h + ".jpg";
        //cout<<path<<" path "<<endl;
        mat img = imread(path);
        img=convertBGRToGray(img);
        assert(img.type()==CV_8UC1);
        //cout<<h<<" h "<<endl;
        imwrite("lesson11/resultsData/debug/"+h+"0.jpg",img);
        mat img2 = norm(img);
        imwrite("lesson11/resultsData/debug/"+h+".jpg",img2);
        v.push_back({h, img2});
    }
}
int cur=(-1);
void prep()
{
    //cout<<" prep "<<endl;
    for(char s='a';s<='z';++s) {string o;o.push_back(s);have.push_back(o);}
    for(char s='1';s<='9';++s) {string o;o.push_back(s);have.push_back(o);}
    generatev();
    //cout<<" generated all"<<endl;
}
string ch(cv::Mat img)
{
    ++cur;
    //cout<<" ch "<<endl;
    string ans;
    if(img.rows<=23 && img.cols<=23) return ans;
    mat img2=norm(img);
    imwrite("lesson11/resultsData/debug/"+to_string(cur)+".jpg",img2);
    string el;int mi=((int) 1e9);
    for(auto h:v)
    {
        int dst=dist(img2,h.second);
        if(dst<mi) {mi=dst;el=h.first;}
    }
    ans=el;
    return ans;
}
