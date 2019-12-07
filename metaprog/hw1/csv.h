#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <exception>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/token_iterator.hpp>

template<typename T>
T convert_to(std::string str);

template<>
std::string convert_to (std::string str) {
    return str;
}

template <typename T> T convert_to (std::string str) {
    std::istringstream ss(str);
    T value;
    ss >> value;
    return value;
}

typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
template<typename... Args>
class csv {
public:
    std::vector<std::tuple<Args...>> readFile(const std::string& fileName) {
        std::vector<std::tuple<Args...>> result;

        std::ifstream file (fileName);
        if (file.is_open()) {
            std::string line;
            while (getline(file, line)){
                result.push_back(readLine(line));
            }
            file.close();
        } else {
            throw std::invalid_argument("Unable to open file");
        }

        return result;
    }

private:
    const char SEPARATOR = ',';

    std::tuple<Args...> readLine(const std::string& line) {
        boost::char_separator<char> sep(&SEPARATOR);
        boost::tokenizer<boost::char_separator<char>> tokens(line, sep); // split line by sep; e.g '1,2'->['1','2']

        std::tuple<Args...> tupleLine;
        auto it = tokens.begin();
        readTuple<0, std::tuple<Args...>, Args...> (it, tupleLine);

        return tupleLine;
    }

    template <int index, typename Tuple, typename Type, typename... Remaining>
    void readTuple(tokenizer::iterator& it, Tuple& tuple) {
        std::get<index>(tuple) = convert_to<Type> (*it);
        it++;
        readTuple<index + 1, Tuple, Remaining...> (it, tuple);
    }

    template <int index, typename Tuple>
    void readTuple(tokenizer::iterator& it, Tuple& tupleLine) {}
};

#endif