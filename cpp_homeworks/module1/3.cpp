#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

vector<int> PrefixFunctionFromZ(const vector<int>& z_function){
    size_t string_size = z_function.size();
    vector<int> prefix_function (string_size);

    for(size_t i = 1; i < string_size; i++){
        for(int j = z_function[i] - 1; j >= 0; j--){
            if(prefix_function[i+j] > 0){
                break;
            }
            else
            {
                prefix_function[i+j] = j+1;
            }
        }
    }

    return prefix_function;
}

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
                int w = tmp - 'a';
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
    vector<int> z_function;

    while(cin >> to_read){
        if(to_read == -1){
            break;
        }

        z_function.push_back(to_read);
    }

    cout << buildFromPrefix(PrefixFunctionFromZ(z_function));
    return 0;
}