#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;

class distance_between_segments{
public:

    explicit distance_between_segments(const vector<vector<double >>& input) : 
            P0 ({input[0][0],input[0][1], input[0][2]}),
            P1 ({input[1][0],input[1][1], input[1][2]}),
            P2 ({input[2][0],input[2][1], input[2][2]}),
            P3 ({input[3][0],input[3][1], input[3][2]}) {}
    
    double solve(){
        CVector first(P1, P0);
        CVector second(P3, P2);
        CVector intersect(P0, P2);

        double tmp = dist_between_vectors(first, first)  * dist_between_vectors(second, second) -
                     dist_between_vectors(first, second) * dist_between_vectors(first, second);

        double first_numerator = 0.0;
        double first_divider = tmp;

        double second_numerator = 0.0;
        double second_divider = tmp;

        if (tmp <= EPSILON) {
            first_divider = 1.0;
            second_numerator = dist_between_vectors(second, intersect);
            second_divider = dist_between_vectors(second, second);
        }
        else {
            first_numerator = (dist_between_vectors(first, second)  * dist_between_vectors(second, intersect) -
                               dist_between_vectors(second, second) * dist_between_vectors(first, intersect));

            second_numerator = (dist_between_vectors(first, first)  * dist_between_vectors(second, intersect) -
                                dist_between_vectors(first, second) * dist_between_vectors(first, intersect));

            if (first_numerator <= 0.0) {
                first_numerator = 0.0;
                second_numerator = dist_between_vectors(second, intersect);
                second_divider = dist_between_vectors(second, second);
            }
            else if (first_numerator >= tmp) {
                first_numerator = tmp;
                second_numerator = dist_between_vectors(second, intersect) + dist_between_vectors(first, second);
                second_divider = dist_between_vectors(second, second);
            }
        }

        if (second_numerator < 0.0) {
            second_numerator = 0.0;
            if (dist_between_vectors(first, intersect) >= 0.0) {
                first_numerator = 0.0;
            }
            else if (dist_between_vectors(first, intersect) <= dist_between_vectors(first, first)) {
                first_numerator = tmp;
            }
            else {
                first_numerator = -1.0 * dist_between_vectors(first, intersect);
                first_divider = -1.0 * dist_between_vectors(first, first);
            }
        }
        else if (second_numerator >= second_divider) {
            second_numerator = second_divider;
            if ( (dist_between_vectors(first, second) - dist_between_vectors(first, intersect)) <= 0.0) {
                first_numerator = 0.0;
            }
            else if ((dist_between_vectors(first, second) - dist_between_vectors(first, intersect))
                     >= dist_between_vectors(first, first)) {
                first_numerator = tmp;
            }
            else {
                first_numerator = (dist_between_vectors(first, second) - dist_between_vectors(first, intersect));
                first_divider = dist_between_vectors(first, first);
            }
        }

        double alpha = 0.0;
        if(abs(first_numerator) < EPSILON){
            alpha = 0.0;
        } else {
            alpha = first_numerator / first_divider;
        }

        double beta = 0.0;
        if(abs(second_numerator) < EPSILON){
            beta = 0.0;
        } else {
            beta = second_numerator / second_divider;
        }

        return (intersect + first*alpha - second*beta).len();
    }

private:
    const double EPSILON = 0.0000001;

    struct Point{
        double x;
        double y;
        double z;

        Point(const double X,const double Y,const double Z) : x(X), y(Y), z(Z) {}
    };
    Point P0; Point P1;
    Point P2; Point P3;

    struct CVector{
        Point dot;

        explicit CVector(const Point& m_dot) : dot(m_dot) {}

        CVector(const Point& m_start, const Point& m_dot) : dot(m_dot.x - m_start.x,
                                                                m_dot.y - m_start.y,
                                                                m_dot.z - m_start.z)  {}

        CVector operator + (const CVector& other) {
            return CVector(Point(dot.x + other.dot.x,
                                 dot.y + other.dot.y,
                                 dot.z + other.dot.z));
        }

        CVector operator - (const CVector& other) {
            return CVector(Point(dot.x - other.dot.x,
                                 dot.y - other.dot.y,
                                 dot.z - other.dot.z));
        }

        CVector operator * (const double& number) {
            return CVector(Point(dot.x * number,
                                 dot.y * number,
                                 dot.z * number));
        }

        double len() {
            return sqrt(dot.x * dot.x +
                        dot.y * dot.y +
                        dot.z * dot.z);
        }
    };
    
    inline double dist_between_vectors(const CVector &first, const CVector &second){
        return (first.dot.x * second.dot.x +
                first.dot.y * second.dot.y +
                first.dot.z * second.dot.z);
    }
};





int main() {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    vector<vector<double >> input;
    for(int i = 0; i < 4; i++) {
        cin >> x;
        cin >> y;
        cin >> z;
        vector<double> tmp;
        tmp.push_back(x);
        tmp.push_back(y);
        tmp.push_back(z);
        input.push_back(tmp);
    }

    distance_between_segments solver(input);
    cout << setprecision(10) << solver.solve();
    return 0;
}

