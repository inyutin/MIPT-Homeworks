#include <iostream>
#include <vector>

using namespace std;
const int ALPHABET_SIZE = 256; // размер алфавита

vector<int> build_suffix_array(const string& origin_string){
    const size_t STRING_SIZE = origin_string.size();
    vector<int> suffix_array(STRING_SIZE);
    vector<int> buckets(ALPHABET_SIZE);
    vector<int> classes (STRING_SIZE);

    for (auto i=0; i<STRING_SIZE; i++) {
        buckets[origin_string[i]]++;
    }
    for (auto i=1; i<ALPHABET_SIZE; i++) {
        buckets[i] += buckets[i - 1];
    }
    for (auto i=0; i<STRING_SIZE; i++) {
        buckets[origin_string[i]]--;
        suffix_array[buckets[origin_string[i]]] = i;
    }

    classes[suffix_array[0]] = 0;
    int num_of_class = 1;
    for (auto i = 1; i < STRING_SIZE; i++) {
        if (origin_string[suffix_array[i]] != origin_string[suffix_array[i-1]]) {
            num_of_class++;
        }
        classes[suffix_array[i]] = num_of_class-1;
    }

    vector<int> tmp_buckets(ALPHABET_SIZE);
    vector<int> tmp_classes(ALPHABET_SIZE);
    for (auto h=0; (1<<h) < STRING_SIZE; h++) {
        for (size_t  i=0; i<STRING_SIZE; i++) {
            tmp_buckets[i] = suffix_array[i] - (1<<h);
            if (tmp_buckets[i] < 0) {
                tmp_buckets[i] += STRING_SIZE;
            }
        }

        buckets.assign(STRING_SIZE,0);
        for (auto  i=0; i<STRING_SIZE; i++) {
            buckets[classes[tmp_buckets[i]]]++;
        }
        for (auto  i=1; i<num_of_class; i++) {
            buckets[i] += buckets[i - 1];
        }
        for (auto i = int(STRING_SIZE)-1; i>=0; i--) {
            suffix_array[--buckets[classes[tmp_buckets[i]]]] = tmp_buckets[i];
        }

        tmp_classes[suffix_array[0]] = 0;
        num_of_class = 1;
        for (auto i=1; i<STRING_SIZE; i++) {
            size_t first = (suffix_array[i] + (1<<h)) % STRING_SIZE;
            size_t second = (suffix_array[i-1] + (1<<h)) % STRING_SIZE;
            if (classes[suffix_array[i]] != classes[suffix_array[i-1]] || classes[first] != classes[second]) {
                ++num_of_class;
            }
            tmp_classes[suffix_array[i]] = num_of_class-1;
        }
        for(int i = 0; i < STRING_SIZE; i++){
            classes[i] = tmp_classes[i];
        }
    }
    return suffix_array;
}
vector<int> build_LCP(const string &str,const vector<int>& suf){
    size_t size = str.size();
    vector<int> lcp (size);
    vector<int> pos (size);
    for(int i = 0; i < size-1 ; i++){
        pos[suf[i]] = i;
    }
    int num = 0;
    for(auto i = 0; i < size-1; i++){
        if(num > 0){
            num--;
        }
        if(pos[i] == size-1){
            lcp[size-1] = -1;
            num = 0;
        }
        else{
            int tmp = suf[pos[i]+1];
            while( (max(i+num,tmp+num) < size) && (str[i+num] == str[tmp+num] ) ){
                num++;
            }
            lcp[pos[i]] = num;
        }
    }
    return lcp;
}
int main() {
    string origin_string;   //  Исходная строка
    cin >> origin_string;

    origin_string += '$';

    vector<int> suffix_array = build_suffix_array(origin_string);   // Строим массив суффиксов
    vector<int> LCP = build_LCP(origin_string, suffix_array);       // Longest common prefix

    int seq_number = 0;    // Количество подстрок
    size_t size = origin_string.size();
    for(int i = 0; i < size; i++){
        seq_number += (size-1) - suffix_array[i] - LCP[i];
    }
    cout << seq_number;
    return 0;
}