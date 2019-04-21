#include <iostream>
#include <vector>
#include <tgmath.h>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;

const double  pi = atan(1.0) * 4.0;

// Структура, задающая одну точку
struct Dot{
    double x;
    double y;

    Dot(double X, double Y) : x(X), y(Y) {}
};

// Структура, задающая выпуклый многоугольник
struct Polygon{
    vector<Dot> points; // Точки граней

    void add_dot(double x, double y){
        Dot tmp(x,y);
        points.push_back(tmp);
    }
};

// Возвращает полярный угол для вектора, заданного двумя точками
double polar_angle(const Dot& first,const Dot& Second){
    double x = Second.x - first.x;
    double y = Second.y - first.y;

    if(x > 0 && y >= 0) {
        return atan(y/x);
    }
    if(x > 0 && y < 0) {
        return atan(y/x) + 2*pi;
    }
    if(x < 0) {
        return atan(y/x) + pi;
    }
    if(x == 0 && y > 0){
        return pi/2;
    }
    if(x == 0 && y < 0){
        return 3*pi/2;
    }
}

// Считываем многоугольник
void cin_Polygon(Polygon& to_read, bool minus=false){
    double size;    // Количество точек
    cin >> size;

    int start = 0;  // Номер точки, которая будет крайней
    double min_x = 800000;
    double min_y = 800000;
    for(int i = 0; i < size; i++){
        double x = 0;   // Координата x точки
        double y = 0;   // Координата y точки

        cin >> x >> y;

        if(minus) { // Если надо получить фигуру, умноженную на минус
            x *= -1;
            y *= -1;

            if(y < min_y) {
                min_x = x;
                min_y = y;
                start = i;
            }  else if(y == min_y) {
                if(x <= min_x) {
                    min_x = x;
                    min_y = y;
                    start = i;
                }
            }
            to_read.add_dot(x, y);
        } else {
            if(y < min_y) {
                min_x = x;
                min_y = y;
                start = i;
            } else if(y == min_y) {
                if(x <= min_x) {
                    min_x = x;
                    min_y = y;
                    start = i;
                }
            }
            to_read.add_dot(x, y);
        }
    }
    // Повернем точки так, чтобы крайняя оказалась 0-ой
    std::rotate(to_read.points.begin(), to_read.points.begin()+start, to_read.points.end());

    // Сделаем обход проти в часовой стрелки
    for(int i = 1; i < size/2; i++){
        Dot tmp = to_read.points[i];
        to_read.points[i] = to_read.points[size-i];
        to_read.points[size-i] = tmp;
    }
}

// Разница Минковского:  Q - P = Q + (-P)
Polygon min_diff(const Polygon& Q,const Polygon&  P){
    size_t Q_size = Q.points.size(); // Количество точек в первом многоугольнике
    size_t P_size = P.points.size(); // Количество точек во втором многоугольнике

    int Q_pointer = 0;  // Количество уже пройденных точек в первом
    int P_pointer = 0;  // Количество уже пройденных точек во втором

    Polygon tmp;  // Многоугольник, хранящий разницу Q и P
    while(Q_pointer < Q_size || P_pointer < P_size) {
        double x_coor = Q.points[Q_pointer%Q_size].x + P.points[P_pointer%P_size].x;
        double y_coor = Q.points[Q_pointer%Q_size].y + P.points[P_pointer%P_size].y;

        tmp.add_dot(x_coor, y_coor);

        double Q_angle = polar_angle(Q.points[Q_pointer%Q_size], Q.points[(Q_pointer + 1)%Q_size]);

        double P_angle = polar_angle(P.points[P_pointer%P_size], P.points[(P_pointer + 1)%P_size]);

        if (Q_angle == 0 && P_angle >= pi) {
            Q_angle = 2*pi;
        }

        if (P_angle == 0 && Q_angle >= pi) {
            P_angle = 2*pi;
        }


        if(Q_angle < P_angle){
            if(Q_pointer != Q_size) {
                Q_pointer++;
            } else{
                P_pointer++;
            }

        }
        else if (P_angle < Q_angle){
            if(P_pointer != P_size) {
                P_pointer++;
            } else{
                Q_pointer++;
            }
        }
        else {
            Q_pointer++;
            P_pointer++;
        }
    }
    return tmp;
}

// Проверяет слева ли точка P2 относительно прямой, задаваемой точками P0 и P1
inline double isLeft(const Dot& P0,const Dot& P1,const Dot& P2 )
{
    return ( (P1.x - P0.x) * (P2.y - P0.y) - (P2.x -  P0.x) * (P1.y - P0.y) );
}

// Проверка на то лежит ли (0, 0) в нашем многоугольнике
bool Check(const Polygon& tmp){
    Dot P2(0,0);
    for(int i = 0; i < tmp.points.size()-1;i++){
        double side = isLeft(tmp.points[i], tmp.points[i+1], P2);
        if(side < 0) {
            return false;
        }
    }
    return true;
}

int main() {
    // Считываем многоугольник Q
    Polygon Q;
    cin_Polygon(Q);

    // Считываем многоугольник P
    Polygon P;
    cin_Polygon(P, true);

    // Посчитаем разницу Минковского двух фигур
    Polygon tmp = min_diff(Q,P);

    // Добавим точку, чтобы образовать цикл
    Dot T = tmp.points[0];
    tmp.points.push_back(T);


    bool ans = Check(tmp);
    if(ans){
        cout << "YES";
    }
    else {
        cout << "NO";
    }

    return 0;
}
