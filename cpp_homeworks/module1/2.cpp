#include <cstddef>
#include <iostream>
#include "vector"

using namespace std;

string buildFromPrefix(const vector<int>& prefix_function){
    string to_return = "a";     // Строка, которую мы строим.
    size_t line_size = prefix_function.size();    // Размер этой строки
    for(size_t i = 1; i < line_size; i++){     // Обработка каждого числа в префикс-функции
        if(prefix_function[i] == 0){
            int position = i;
            string alphabet = "@bcdefghijklmnopqrstuvwxyz";
            int alphabet_size = 27;
            position = prefix_function[position - 1];
            while (position > 0) {
                char tmp = to_return[position];
                int w = (int) tmp - 97;
                alphabet[w] = '@';
                position = prefix_function[position - 1];
            }
            for(int j = 0; j < alphabet_size; j++){
                if(alphabet[j] != '@'){
                    to_return +=  alphabet[j];
                    break;
                }
            }
        }
        else{
            to_return += to_return[prefix_function[i]-1];
        }
    }
    return to_return;
}

int main() {
    int to_read = -1;
    vector<int> prefix_function;

    while(cin >> to_read){
        prefix_function.push_back(to_read);
    }

    cout << buildFromPrefix(prefix_function);
    return 0;
}