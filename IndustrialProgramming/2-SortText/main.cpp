#include <algorithm>
#include <cstring>
#include <set>
#include "Text.hpp"

const static std::set<char> punctuationMarks = {'!','?', '-', ';', ':',
                                                '>', '<', '[', ']' ,'"',
                                                ' ', '\'', '(', ')', '.', ','};

void writeToFile(Text& text, const std::string& outputFile) {
    std::fstream file;
    file.open(outputFile, std::ios::out);
    for (int i = 0; i < text.size(); i++) {
        if (*text.begin()[i] != '\000') {
            file << text.begin()[i] << "\n";
        }
    }
    file.close();
}

void writeInAlphabetOrder(Text& text, const std::string& outputFile) {
    std::sort(text.begin(), text.begin()+text.size(), [](char* word1, char* word2){
        return strcmp(word1, word2) < 0;
    });

    writeToFile(text, outputFile);
}

char findPos(const char* word, size_t& index) {
    char symbol = word[index - 1];
    while (punctuationMarks.find(symbol) != punctuationMarks.end()) {
        index -= 1;
        symbol = word[index - 1];
    }
    return symbol;
}

void writeInReverseOrder(Text& text, const std::string& outputFile) {
    std::sort(text.begin(), text.begin() + text.size(), [](char *word1, char *word2) {
        size_t firstIndex  = strlen(word1);
        size_t secondIndex = strlen(word2);

        while (firstIndex != 0 && secondIndex != 0) {
            char firstSymbol  = findPos(word1, firstIndex);
            char secondSymbol = findPos(word2, secondIndex);

            if (firstSymbol == secondSymbol) {
                firstIndex--;
                secondIndex--;
            } else {
                return firstSymbol < secondSymbol;
            }
        }

        if (firstIndex == 0 && secondIndex == 0) {
            return false;
        }

        return firstIndex > 0;
    });

    writeToFile(text, outputFile);
}

int main () {
    char inputFile[] = "shakespeare.txt";
    Text text (inputFile);

    std::string outputFile ("sort.txt");
    writeInAlphabetOrder(text, "inAlphabetOrder.txt");
    writeInReverseOrder(text, "inReverseOrder.txt");
    text.writeOriginal("original.txt");
    return 0;
}