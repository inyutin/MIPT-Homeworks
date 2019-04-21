#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <set>

using std::vector;
using std::queue;
using std::set;
using std::cin;
using std::cout;
using std::endl;

struct Point{
    double x;   // Координата точки по x
    double y;   // Координата точки по y
    double z;   // Координата точки по z
    int num;    // Номер точки

    Point();
    Point(Point A, Point B);
    Point(double X, double Y, double Z);

    double Mod();
};

Point::Point() : x(0), y(0), z(0), num(-1) {}
Point::Point(Point A, Point B) : x(B.x-A.x), y(B.y-A.y), z(B.z-A.z), num(-1) {}
Point::Point(double X, double Y, double Z) : x(X), y(Y), z(Z), num(-1) {}
double Point::Mod(){
    return sqrt(x*x+y*y+z*z);
}

Point static CrossProduct (const Point& A,const Point& B) {
    return Point{ A.y*B.z-A.z*B.y,
                  A.z*B.x-A.x*B.z,
                  A.x*B.y-A.y*B.x };
}
double static Scalar(const Point& A,const Point& B) {
    return(A.x*B.x+A.y*B.y+A.z*B.z);
}

struct Flat {
    Point First;
    Point Second;
    Point Third;

    Flat();
    Flat(Point F, Point S, Point T);

    Point Normal();
    void sort(vector<Point> &dots);

    bool operator<(const Flat& other) const;
    bool operator==(const Flat& other) const;
};

Flat::Flat() : First(), Second(), Third() {}
Flat::Flat(Point F, Point S, Point T) : First(F), Second(S), Third(T) {
    while (Second.num < First.num || Third.num < First.num) {
        Point tmp = First;
        First = Second;
        Second = Third;
        Third = tmp;
    }
}

Point Flat::Normal() {
    Point u1(First,Second);
    Point u2(First,Third);
    Point norm = CrossProduct(u1,u2);
    return norm;
}
void Flat::sort(vector<Point> &dots) {
    Point help;
    if (First.num != 0 && Second.num != 0 && Third.num != 0) {
        help = dots[0];
    } else if (First.num != 1 && Second.num != 1 && Third.num != 1) {
        help = dots[1];
    } else if (First.num != 2 && Second.num != 2 && Third.num != 2) {
        help = dots[2];
    } else {
        help = dots[3];
    }

    Point u1(First,Second);
    Point u2(First,Third);
    Point u3(First, help);

    Point norm = CrossProduct(u1,u2);
    if(Scalar(norm,u3) > 0) {
        Point tmp = Second;
        Second = Third;
        Third = tmp;
    }
}

bool Flat::operator<(const Flat& other) const {
    return (First.num < other.First.num || (First.num == other.First.num && (Second.num < other.Second.num ||
                                                                             Second.num == other.Second.num && Third.num < other.Third.num))) ? true : false;
}
bool Flat::operator==(const Flat& other) const {
    return (First.num == other.First.num & Second.num == other.Second.num && Third.num == other.Third.num) ? true : false;
}

class ConvexHull {
public:
    explicit ConvexHull(size_t num);
    ~ConvexHull();
    void read_dots();
    void print_convex();
private:
    const int MAX_COORDINATE = 1000000;
    vector<Point> dots;
    size_t num_dots;

    Flat initialize();
    double get_angle(Flat H, Flat S);
};

