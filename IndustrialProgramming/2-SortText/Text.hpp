#ifndef TEXT_HPP
#define TEXT_HPP

#include <iostream>
#include <fstream>
class Text {
public:
    explicit Text (char* inputFileName);
    ~Text();

    void writeOriginal(const std::string &outputFile);

    char** begin();
    size_t size();
private:
    size_t fileSize_;
    char* text_;

    size_t numberOfLines_;
    char** lines_;

    void setFileSize(FILE* inputFile);
    void setText(FILE *inputFile);
    void setNumberOfLines();
    void setLinesPos();
};

#endif
