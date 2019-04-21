#include <Derivative.hpp>

// Публичные методы
AST::AST(const std::string &line) {
    std::vector<int> post = Text::InfixToPostfix(line);
    head = new Node();

    auto it = post.begin();
    auto end = post.end();
    recursivelyBuild(it, end, head);
}
AST::~AST() {
    deleteNode(head);
}
AST AST::derivative(const std::string& file_name, int degree) {
    std::ofstream file;
    file.open (file_name);

    file << latexConst["begin"];
    Node* tmp = head;
    int total = degree;
    while (degree != 0) {
        file << latexConst["skip"];
        file << " Taking " << total-degree+1 << " derivative " << ": "<< latexConst["empty"];
        Node* headDerivated = new Node;
        file << "$( ";
        showTree(tmp, file);
        file << ")'" << "$ = $";
        derivativeRecursively(headDerivated, tmp, file);

        showTree(headDerivated, file);
        file << "$";
        while (optimize(headDerivated)) {
            file << latexConst["empty"] << "equal" << latexConst["empty"];
            file << "$";
            showTree(headDerivated, file);
            file << "$";
        }
        file << latexConst["newline"];
        tmp = headDerivated;
        degree--;
    }
    file << latexConst["skip"];
    file << "$";
    file << "Answer: ";
    showTree(tmp, file);
    file << "$";
    file << latexConst["end"];
    file.close();

    std::string command = ("pdflatex " + file_name);
    std::system(command.c_str());
    return AST(tmp);
}

// Реализация ноды дерева
std::string AST::Node::toString() {
    switch (type) {
        case mul:
            return " \\cdot ";
        case plus:
            return " + ";
        case minus:
            return " - ";
        case div:
            return " / ";
        case sin:
            return "sin";
        case cos:
            return "cos";
        case constant:
            return std::to_string(num);
        case variable:
            return " x ";
        default:
            return "";
    }
}
AST::Node::Node() : num(0), type(constant) {}
AST::Node::Node(Node *pNode) {
    if (pNode == nullptr) {
        return;
    }
    this->type = pNode->type;
    this->num = pNode->num;
    if (pNode->left != nullptr) {
        Node *left_ = new Node(pNode->left);
        this->left = left_;
    }

    if (pNode->right != nullptr) {
        Node *right_ = new Node(pNode->right);
        this->right = right_;
    }
}
AST::Node::Node(Type type) {
    this->type = type;
}
void AST::Node::setType(char c) {
    switch (c) {
        case '+': {
            type = plus;
            return;
        }
        case '-': {
            type = minus;
            return;
        }
        case '*': {
            type = mul;
            return;
        }
        case '/': {
            type = div;
            return;
        }
        case 's': {
            type = sin;
            return;
        }
        case 'c': {
            type = cos;
            return;
        }
        case 'x': {
            type = variable;
            return;
        }
        default:
            type = constant;
            return;
    }
}
void AST::Node::copy(Node *pNode){
    this->type = pNode->type;
    this->num = pNode->num;
    this->left = pNode->left;
    this->right = pNode->right;
}
int AST::Node::getPriority() {
    switch (type) {
        case (plus):
        case (minus):
            return 1;
        case (mul):
        case (div):
            return 2;
        case (sin):
        case (cos):
            return 3;
        case (constant):
        case (variable):
            return -1;
    }
}