ConvexHull::ConvexHull(size_t num) {
    dots.clear();
    num_dots = num;
}
ConvexHull::~ConvexHull() {
    dots.clear();
}
void ConvexHull::read_dots(){
    for(int i = 0; i < num_dots; i++) {
        double x = 0;
        double y = 0;
        double z = 0;
        cin >> x >> y >> z;
        Point tmp(x, y, z);
        tmp.num = i;
        dots.push_back(tmp);
    }
}
void ConvexHull::print_convex() {
    set<Flat>ans;

    queue<Flat> edges;
    Flat FirstFace = initialize();
    edges.push(FirstFace);
    ans.insert(FirstFace);

    vector<vector<bool>> edges_proceed;
    edges_proceed.resize(num_dots);
    for(int i = 0; i < num_dots; i++){
        edges_proceed[i].resize(num_dots);
    }

    set<set<int>> triplets_of_vertices; // Для запоминания тройки вершин - треугольника
    while(!edges.empty()) {
        Flat flat_in_action = edges.front();
        edges.pop();
        Point flat_normal = flat_in_action.Normal(dots);
        if (!edges_proceed[flat_in_action.First.num][flat_in_action.Second.num]) {
            Flat new_flat;
            double first_angle = -M_PI;
            for (int i = 0; i <num_dots; ++i) {
                if (flat_in_action.First.num != i && flat_in_action.Second.num != i && flat_in_action.Third.num != i) {
                    Flat tmp_flat = Flat(flat_in_action.First, dots[i], flat_in_action.Second, dots);
                    Point tmp_normal = CrossProduct(Point(tmp_flat.First,tmp_flat.Second),Point(tmp_flat.First,tmp_flat.Third));
                    double tmp_angle = Scalar(flat_normal, tmp_normal) / (flat_normal.Mod() * tmp_normal.Mod());

                    if (tmp_angle > first_angle) {
                        first_angle = tmp_angle;
                        new_flat = tmp_flat;
                    }
                }
            }
            edges_proceed[flat_in_action.First.num][flat_in_action.Second.num] = true;
            edges_proceed[flat_in_action.Second.num][flat_in_action.First.num] = true;
            if(ans.count(new_flat) == 0) {
                edges.push(new_flat);
                ans.insert(new_flat);
            }
        }
        if (!edges_proceed[flat_in_action.Second.num][flat_in_action.Third.num]) {
            Flat new_flat;
            double first_angle = -M_PI;
            for (int i = 0; i <num_dots; ++i) {
                if (flat_in_action.First.num != i && flat_in_action.Second.num != i && flat_in_action.Third.num != i) {
                    Flat tmp_flat = Flat(flat_in_action.Second, dots[i], flat_in_action.Third, dots);
                    Point tmp_normal = CrossProduct(Point(tmp_flat.First,tmp_flat.Second),Point(tmp_flat.First,tmp_flat.Third));
                    double tmp_angle = Scalar(flat_normal, tmp_normal) / (flat_normal.Mod() * tmp_normal.Mod());

                    if (tmp_angle > first_angle) {
                        first_angle = tmp_angle;
                        new_flat = tmp_flat;
                    }
                }
            }
            edges_proceed[flat_in_action.Second.num][flat_in_action.Third.num] = true;
            edges_proceed[flat_in_action.Third.num][flat_in_action.Second.num] = true;
            if(ans.count(new_flat) == 0) {
                edges.push(new_flat);
                ans.insert(new_flat);
            }
        }
        if (!edges_proceed[flat_in_action.Third.num][flat_in_action.First.num]) {
            Flat new_flat;
            double first_angle = -M_PI;
            for (int i = 0; i <num_dots; ++i) {
                if (flat_in_action.First.num != i && flat_in_action.Second.num != i && flat_in_action.Third.num != i) {
                    Flat tmp_flat = Flat(flat_in_action.Third, dots[i], flat_in_action.First, dots);
                    Point tmp_normal = CrossProduct(Point(tmp_flat.First,tmp_flat.Second),Point(tmp_flat.First,tmp_flat.Third));
                    double tmp_angle = Scalar(flat_normal, tmp_normal) / (flat_normal.Mod() * tmp_normal.Mod());

                    if (tmp_angle > first_angle) {
                        first_angle = tmp_angle;
                        new_flat = tmp_flat;
                    }
                }
            }
            edges_proceed[flat_in_action.Third.num][flat_in_action.First.num] = true;
            edges_proceed[flat_in_action.First.num][flat_in_action.Third.num] = true;
            if(ans.count(new_flat) == 0) {
                edges.push(new_flat);
                ans.insert(new_flat);
            }
        }
    }
    cout << ans.size() << endl;
    for (Flat face: ans) {
        cout << 3 << ' ' << face.First.num << ' ' << face.Second.num << ' ' << face.Third.num << endl;
    }
}

Flat ConvexHull::initialize() {
    // Первая точка инициализации
    Point first_point(MAX_COORDINATE, MAX_COORDINATE, MAX_COORDINATE);

    for (int i = 0; i < num_dots; i++) {  // Находим эту самую точку first_point
        if (dots[i].z < first_point.z) {
            first_point = dots[i];
        } else if (dots[i].z == first_point.z) {
            if (dots[i].x < first_point.x) {
                first_point = dots[i];
            } else if (dots[i].x == first_point.x) {
                if (dots[i].y < first_point.y) {
                    first_point = dots[i];
                }
            }
        }
    }
    // В качестве l возьмем  прямую проходящую через first_point и какую-то случайную точку
    Point l_point(rand() % 100 + 1985, rand() % 30 + 1985, first_point.z);

    // Вторая точка первого треугольника
    Point second_point(MAX_COORDINATE, MAX_COORDINATE, MAX_COORDINATE);

    // Плоскость, проходящая через first_point и перпендекулярная Oz
    Flat H(first_point,l_point,Point(rand() % 100 - 1985, rand() % 30 - 1985, first_point.z),dots);

    double max_angle = -1*M_PI;
    Flat G;
    for (int i = 0; i < num_dots; i++) {
        if (i != first_point.num) {
            Flat tmp_G(dots[first_point.num], l_point, dots[i], dots);
            double angle = get_angle(H, tmp_G);
            if (angle >= max_angle) {
                G = tmp_G;
                max_angle = angle;
                second_point = dots[i];
            }
        }
    }

    Point third_point(MAX_COORDINATE, MAX_COORDINATE, MAX_COORDINATE);
    max_angle = -1*M_PI;
    for (int i = 0; i < num_dots; i++) {
        if (i != first_point.num && i != second_point.num) {
            double angle = get_angle(G, Flat(dots[first_point.num], second_point, dots[i], dots));
            if (angle >= max_angle) {
                max_angle = angle;
                third_point = dots[i];
            }
        }
    }

    Flat FirstFace = Flat(first_point, second_point, third_point, dots);
    FirstFace.sort(dots);
    return FirstFace;
}
double ConvexHull::get_angle(Flat H, Flat S) {
    Point norm_H = CrossProduct(Point(H.First, H.Second), Point(H.First, H.Third));
    Point norm_S = CrossProduct(Point(S.First, S.Second), Point(S.First, S.Third));

    double numerator = Scalar(norm_H, norm_S);
    double denominator = norm_H.Mod()*norm_S.Mod();

    return  numerator/denominator;
}

int main() {
    int num_tests = 0;
    cin >> num_tests;

    for(int test = 0; test < num_tests; test++){
        size_t num_dots = 0;
        cin >> num_dots;

        ConvexHull shell(num_dots);
        shell.read_dots();
        shell.print_convex();
    }
}
