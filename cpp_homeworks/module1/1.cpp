#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

vector<int> prefix_function(const string& pattern){
    vector<int> tmp;            // Вектор префикс-функции для возврата
    size_t size = pattern.size();  // Размер строчки
    tmp.resize(size);           // Размер Префикс функции.
    tmp[0] = 0;                 // pi(s,0) = 0

    int back_step = 0;                  // Значение префикс-функции на предыдущим шаге
    for(size_t i  = 1; i < size; ++i){     // Перебираем все оставшиеся значения динамикой
        while((back_step > 0) && (pattern[back_step] != pattern[i])){
            back_step = tmp[back_step-1];
        }
        if(pattern[back_step] == pattern[i]){
            ++back_step;
        }
        tmp[i] = back_step;
    }
    return tmp;     // Возвращаем массив префикс-функции
}


int main() {
    string pattern;  // pattern = pattern = шаблон
    string text;  // text = text = текст

    cin >> pattern;
    cin >> text;

    vector<int> tmp = prefix_function(pattern+'@');   // Вычисляем вектор-функцию от шаблона + @
    size_t pattern_size = pattern.size();
    size_t text_size = text.size();

    int back_prefix = 0;
    for(size_t i = 0; i < text_size; i++){      // Вычисляем префикс-функцию для всех символов текста, без сохранения
        while((back_prefix > 0) && (pattern[back_prefix] != text[i])){
            back_prefix = tmp[back_prefix-1];
        }
        if(pattern[back_prefix] == text[i]){
            ++back_prefix;
        }
        if(back_prefix == pattern_size){
            cout << i - pattern_size + 1 << " ";    // Вывод
        }
    }
    return 0;
}