void AST::recursivelyBuild(std::vector<int>::iterator &it, std::vector<int>::iterator end, Node *node) {
    if (it >= end) {
        throw std::invalid_argument("Выражение невалидно!");
    }
    if (*it >= 300) {
        node->type = Node::constant;
        node->num = *it - 300;
        it++;
        return;
    }

    char c = char(*it);

    node->setType(c);
    it++;
    switch (c) {
        case 's':
        case 'c': {
            Node *left = new Node();
            recursivelyBuild(it, end, left);

            node->left = left;
            return;
        }
        case '+':
        case '-':
        case '*':
        case '/': {
            Node *left = new Node();
            node->left = left;

            Node *right = new Node();
            node->right = right;

            recursivelyBuild(it, end, node->right);
            recursivelyBuild(it, end, node->left);

            return;
        }
        case ')': {
            recursivelyBuild(it, end, node);
        }
        default:
            return;
    }
}
void AST::derivativeRecursively(Node *d_curNode, Node *curNode, std::ofstream &cout) {
    if (curNode == nullptr) {
        return;
    }
    switch (curNode->type) {
        case Node::variable: {
            d_curNode->type = Node::constant;
            d_curNode->num = 1;
            return;
        }
        case Node::constant: {
            d_curNode->type = Node::constant;
            d_curNode->num = 0;
            return;
        }
        case Node::plus:
        case Node::minus: {
            d_curNode->type = curNode->type;

            Node *left = new Node();

            derivativeRecursively(left, curNode->left, cout);

            d_curNode->left = left;


            Node *right = new Node();

            derivativeRecursively(right, curNode->right, cout);

            d_curNode->right = right;
            return;

        }
        case Node::mul: {
            d_curNode->type = Node::plus;

            Node *newNodeL = new Node();
            newNodeL->type = Node::mul;
            newNodeL->right = curNode->right;

            Node *newLeftNode = new Node();
            derivativeRecursively(newLeftNode, curNode->left, cout);
            newNodeL->left = newLeftNode ;
            d_curNode->left = newNodeL;

            Node *newNodeR = new Node();
            newNodeR->type = Node::mul;
            newNodeR->left = curNode->left;

            Node *newRightNode = new Node();
            derivativeRecursively(newRightNode, curNode->right, cout);
            newNodeR->right = newRightNode;
            d_curNode->right = newNodeR;
            return;
        }
        case Node::div: {
            d_curNode->type = Node::div;

            Node *newNodeL = new Node();
            Node *newNodeLL = new Node();
            Node *newNodeLLL = new Node();
            Node *newNodeLLR = new Node();
            Node *newNodeLR = new Node();
            Node *newNodeLRL = new Node();
            Node *newNodeLRR = new Node();

            derivativeRecursively(newNodeLLL, curNode->left, cout);
            newNodeLLR = curNode->right;

            newNodeLL->type = Node::mul;
            newNodeLL->left = newNodeLLL;
            newNodeLL->right = newNodeLLR;

            derivativeRecursively(newNodeLRL, curNode->right, cout);
            newNodeLRR = curNode->left;

            newNodeLR->type = Node::mul;
            newNodeLR->left = newNodeLRL;
            newNodeLR->right = newNodeLRR;

            newNodeL->type = Node::minus;
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            Node *newNodeR = new Node();
            Node *newNodeRL = new Node();
            Node *newNodeRR = new Node();

            newNodeRL = curNode->right;
            newNodeRR = curNode->right;
            newNodeR->type = Node::mul;
            newNodeR->left = newNodeRL;
            newNodeR->right = newNodeRR;

            d_curNode->left = newNodeL;
            d_curNode->right = newNodeR;
            return;
        }
        case Node::sin: {
            d_curNode->type = Node::mul;

            Node *left = new Node();
            left->type = Node::cos;
            d_curNode->left = left;

            Node *leftCopy = new Node(curNode->left);
            left->left = leftCopy;

            Node *right = new Node();
            d_curNode->right = right;

            derivativeRecursively(right, curNode->left, cout);

            return;
        }
        case Node::cos: {
            d_curNode->type = Node::mul;

            Node *mainleft = new Node();
            mainleft->type = Node::mul;
            d_curNode->left = mainleft;

            Node *left = new Node();
            left->type = Node::sin;
            mainleft->left = left;

            Node *leftCopy = new Node(curNode->left);
            left->left = leftCopy;

            Node *right_left = new Node();
            right_left->type = Node::constant;

            right_left->num = -1;
            mainleft->right = right_left;

            Node *right = new Node();
            d_curNode->right = right;

            derivativeRecursively(right, curNode->left, cout);

            return;
        }
    }
}

