#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class num_of_substrings {
public:
    explicit num_of_substrings(const string& _text) {
        text = _text + '$';
        size = text.length();
    }

    int answer() {
        vector<int> suffix_array = build_suffix_array();
        vector<int> LCP = build_LCP(suffix_array);

        int count = size - 1 - suffix_array[0];
        for(int i = 1; i < size; i++){
            count += (size-1) - suffix_array[i] - LCP[i];
        }
        return (count - 1);
    }
private:
    string text;
    size_t size;

    struct suffix
    {
        int index;
        int rank[2];

        suffix(int _index, int _first_rank, int _second_rank) : index(_index) {
            rank[0] = _first_rank;
            rank[1] = _second_rank;
        }

        suffix() : index(-1), rank() {}

        bool operator < (const suffix& other) {
            if(this->rank[0] == other.rank[0]) {
                return (this->rank[1] < other.rank[1]);
            }
            return this->rank[0] < other.rank[0];
        }
    };

    vector<int> build_suffix_array()
    {
        size_t size = text.length();
        vector<suffix> tmp_suffix_array;
        tmp_suffix_array.resize(size);
        for (int i = 0; i < size; i++)
        {
            tmp_suffix_array[i].index = i;
            tmp_suffix_array[i].rank[0] = text[i] - 'a';
            if(i+1 < size) {
                tmp_suffix_array[i].rank[1] = text[i+1] - 'a';
            }
            else {
                tmp_suffix_array[i].rank[1] = -1;
            }
        }
        sort(tmp_suffix_array.begin(), tmp_suffix_array.begin() + size);
        vector<int> tmp(size, 0);
        for (int degree = 4; degree < 2 * size; degree = degree * 2)
        {
            int rank = 0;
            int previous_rank = tmp_suffix_array[0].rank[0];
            tmp_suffix_array[0].rank[0] = rank;
            tmp[tmp_suffix_array[0].index] = 0;

            for (int j = 1; j < size; j++)
            {
                if (tmp_suffix_array[j].rank[0] == previous_rank && tmp_suffix_array[j].rank[1] == tmp_suffix_array[j - 1].rank[1])
                {
                    previous_rank = tmp_suffix_array[j].rank[0];
                    tmp_suffix_array[j].rank[0] = rank;
                }
                else
                {
                    previous_rank = tmp_suffix_array[j].rank[0];
                    tmp_suffix_array[j].rank[0] = ++rank;
                }
                tmp[tmp_suffix_array[j].index] = j;
            }

            for (int j = 0; j < size; j++)
            {
                int next_index = tmp_suffix_array[j].index + degree / 2;
                if(next_index < size) {
                    tmp_suffix_array[j].rank[1] = tmp_suffix_array[tmp[next_index]].rank[0];
                }
                else {
                    tmp_suffix_array[j].rank[1] = -1;
                }
            }
            sort(tmp_suffix_array.begin(), tmp_suffix_array.begin() + size);
        }
        vector<int>suffix_array(size, 0);
        for (int i = 0; i < size; i++) {
            suffix_array[i] = tmp_suffix_array[i].index;
        }
        return suffix_array;
    }

    vector<int> build_LCP(const vector<int>& suffix_array){
        vector<int> LCP (size);
        vector<int> positions (size);
        for(int i = 0; i < size; i++){
            positions[suffix_array[i]] = i;
        }
        int num = 0;
        for(int i = 0; i < size; i++){
            if(num > 0){
                num--;
            }
            if(positions[i] == size-1){
                LCP[size-1] = -1;
                num = 0;
            }
            else{
                int tmp = suffix_array[positions[i]+1];
                while( (max(i+num,tmp+num) < size) && (text[i+num] == text[tmp+num] ) ){
                    num++;
                }
                LCP[positions[i]] = num;
            }
        }
        return LCP;
    }
};

int main() {
    string origin_string;
    cin >> origin_string;

    num_of_substrings solver(origin_string);
    cout << solver.answer();
    return 0;
}