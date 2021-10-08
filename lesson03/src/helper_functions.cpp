#pragma GCC optimize("O3","Ofast","unroll-loops")
#pragma GCC target("avx2")
#include "helper_functions.h"

#include <libutils/rasserts.h>
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;
cv::Mat getanime(cv::Mat object, cv::Mat largeBackground,int n)
{
    rassert(largeBackground.rows>object.rows,"ti tupoi schto li?");
    rassert(largeBackground.cols>object.cols,"ti tupoi schto li?");
    for(int cyc=0;cyc<n;++cyc)
    {
        int ox=largeBackground.rows-object.rows;
        int oy=largeBackground.cols-object.cols;
        int x=rand()%(ox+1);
        int y=rand()%(oy+1);
        for(int i=0;i<object.rows;++i)
        {
            for(int j=0;j<object.cols;++j)
            {
                cv::Vec3b color = object.at<cv::Vec3b>(i, j);
                cv::Vec3b &color2 = largeBackground.at<cv::Vec3b>(x+i, y+j);
                unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
                unsigned char green = color[1];
                unsigned char red = color[2];
                if((blue+green+red)>=100)
                {
                    color2[0]=blue;color2[1]=green;color2[2]=red;
                }
            }
        }
    }
    return largeBackground;
}
cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((abs(blue+green+red)>255*3-150))
            {
                blue=255;green=0;red=0;
            }
            color={blue,green,red};
        }
    }
    return image;
    //  реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    cv::Vec3b color = image.at<cv::Vec3b>(13, 5); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    //unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    //unsigned char green = color[1];
    //unsigned char red = color[2];

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
    //red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
    //image.at<cv::Vec3b>(13, 5) = cv::Vec3b(blue, green, red);

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // реализуйте функцию которая каждый цвет картинки инвертирует:
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            blue=255-blue;green=255-green;red=255-red;
            color={blue,green,red};
        }
    }
    return image;
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    for(int i=0;i<object.rows;++i)
    {
        for(int j=0;j<object.cols;++j)
        {
            cv::Vec3b &color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b color2 = background.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((blue+green+red)<100)
            {
                blue=color2[0];green=color2[1];red=color2[2];
            }
            color={blue,green,red};
        }
    }
    return object;
    // т.е. что-то вроде накладного фона получится

    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    rassert(largeBackground.rows>object.rows,"ti tupoi schto li?");
    rassert(largeBackground.cols>object.cols,"ti tupoi schto li?");
    int x=(largeBackground.rows-object.rows)/2;
    int y=(largeBackground.cols-object.cols)/2;
    for(int i=0;i<object.rows;++i)
    {
        for(int j=0;j<object.cols;++j)
        {
            cv::Vec3b color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b &color2 = largeBackground.at<cv::Vec3b>(x+i, y+j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((blue+green+red)>=100)
            {
                color2[0]=blue;color2[1]=green;color2[2]=red;
            }
        }
    }
    return largeBackground;

    // реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
}
cv::Mat blackrandom(cv::Mat image1) {
    cv::Mat image=image1.clone();
    std::cout<<time(NULL)+getpid()+rand()<<std::endl;
    srand(time(NULL)+getpid()+rand());
    mt19937 rnd(time(NULL)+getpid()+rand());
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((abs(blue+green+red)<150))
            {
                blue=rnd()%256;green=rnd()%256;red=rnd()%256;
            }
            color={blue,green,red};
        }
    }
    return image;
}
Mat tuda(Mat image,int c,int d)
{
    int a=image.rows;
    int b=image.cols;
    Mat image1(c, d, CV_8UC3, Scalar(0, 0, 0));
    for(int i=0;i<c;++i)
    {
        for(int j=0;j<d;++j)
        {
            Vec3b &color=image1.at<cv::Vec3b>(i, j);
            Vec3b color2=image.at<cv::Vec3b>((i*a)/c, (j*b)/d);
            color=color2;
        }
    }
    return image1;
}
vector <vector <int> > go(vector <vector <int> > v,int r,double c,int coef)
{
    vector <vector <int> > ans=v;
    int n=v.size();int m=v[0].size();
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<m;++j)
        {
            int u1=0;
            int u2=0;
            for(int dx=(-r);dx<=r;++dx)
            {
                for(int dy=(-r);dy<=r;++dy)
                {
                    int val=(dx*dx+dy*dy);
                    if(val>(r*r)) continue;
                    int o1=(i+dx*coef);int o2=(j+dy*coef);
                    if(o1<n && o2<m && o1>=0 && o2>=0)
                    {
                        u1+=v[o1][o2];
                        ++u2;
                    }
                }
            }
            ans[i][j]=(u1>=(u2*c));
        }
    }
    return ans;
}
vector <vector <int> > go2(vector <vector <int> > v,int r,double c,int coef)
{
    vector <vector <int> > ans=v;
    int n=v.size();int m=v[0].size();
    for(int i=0;i<n;i+=r)
    {
        for(int j=0;j<m;j+=r)
        {
            int val=0;int val1=0;
            for(int k=0;k<r;++k)
            {
                for(int l=0;l<r;++l) {
                    if ((i + k) < n && (j + l) < m)
                    {
                        val+=v[i][j];
                        ++val1;
                    }
                }
            }
            for(int k=0;k<r;++k)
            {
                for(int l=0;l<r;++l) {
                    if ((i + k) < n && (j + l) < m)
                    {
                        ans[i+k][j+l]=(val>=(val1*c));
                    }
                }
            }
        }
    }
    return ans;
}
vector <vector <int> > go3(vector <vector <int> > v,int r,double c,int coef)
{
    int n=v.size();int m=v[0].size();
    vector <vector <double> > ans(n);
    for(int i=0;i<n;++i)
    {
        ans[i].resize(m);
        for(int j=0;j<m;++j)
        {
            ans[i][j]=v[i][j];
        }
    }
    for(int i=1;i<n;++i)
    {
        for(int j=1;j<m;++j)
        {
            ans[i][j]=(2*ans[i][j]+ans[i][j-1]+ans[i-1][j])/4;
        }
    }
    for(int i=0;i<n-1;++i)
    {
        for(int j=0;j<m-1;++j)
        {
            ans[i][j]=(2*ans[i][j]+ans[i+1][j]+ans[i][j+1])/4;
        }
    }
    for(int i=1;i<n;++i)
    {
        for(int j=0;j<m-1;++j)
        {
            ans[i][j]=(2*ans[i][j]+ans[i-1][j]+ans[i][j+1])/4;
        }
    }
    for(int i=0;i<n-1;++i)
    {
        for(int j=1;j<m;++j)
        {
            ans[i][j]=(2*ans[i][j]+ans[i+1][j]+ans[i][j-1])/4;
        }
    }
    vector <vector <int> > res(n);
    for(int i=0;i<n;++i) {res[i].resize(m);for(int j=0;j<m;++j) res[i][j]=(ans[i][j]>=c);}
    return res;
}
vector <int> parent;
vector <bool> active;
vector <int> sz;
int get(int x)
{
    if(parent[x]==(-1)) return x;
    int ans=get(parent[x]);
    parent[x]=ans;
    return ans;
}
void merg(int x,int y)
{
    x=get(x);y=get(y);
    if(x==y) return;
    parent[x]=y;if(active[x]) active[y]=true;sz[y]+=sz[x];
}
vector <vector <int> > godsu(vector <vector <int> > v,int minsz) {
    int n = v.size();
    int m = v[0].size();
    parent.resize(n * m);
    active.resize(n * m);
    sz.resize(n*m);
    for(auto &h:sz) h=1;
    for (auto &h:parent) h = (-1);
    for (int i = 0; i < (n * m); ++i) {
        active[i] = (i / m == 0 || i / m == (n - 1) || i % m == 0 || i % m == (m - 1));
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if ((i + 1) < n && j < m && v[i][j] == v[i + 1][j]) {
                merg(i * m + j, (i + 1) * m + j);
            }
            if (i < n && (j + 1) < m && v[i][j] == v[i][j + 1]) {
                merg(i * m + j, i * m + (j + 1));
            }
        }
    }
    vector<vector<int> > ans;
    ans.resize(n);
    for (auto &h:ans) h.resize(m);
    for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j)
        {
        if(v[i][j]==0)
        {
            ans[i][j]=(sz[get(i*m+j)]<minsz);
        }
        else
        {
            ans[i][j]=active[get(i*m+j)];
        }
        }
    return ans;
}