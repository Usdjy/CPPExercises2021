#include <bits/stdc++.h>
#include "some_math.h"

using namespace std;

int fibbonachiRecursive(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibbonachiRecursive(n - 1) + fibbonachiRecursive(n - 2);
}
vector<vector<int> > mul (vector<vector<int> > v1, vector<vector<int> > v2) {
    vector<vector<int> > ans(2);
    for (int i = 0; i < 2; ++i) ans[i].resize(2);
    for (auto &h:ans) for (auto &h1:h) h1 = 0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                ans[i][k] += v1[i][j] * v2[j][k];
            return ans;
};
vector<vector<int> > po(vector<vector<int> > v, int n) {
    if (n == 1) return v;
    if (n % 2 == 0) {
        vector<vector<int> > u = po(v, n / 2);
        return mul(u, u);
    } else {
        vector<vector<int> > u = po(v, n - 1);
        return mul(u, v);
    }
};
int fibbonachiFast(int n) {
    // 04 реализуйте быструю функцию Фибоначчи с использованием std::vector
    if (n == 0) return 0;
    if(n==1) return 1;
    vector<vector<int> > v;
    v = {{1, 1},
         {1, 0}};
    vector<vector<int> > u = po(v, n-1);
    return u[0][0];
}

double solveLinearAXB(double a, double b) {
    //  10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    const double md = std::numeric_limits<double>::max(); //???
    if (a == 0 && b == 0) return (-md);
    if (a == 0) return md;
    return (-b / a);
}

std::vector<double> solveSquare(double a, double b, double c) {
    //  20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> res;
    // чтобы добавить в вектор элемент - нужно вызвать у него метод push_back:
    if(a==0)
    {
        if(b==0) return res;
        else return {-c/b};
    }
    if (b * b == (4 * a * c)) {
        res.push_back((-b) / (2 * a));
        return res;
    }
    if (b * b < (4 * a * c)) {
        return res;
    }
    res.push_back((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
    res.push_back((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
    sort(res.begin(), res.end());
    return res;
}
