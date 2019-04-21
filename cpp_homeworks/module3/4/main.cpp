#include <iostream>
#include <vector>
#include <set>
#include <array>
using namespace std;

const double MAX = 10000000000;
struct Point {
    double x;
    double y;
    double z;
    int num;
    Point* next;
    Point* prev;

    void action() {
        if(prev->next != this) {
            prev->next = next->prev = this;
        }
        else {
            prev->next = next;
            next->prev = prev;
        }
    }
    bool operator <(const Point &tmp) const {
        return (x < tmp.x || (x == tmp.x && y < tmp.y));
    }

    Point() : x(MAX), y(MAX), z(MAX), num(-1), next(nullptr), prev(nullptr) {}
    Point(double _x, double _y, double _z, int _num) : x(_x), y(_y), z(_z), num(_num), next(nullptr), prev(nullptr) {}
};

class Voronoi {
public:
    explicit Voronoi (const vector<Point>& Points) {
        points = Points;
        null_point = new Point();
    }

    double answer() {
        auto **point_of_third_dimension = third_dimension_hull();
        set<int> point_of_second_dimension_hull = second_dimension_hull();

        size_t num_of_points = points.size() - point_of_second_dimension_hull.size();
        if(num_of_points == 0) {
            return 0;
        }

        int sum = 0;
        for (int i = 0; point_of_third_dimension[i] != null_point; point_of_third_dimension[i++]->action()) {
            auto first = point_of_third_dimension[i]->prev-&points[0];
            auto second = point_of_third_dimension[i]-&points[0];
            auto third = point_of_third_dimension[i]->next-&points[0];

            if(point_of_second_dimension_hull.find(first) == point_of_second_dimension_hull.end()) {
                sum++;
            }
            if(point_of_second_dimension_hull.find(second) == point_of_second_dimension_hull.end()) {
                sum++;
            }
            if(point_of_second_dimension_hull.find(third) == point_of_second_dimension_hull.end()) {
                sum++;
            }
        }
        return (1.0*sum) / num_of_points;
    }
private:
    vector<Point> points;
    int orientation(const Point& first, const Point& second, const Point& third)
    {
        double value = (second.y - first.y) * (third.x - second.x) -
                       (second.x - first.x) * (third.y - second.y);

        if (value == 0) {
            return 0;
        }
        else if (value > 0) {
            return 1;
        }
        return 2;
    }
    set<int> second_dimension_hull()
    {
        set<int> ans;
        size_t size = points.size();

        if (size < 3) {
            return ans;
        }

        int first_point_num = 0;
        for (int current_point_num = 1; current_point_num < size; current_point_num++) {
            if (points[current_point_num].x < points[first_point_num].x) {
                first_point_num = current_point_num;
            }
        }
        int current_point_num = first_point_num;
        do
        {
            ans.insert(points[current_point_num].num);
            int to_be_new_point_num = (current_point_num+1)%size;
            for (int i = 0; i < size; i++)
            {
                if (orientation(points[current_point_num], points[i], points[to_be_new_point_num]) == 2)
                    to_be_new_point_num = i;
            }
            current_point_num = to_be_new_point_num;
        }
        while (current_point_num != first_point_num);
        return ans;
    }

    Point *null_point;
    inline double turn(const Point* first_point, const Point* second_point, const Point* third_point) {
        if (first_point == null_point || second_point == null_point || third_point == null_point) {
            return 1.0;
        }
        return (second_point->x-first_point->x)*(third_point->y-first_point->y) -
               (third_point->x-first_point->x)*(second_point->y-first_point->y);
    }
    inline double time(const Point *first_point,const Point *second_point,const Point *third_point) {
        if (first_point == null_point || second_point == null_point || third_point == null_point) {
            return MAX;
        }
        return ((second_point->x-first_point->x)*(third_point->z-first_point->z)
                - (third_point->x-first_point->x)*(second_point->z-first_point->z))
               / turn(first_point,second_point,third_point);
    }

