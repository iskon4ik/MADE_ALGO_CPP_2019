/*
 Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
Обратите внимание: три и более точки из множества могут лежать на одной прямой.
Формат входного файла
Количество точек, далее на каждой строке координаты точек x, y.
Общее количество точек не превосходит 100000.
Координаты точек - числа с плавающей точкой в диапазоне [-1000, 1000].
Формат выходного файла
Выведите периметр выпуклой оболочки.


stdin
3
1. 1.
2. 1.
0. 0.
stdout
4.65028154

stdin
5
1. 1.
2. 0.
0. 0.
0. 2.
2. 2.
stdout
8
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct point {
    double x, y;
};

bool comparing_of_points (point a, point b) {
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool is_right (point a, point b, point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool is_left (point a, point b, point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

double distance_between_points(point a, point b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

void convex_hull (std::vector<point> & points) {

    if(points.size() <= 1) {
        return;
    }

    std::sort(points.begin(), points.end(), &comparing_of_points);
    point p1 = points[0],  p2 = points.back();

    std::vector<point> upper, lower;

    upper.push_back (p1);
    lower.push_back (p1);

    for (size_t i=1; i < points.size(); ++i) {
        if(i == points.size() - 1 || is_right(p1, points[i], p2)) {
            while(upper.size() >= 2 && !is_right(upper[upper.size() - 2], upper[upper.size() - 1], points[i])) {
                upper.pop_back();
            }
            upper.push_back (points[i]);
        }
        if(i == points.size() - 1 || is_left(p1, points[i], p2)) {
            while(lower.size() >= 2 && !is_left(lower[lower.size() - 2], lower[lower.size() - 1], points[i])) {
                lower.pop_back();
            }
            lower.push_back (points[i]);
        }
    }

    std::vector<point> hull;

    points.clear();

    for(size_t i=0; i < upper.size(); ++i) {
        points.push_back (upper[i]);
//        hull.push_back(upper[i]);
    }

    for(size_t i= lower.size() - 2; i > 0; --i) {
        points.push_back (lower[i]);
//        hull.push_back(lower[i]);
    }

//    return hull;
}

double perimetr(std::vector<point> & a) {
//    std::sort(a.begin(), a.end(), &comparing_of_points);
    double answer = 0;
    for(size_t i=0; i<a.size() - 1; ++i) {
        answer += distance_between_points(a[i], a[i+1]);
    }
    answer += distance_between_points(a[a.size() - 1], a[0]);
    return answer;
}

int main() {

    int n;
    std::cin >> n;

    std::vector<point> points;

    for(size_t i=0; i<n; ++i) {
        double x, y;
        std::cin >> x >> y;
        point pt{x, y};
        points.push_back(pt);
    }

    convex_hull(points);
    std::cout.precision(9);
    std::cout << perimetr(points);

    return 0;
}