#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <thread>
#include <iostream>
#include <filesystem>
#include <libutils/timer.h>
#include <libutils/rasserts.h>
#include <libutils/fast_random.h>
#include "helpers.h" // ЗДЕСЬ (и в helpers.cpp) ДЛЯ ВАС РЕАЛИЗОВАНЫ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ И СТРУКТУРЫ, ПОСТРОЙТЕ АЛГОРИТМ НА ИХ БАЗЕ
#include "disjoint_set.h"
#include "constants.h"
using namespace std;
using namespace cv;
vector<int> par;
int get(int x) {if(par[x]==(-1)) return x; int ans=get(par[x]);par[x]=ans;return ans;}
void merg(int x,int y) {x=get(x);y=get(y);if(x==y) return; par[x]=y;}
Mat orig;Mat img;Mat img2;Mat img3;
int bld[4][8][8];
int cnt[8][8];double sum1[8][8];int ans2[8][8];
const int szt=2;
void paint(pair<int,int> c,Vec3b color) {for(int i=max(0,c.first-szt);i<=min(img.rows,c.first+szt);++i) for(int j=max(0,c.second-szt);j<=min(img.cols,c.second+szt);++j) {img.at<Vec3b>(i,j)=color;}}
void show(string s) {imwrite(s,img);}
void show2(string s) {imwrite(s,img2);}
pair<int,int> operator-(pair <int,int> a,pair<int,int> b) {a.first-=b.first;a.second-=b.second;return a;}
pair<int,int> operator+(pair <int,int> a,pair<int,int> b) {a.first+=b.first;a.second+=b.second;return a;}
void go(int id,string s)
{
    Mat img=imread(s).clone();int n=img.rows;int m=img.cols;vector<vector<double> > a(n,vector<double> (m,0));for(int i=0;i<n;++i) for(int j=0;j<m;++j) {Vec3b color=img.at<Vec3b>(i,j);a[i][j]=((int) color[0])*0.114+((int) color[1])*0.587+((int) color[2])*0.299;}
    vector<vector<double> > b(n,vector<double> (m,0));vector<vector<double> > c(n,vector<double> (m,0));for(int i=1;i<n-1;++i) for(int j=1;j<m-1;++j) {b[i][j]=a[i+1][j+1]+2*a[i+1][j]+a[i+1][j-1]-a[i-1][j+1]-2*a[i-1][j]-a[i-1][j-1];}for(int i=1;i<n-1;++i) for(int j=1;j<m-1;++j) {c[i][j]=a[i+1][j+1]+2*a[i][j+1]+a[i-1][j+1]-a[i+1][j-1]-2*a[i][j-1]-a[i-1][j-1];}
    vector<vector<double> > d(n,vector<double>(m,0));for(int i=0;i<n;++i) for(int j=0;j<m;++j) d[i][j]=sqrt(c[i][j]*c[i][j]+b[i][j]*b[i][j]);
    vector<vector<bool> > bigd(n,vector<bool>(m,0));for(int i=0;i<n;++i) for(int j=0;j<m;++j) bigd[i][j]=(d[i][j]>=300);
    pair<int,int> el1={-1,-1},el2={-1,-1},el3={-1,-1},el4={-1,-1};
    for(int i=2;i<n-2;++i) for(int j=0;j<m-2;++j) {if(bigd[i][j])
    {
        if(el1.first==(-1) || i+j<el1.first+el1.second) {el1={i,j};}
        if(el2.first==(-1) || j-i<el2.second-el2.first) {el2={i,j};}
        if(el3.first==(-1) || j+i>el3.first+el3.second) {el3={i,j};}
        if(el4.first==(-1) || j-i>el4.second-el4.first) {el4={i,j};}
    }}
    pair<int,int> v2=(el2-el1);pair<int,int> v4=(el4-el1);
    cout<<el1.first<<' '<<el1.second<<endl;
    cout<<v2.first<<' '<<v2.second<<endl;
    cout<<v4.first<<' '<<v4.second<<endl;cout<<endl;
    for(int i=0;i<8;++i) for(int j=0;j<8;++j) {cnt[i][j]=0;sum1[i][j]=0;}
    cout<<" ok1 "<<endl;
    for(double h2=0;h2<1;h2+=0.01) for(double h3=0;h3<1;h3+=0.01)
        {cnt[int(8*h2)][int(8*h3)]++;sum1[int(8*h2)][int(8*h3)]+=a[int(el1.first+h2*v2.first+h3*v4.first)][int(el1.second+h2*v2.second+h3*v4.second)];}
    cout<<" ok2 "<<endl;
    for(int i=0;i<8;++i) for(int j=0;j<8;++j) {double x=sum1[i][j]/cnt[i][j];if(x>=127) {ans2[i][j]=1;} else {ans2[i][j]=0;}}
    for(int i=0;i<8;++i) {for(int j=0;j<8;++j) {cout<<ans2[i][j]<<' ';} cout<<endl;}
    for(int i=0;i<8;++i) for(int j=0;j<8;++j) bld[id][i][j]=ans2[i][j];
}
int main() {try
{
    go(0,"solevoy/data/01.jpg");go(1,"solevoy/data/02.jpg");go(2,"solevoy/data/03.jpg");go(3,"solevoy/data/04.jpg");
orig=imread("solevoy/data/05.jpg").clone();img=orig.clone();img2=img.clone();img3=img2.clone();
int n=img.rows;int m=img.cols;
vector<vector<double> > a(n,vector<double> (m,0));
for(int i=0;i<n;++i) for(int j=0;j<m;++j) {Vec3b color=img.at<Vec3b>(i,j);a[i][j]=((int) color[0])*0.114+((int) color[1])*0.587+((int) color[2])*0.299;}
vector<vector<double> > b(n,vector<double> (m,0));vector<vector<double> > c(n,vector<double> (m,0));
for(int i=1;i<n-1;++i) for(int j=1;j<m-1;++j) {b[i][j]=a[i+1][j+1]+2*a[i+1][j]+a[i+1][j-1]-a[i-1][j+1]-2*a[i-1][j]-a[i-1][j-1];}
for(int i=1;i<n-1;++i) for(int j=1;j<m-1;++j) {c[i][j]=a[i+1][j+1]+2*a[i][j+1]+a[i-1][j+1]-a[i+1][j-1]-2*a[i][j-1]-a[i-1][j-1];}
vector<vector<double> > d(n,vector<double>(m,0));//vector<vector<double> > di(n,vector<double>(m,0));
for(int i=0;i<n;++i) for(int j=0;j<m;++j) d[i][j]=sqrt(c[i][j]*c[i][j]+b[i][j]*b[i][j]);
//for(int i=0;i<n;++i) for(int j=0;j<m;++j) di[i][j]=atan(c[i][j]/b[i][j]);
double ma=0;for(int i=0;i<n;++i) for(int j=0;j<m;++j) ma=std::max(ma,d[i][j]);
for(int i=0;i<n;++i) for(int j=0;j<m;++j) {img2.at<Vec3b>(i,j)={uchar((d[i][j]*255)/ma),uchar((d[i][j]*255)/ma),uchar((d[i][j]*255)/ma)};}
show2("solevoy/resultsData/tgerf.png");
vector<vector<bool> > bigd(n,vector<bool>(m,0));
for(int i=0;i<n;++i) for(int j=0;j<m;++j) bigd[i][j]=(d[i][j]>=300);
par.resize(n*m);for(int& h:par) h=(-1);
for(int i=1;i<n-1;++i) for(int j=1;j<m-1;++j) {if(bigd[i][j] && bigd[i][j+1]) {merg(i*m+j,i*m+j+1);} if(bigd[i][j] && bigd[i+1][j]) {merg(i*m+j,(i+1)*m+j);}}
vector<vector<int> > h(n*m);for(int i=0;i<n*m;++i) {h[get(i)].push_back(i);}
int cur=0;
for(int i=0;i<n*m;++i) {vector<int> ids=h[i];if(ids.size()<=40) continue; ++cur;vector<pair<int,int> > v;for(int u:ids) {v.push_back({u/m,u%m});}
int el11=(-1),el21=(-1),el31=(-1),el41=(-1);int sz=v.size();
for(int j=0;j<sz;++j) {if(el11==(-1) || v[j].first+v[j].second<v[el11].first+v[el11].second) el11=j;}
for(int j=0;j<sz;++j) {if(el21==(-1) || v[j].second-v[j].first<v[el21].second-v[el21].first) el21=j;}
for(int j=0;j<sz;++j) {if(el31==(-1) || v[j].first+v[j].second>v[el31].first+v[el31].second) el31=j;}
for(int j=0;j<sz;++j) {if(el41==(-1) || v[j].second-v[j].first>v[el41].second-v[el41].first) el41=j;}
pair<int,int> el1=v[el11];pair<int,int> el2=v[el21];pair<int,int> el3=v[el31];pair<int,int> el4=v[el41];
pair<int,int> v2=(el2-el1);pair<int,int> v4=(el4-el1);
for(int i=0;i<8;++i) for(int j=0;j<8;++j) {cnt[i][j]=0;sum1[i][j]=0;}
cout<<" ok1 "<<endl;
for(double h2=0;h2<1;h2+=0.01) for(double h3=0;h3<1;h3+=0.01)
    {cnt[int(8*h2)][int(8*h3)]++;sum1[int(8*h2)][int(8*h3)]+=a[int(el1.first+h2*v2.first+h3*v4.first)][int(el1.second+h2*v2.second+h3*v4.second)];}
cout<<" ok2 "<<endl;
for(int i=0;i<8;++i) for(int j=0;j<8;++j) {double x=sum1[i][j]/cnt[i][j];if(x>=127) {ans2[i][j]=1;} else {ans2[i][j]=0;}}
bool ok=false;
for(int id=0;id<4;++id)
{
    int dst=0;
    for(int i=0;i<8;++i) for(int j=0;j<8;++j) {dst+=(ans2[i][j]^bld[id][i][j]);}
    if(dst<=10) {ok=true;}
}
if(ok) {Vec3b color={uchar(rand()%256),uchar(rand()%256),uchar(rand()%256)};for(auto h:{el1,el2,el3,el4}) paint(h,color);}
}
show("solevoy/resultsData/hreghf.png");
cout<<cur;
}
catch (const std::exception &e) {std::cout << "Exception! " << e.what() << std::endl;return 1;}}
