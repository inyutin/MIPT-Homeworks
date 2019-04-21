#include <Text.hpp>
#include <stack>

std::vector<int> Text::InfixToPostfix(const std::string &expr) {
    std::string clear = clearText(expr);

    std::vector<int> output;
    std::stack<char> stack;

    std::map<char, int> priorities;
    priorities['+'] = 1;
    priorities['-'] = 1;
    priorities['*'] = 2;
    priorities['/'] = 2;

    for (auto it = clear.begin(); it < clear.end(); it++) {
        if (std::isdigit(*it)) {
            int number = 0;
            while (*it >= '0' && *it <= '9') {
                number = number * 10 + (*it - '0');
                it++;
            }
            it--;
            output.push_back(number+300);
        } else if (*it == 'x'){
            output.push_back(*it);
        } else if (*it == 'c') {
            if (*(it+1) != 'o' || *(it+2) != 's') {
                throw std::invalid_argument("Выражение невалидно!");
            }
            stack.push(*it);
            it+=2;
        } else if (*it == 's') {
            if (*(it+1) != 'i' || *(it+2) != 'n') {
                throw std::invalid_argument("Выражение невалидно!");
            }
            stack.push(*it);
            it+=2;
        } else if (*it == '(') {
            stack.push(*it);
        } else if (*it == ')') {
            while (!stack.empty() && stack.top() != '(') {
                output.push_back(stack.top());
                stack.pop();
            }
            if (stack.empty()) {
                throw std::invalid_argument("В выражении пропущена скобка");
            }
            stack.pop();
            if (!stack.empty()) {
                if (stack.top() == 'c' || stack.top() == 's') {
                    output.push_back(stack.top());
                    stack.pop();
                }
            }
        } else {
            if ((!stack.empty()) && stack.top() != '(') {
                if ((priorities[stack.top()] >= priorities[*it])) {
                    output.push_back(stack.top());
                    stack.pop();
                    stack.push(*it);
                } else if ((priorities[stack.top()] < priorities[*it])) {
                    stack.push(*it);
                }
            } else {
                stack.push(*it);
            }
        }
    }

    while (!stack.empty()) {
        if (stack.top() == '(' || stack.top() == ')') {
            throw std::invalid_argument("В выражении присутствует незакрытая скобка");
        }
        output.push_back(stack.top());
        stack.pop();
    }

    std::reverse(output.begin(), output.end());
    return output;
}