bool AST::isConst(Node *pNode, int value)  {
    if (pNode == nullptr) {
        return true;
    }
    return pNode->type == Node::constant && pNode->num == value;
}
void AST::showTree(Node *curNode, std::ofstream &cout) {

    if(curNode == nullptr) {
        return;
    }

    if (curNode->left != nullptr) {
        if (curNode->type == Node::sin || curNode->type == Node::cos) {
            cout << curNode->toString();
        }

        if (curNode->type == Node::div) {
            cout << "\\frac {";
        }

        if ((curNode->left->getPriority() < curNode->getPriority() && curNode->left->getPriority() > 0) ||
            curNode->getPriority() == 3) {
            cout << "( ";
        }
        showTree(curNode->left, cout);
        if ((curNode->left->getPriority() < curNode->getPriority() && curNode->left->getPriority() > 0) ||
            curNode->getPriority() == 3) {
            cout << ") ";
        }
        if (curNode->type == Node::div) {
            cout << "}";
        }

    }

    if (!(curNode->type == Node::sin || curNode->type == Node::cos || curNode->type == Node::div)) {
        cout << curNode->toString();
    }

    if (curNode->right != nullptr) {
        if (curNode->type == Node::div) {
            cout << "{";
        }
        if ((curNode->right->getPriority() < curNode->getPriority() && curNode->right->getPriority() > 0) ||
            curNode->getPriority() == 3) {
            cout << "( ";
        }
        showTree(curNode->right, cout);
        if ((curNode->right->getPriority() < curNode->getPriority() && curNode->right->getPriority() > 0) ||
            curNode->getPriority() == 3) {
            cout << ") ";
        }
        if (curNode->type == Node::div) {
            cout << "}";
        }
    }
}
bool AST::optimize(Node *curNode) {

    if (curNode== nullptr) {
        return false;
    }
    switch (curNode->type) {
        case Node::plus:
        case Node::minus: {
            if (curNode->left == nullptr) {
                curNode->type = Node::constant;
                curNode->num = 0;
                curNode->left = nullptr;
                curNode->right = nullptr;
                return false;
            }
            if (isConst(curNode->left, 0)) {
                delete curNode->left;
                curNode->copy(curNode->right);
                return true;
            } else if (isConst(curNode->right, 0)) {
                delete curNode->right;
                curNode->copy(curNode->left);
                return true;
            } else if (curNode->left->type == Node::constant &&
                       curNode->right->type == Node::constant) {
                int a = curNode->left->num;
                int b = curNode->right->num;
                int c = -1;

                delete curNode->left;
                delete curNode->right;

                switch (curNode->type) {
                    case Node::plus: {
                        c = (a + b);
                        break;
                    }
                    case Node::minus: {
                        c = (a - b);
                        break;
                    }
                }

                curNode->left = nullptr;
                curNode->right = nullptr;

                curNode->type = Node::constant;
                curNode->num = c;
                return true;
            }
            else {
                return optimize(curNode->left) || optimize(curNode->right);
            }


        }
        case Node::mul: {
            if (curNode->left == nullptr) {
                curNode->left = nullptr;
                curNode->right = nullptr;
                curNode->type = Node::constant;
                curNode->num = 0;
                return false;
            }
            if (isConst(curNode->left, 1)) {
                delete curNode->left;
                curNode->copy(curNode->right);
                return true;
            } else if (isConst(curNode->right, 1)) {
                delete curNode->right;
                curNode->copy(curNode->left);
                return true;
            } else if (curNode->left->type == Node::constant &&
                       curNode->right->type == Node::constant) {
                int a = curNode->left->num;
                int b = curNode->right->num;
                int c = a * b;

                delete curNode->left;
                delete curNode->right;


                curNode->left = nullptr;
                curNode->right = nullptr;

                curNode->type = Node::constant;
                curNode->num = c;
                return true;
            } else {
                if (isConst(curNode->left, 0) || isConst(curNode->right, 0)) {
                    delete curNode->left;
                    delete curNode->right;
                    curNode->type = Node::constant;
                    curNode->num = 0;
                    curNode->left = nullptr;
                    curNode->right = nullptr;
                    return true;
                }
                return optimize(curNode->left) || optimize(curNode->right);
            }


        }
        case Node::div: {
            if (curNode->left == nullptr) {
                curNode->left = nullptr;
                curNode->right = nullptr;
                curNode->type = Node::constant;
                curNode->num = 0;
                return false;
            }
            if (isConst(curNode->right, 1)) {
                delete curNode->right;
                curNode->copy(curNode->left);
                return true;
            } else if (curNode->left->type == Node::constant &&
                       curNode->right->type == Node::constant) {
                int a = curNode->left->num;
                int b = curNode->right->num;
                int c = a / b;

                delete curNode->left;
                delete curNode->right;


                curNode->left = nullptr;
                curNode->right = nullptr;

                curNode->type = Node::constant;
                curNode->num = c;
                return true;
            }
            else {
                if (isConst(curNode->left, 0)) {
                    delete curNode->left;
                    delete curNode->right;
                    curNode->type = Node::constant;
                    curNode->num = 0;
                    curNode->left = nullptr;
                    curNode->right = nullptr;
                    return true;
                }
                return optimize(curNode->left) || optimize(curNode->right);
            }
        }

        case Node::sin:
        case Node::cos: {
            return optimize(curNode->left);
        }
        default:
            return false;
    }
}



