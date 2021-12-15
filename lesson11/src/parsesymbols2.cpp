#include "parsesymbols2.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;
#define SAMPLES_PER_LETTER 20
int randFont3() {
    int fonts[] = {
            cv::FONT_HERSHEY_SIMPLEX,
            cv::FONT_HERSHEY_PLAIN,
            cv::FONT_HERSHEY_DUPLEX,
            cv::FONT_HERSHEY_COMPLEX,
            cv::FONT_HERSHEY_TRIPLEX,
            cv::FONT_HERSHEY_COMPLEX_SMALL,
            cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
            cv::FONT_HERSHEY_SCRIPT_COMPLEX,
    };
    // Выбираем случайный шрифт из тех что есть в OpenCV
    int nfonts = (sizeof(fonts) / sizeof(int));
    int font = rand() % nfonts;

    // С вероятностью 20% делаем шрифт наклонным (italic)
    bool is_italic = ((rand() % 5) == 0);
    if  (is_italic) {
        font = font | cv::FONT_ITALIC;
    }

    return font;
}

double randFontScale3() {
    double min_scale = 2.5;
    double max_scale = 3.0;
    double scale = min_scale + (max_scale - min_scale) * ((rand() % 100) / 100.0);
    return scale;
}

int randThickness3() {
    int min_thickness = 2;
    int max_thickness = 5;
    int thickness = min_thickness + rand() % (max_thickness - min_thickness + 1);
    return thickness;
}

cv::Scalar randColor23() {
    return cv::Scalar(rand() % 128, rand() % 128, rand() % 128); // можно было бы брать по модулю 255, но так цвета будут темнее и контрастнее
}

cv::Mat generateImage3(std::string text, int width, int height) {
    cv::Scalar white(255, 255, 255);
    cv::Scalar backgroundColor = white;
    // Создаем картинку на которую мы нанесем символ (пока что это просто белый фон)
    cv::Mat img(height, width, CV_8UC3, backgroundColor);

    // Выберем случайные параметры отрисовки текста - шрифт, размер, толщину, цвет
    int font = randFont3();
    double fontScale = randFontScale3();
    int thickness = randThickness3();
    cv::Scalar color = randColor23();

    // Узнаем размер текста в пикселях (если его нарисовать с указанными параметрами)
    int baseline = 0;
    cv::Size textPixelSize = cv::getTextSize(text, font, fontScale, thickness, &baseline);

    // Рисуем этот текст идеально в середине картинки
    // (для этого и нужно было узнать размер текста в пикселях - чтобы сделать отступ от середины картинки)
    // (ведь при рисовании мы указываем координаты левого нижнего угла текста)
    int xLeft = (width / 2) - (textPixelSize.width / 2);
    int yBottom = (height / 2) + (textPixelSize.height / 2);
    cv::Point coordsOfLeftBorromCorner(xLeft, yBottom);
    cv::putText(img, text, coordsOfLeftBorromCorner, font, fontScale, color, thickness);

    return img;
}
int dist1(cv::Mat img1,cv::Mat img2)
{
    int n=img1.rows;int m=img1.cols;
    assert(img2.rows==n);assert(img2.cols==m);
    int mid=127;
    int sum=0;
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<m;++j)
        {
            //cout<<i<<" i "<<j<<" j "<<endl;
            int z1=img1.at<uchar>(i,j);int z2=img2.at<uchar>(i,j);
            sum+=((z1>=mid)!=(z2>=mid));
        }
    }
    return sum;
}
vector <char> gen;
vector <cv::Mat> v[256];
int num;
void prep2()
{
    num=0;
    for(char cur='a';cur<='z';++cur) gen.push_back(cur);
    for(char cur='0';cur<='9';++cur) gen.push_back(cur);
}
string ch2(cv::Mat img,string out_path)
{
    for(int i=0;i<256;++i) v[i].clear();
    cout<<" ch2 "<<endl;
    string ans;
    int n=img.rows;int m=img.cols;
    if(n<=10 || m<=10) return ans;
    for(auto h:gen)
    {
        string h1;h1.push_back(h);
        for(int i=0;i<SAMPLES_PER_LETTER;++i)
        {
            v[h].push_back(generateImage3(h1,m,n));
            //cout<<n<<" n "<<m<<" m "<<v[h].back().rows<<" v[h].back().rows "<<v[h].back().cols<<" v[h].back().cols "<<endl;
        }
    }
    for(int i=0;i<SAMPLES_PER_LETTER;++i) cv::imwrite(out_path +"/"+ "000000"+to_string(i)+"_a00000000.jpg", v['a'][i]);
    int el=(-1);int dist=1e9;
    for(int i=0;i<256;++i)
    {
        if(v[i].empty()) continue;
        for(auto h:v[i])
        {
            int o=dist1(h,img);
            if(el==(-1) || o<=dist)
            {
                dist=o;
                el=i;
            }
        }
    }
    cout<<dist<<" dist "<<endl;
    cout<<((char) el)<<" el "<<endl;
    ans.push_back(el);
    return ans;
}