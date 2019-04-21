#pragma once
#include <fstream>
#include <vector>
#include <Text.hpp>
#include <string>
#include <map>
#include <iostream>

namespace {
    std::map<std::string, std::string> latexConst = {
            {"begin", "\\documentclass[a4paper, 20pt]{article}\n"
                      "\\usepackage[utf8]{inputenc}\n"
                      "\n"
                      "\\usepackage[square,numbers]{natbib}\n"
                      "\\bibliographystyle{unsrtnat}\n"
                      "\n"
                      "\\title{Derivative}\n"
                      "\n"
                      "\\begin{document}\n"
                      "\n"
                      "\\maketitle\n"
                      "\n"
                      "\n"
                      "\n"},
            {"end", "\n"
                    "\\medskip\n"
                    "\n"
                    "\n"
                    "\\end{document}"},
            {"empty", " \\\\ "},
            {"newline", "\\newline\n"},
            {"skip", "\\bigbreak\n"}
    };
}

class AST {
public:
    explicit AST(const std::string &line);
    ~AST();
    AST derivative(const std::string &file_name = "derrivative", int degree=1);

private:
    struct Node {
        int num;
        enum Type { plus, minus, mul,
                    div, sin, cos,
                    constant, variable} type;

        Node *left = nullptr;
        Node *right = nullptr;

        std::string toString();

        Node();

        explicit Node(Node *pNode);

        explicit Node(Type type);

        void setType(char c);

        void copy(Node *pNode);

        int getPriority();
    };

    Node *head;

    explicit AST(Node* tmp) {
        this->head = tmp;
    }

    void recursivelyBuild(std::vector<int>::iterator &it, std::vector<int>::iterator end, Node *node);

    void derivativeRecursively(Node *d_curNode, Node *curNode, std::ofstream &cout);

    bool isConst(Node *pNode, int value);

    void showTree(Node *curNode, std::ofstream &cout);

    bool optimize(Node *curNode);

    void deleteNode(Node *node) {
        if (node->left != nullptr) {
            deleteNode(node->left);
        }
        if (node->right != nullptr) {
            deleteNode(node->right);
        }
        delete node;
    }
};