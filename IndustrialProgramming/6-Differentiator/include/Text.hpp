#pragma once
#include <regex>
#include <string>

// Этот namespace нужен для обработки текста.
namespace Text {
    // Отчищает выражение от лишних символов.
    namespace {
        std::string clearText(const std::string &str) {
            std::string tmp;

            // Заменем две `-` на `+`
            tmp = std::regex_replace(str, std::regex("\\-\\-"), "+");

            // Заменяем несколько `+` на 1
            tmp = std::regex_replace(tmp, std::regex("\\+\\++"), "+");

            // Заменяем `-+` to '-`
            tmp = std::regex_replace(tmp, std::regex("\\-\\+"), "-");

            // Заменяем `+-` to '-`
            tmp = std::regex_replace(tmp, std::regex("\\+\\-"), "-");

            // Убираем `+` после `*`
            tmp = std::regex_replace(tmp, std::regex("\\*\\+"), "*");

            // Убираем `+` после `/`
            tmp = std::regex_replace(tmp, std::regex("\\/\\+"), "/");

            // Убираем лишние пробелы
            tmp = std::regex_replace(tmp, std::regex("\\ "), "");

            // Проверим, что все символы валидны
            if (tmp.find_first_not_of("0123456789()-+/*cosinx") != std::string::npos) {
                throw std::invalid_argument("В выражении есть запрещенный символ");
            }

            return tmp;
        }
    }

    // Алгоритм сортировочной станции
    // Переводит выражение в постфиксную запись
    std::vector<int> InfixToPostfix(const std::string &expr);
}