    Point** third_dimension_hull() {
        size_t point_num = points.size();
        auto *third_dimension_points = &points[0];
        Point *list = sort_points(third_dimension_points, point_num);
        auto **point_of_third_dimension = new Point *[2*point_num];
        auto **tmp = new Point *[2*point_num];
        third_dimension_hull(list, point_num, point_of_third_dimension, tmp);
        return point_of_third_dimension;
    }
    Point *sort_points(Point *current, size_t size) {
        if (size == 1) {
            current[0].next = null_point;
            return current;
        }
        Point *left = sort_points(current, size / 2);
        Point *right = sort_points(current + size / 2, size - size / 2);

        Point head;
        Point *head_pointer = &head;
        do {
            if (left->x < right->x) {
                head_pointer = head_pointer->next = left;
                left = left->next;
            } else {
                head_pointer = head_pointer->next = right;
                right = right->next;
            }
        }
        while (head_pointer != null_point);
        return head.next;
    }
    void third_dimension_hull(Point *list, size_t size, Point **ans, Point **tmp_points) {
        if (size == 1) {
            ans[0] = null_point;
            list->prev = null_point;
            list->next = null_point;
            return;
        }

        Point *main_point = list;
        for (int i = 0; i < (size/2-1); i++) {
            main_point = main_point->next;
        }

        Point *add_point   = main_point->next;
        Point *mid = main_point->next;

        third_dimension_hull(list, size / 2, tmp_points, ans);
        third_dimension_hull(mid, size - size / 2, tmp_points + size / 2 * 2, ans + size / 2 * 2);

        while(true) {
            if (turn(main_point, add_point, add_point->next) < 0) {
                add_point = add_point->next;
            }
            else if (turn(main_point->prev, main_point, add_point) < 0) {
                main_point = main_point->prev;
            }
            else break;
        }

        int right = 0;
        int left = size/2*2;
        int num = 0;

        double value = -MAX;
        while(true) {
            array<double, 6> time_array = {
                    time(tmp_points[right]->prev, tmp_points[right], tmp_points[right]->next),
                    time(tmp_points[left]->prev, tmp_points[left], tmp_points[left]->next),
                    time(main_point, main_point->next, add_point),
                    time(main_point->prev, main_point, add_point),
                    time(main_point, add_point->prev, add_point),
                    time(main_point, add_point, add_point->next)
            };
            int min_left = 0;
            double tmp = MAX;
            for (int i = 0; i < 6; i++) {
                if (time_array[i] > value && time_array[i] < tmp) {
                    min_left = i;
                    tmp = time_array[i];
                }
            }
            if (tmp == MAX) break;
            switch (min_left) {
                case 0:
                    if (tmp_points[right]->x < main_point->x) {
                        ans[num++] = tmp_points[right];
                    }
                    tmp_points[right++]->action();
                    break;
                case 1:
                    if (tmp_points[left]->x > add_point->x) {
                        ans[num++] = tmp_points[left];
                    }
                    tmp_points[left++]->action();
                    break;
                case 2:
                    ans[num++] = main_point = main_point->next;
                    break;
                case 3:
                    ans[num++] = main_point; main_point = main_point->prev;
                    break;
                case 4:
                    ans[num++] = add_point = add_point->prev;
                    break;
                case 5:
                    ans[num++] = add_point; add_point = add_point->next;
                    break;
                default:
                    break;

            }
            value = tmp;
        }
        ans[num] = null_point;
        main_point->next = add_point;
        add_point->prev = main_point;
        for (num--; num >= 0; num--) {
            if (ans[num]->x <= main_point->x || ans[num]->x >= add_point->x) {
                ans[num]->action();
                if (ans[num] == main_point) {
                    main_point = main_point->prev;
                }
                else if (ans[num] == add_point) {
                    add_point = add_point->next;
                }
            }
            else {
                main_point->next = ans[num];
                ans[num]->prev = main_point;
                add_point->prev = ans[num];
                ans[num]->next = add_point;
                if (ans[num]->x < mid->x) {
                    main_point = ans[num];
                }
                else {
                    add_point = ans[num];
                }
            }
        }
    }
};

int main() {
    vector<Point> Points;
    int point_num = 0;
    double x = 0.0;
    double y = 0.0;
    while(cin >> x >> y) {

        if(x==-100) {
            break;
        }
        Points.emplace_back(x, y, x*x+y*y, point_num);
        point_num++;
    }

    Voronoi solver(Points);
    cout << solver.answer();
    return 0;
}
