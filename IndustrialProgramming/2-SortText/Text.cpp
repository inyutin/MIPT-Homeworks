#include "Text.hpp"

// Public:
Text::Text (char* inputFileName) {
    FILE* inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        throw std::runtime_error("Could not open inputFile");
    }

    setFileSize(inputFile);
    setText(inputFile);
    setNumberOfLines();
    setLinesPos();
}
Text::~Text() {
    delete(lines_);
    delete(text_);
}

void Text::writeOriginal(const std::string &outputFile) {
    for (int i = 0; i < fileSize_+1; i++) {
        if (text_[i] == '\0') {
            text_[i] = '\n';
        }
    }

    std::fstream file;
    file.open(outputFile, std::ios::out);
    file << &text_[0];
    file.close();

    for (int i = 0; i < fileSize_+1; i++) {
        if (text_[i] == '\n') {
            text_[i] = '\0';
        }
    }
}

char** Text::begin() {
    return lines_;
}
size_t Text::size() {
    return numberOfLines_+1;
}

// Private:
void Text::setFileSize(FILE* inputFile) {
    fileSize_ = 0;
    fseek(inputFile, 0L, SEEK_END);
    fileSize_ = (size_t) ftell(inputFile);
    rewind(inputFile);
}
void Text::setText(FILE *inputFile) {
    text_ = new char[fileSize_ + 1];
    if (!text_) {
        fclose(inputFile);
        throw std::runtime_error("Memory alloc fails");
    }

    if (fread(text_, fileSize_, 1, inputFile) != 1) {
        fclose(inputFile);
        throw std::runtime_error("entire read fails");
    }
    fclose(inputFile);

}
void Text::setNumberOfLines() {
    numberOfLines_ = 0;
    for (int i = 0; i < fileSize_+1; i++) {
        if (text_[i] == '\n') {
            text_[i] = '\0';
            numberOfLines_ += 1;
        }
    }
}
void Text::setLinesPos() {
    lines_ = new char*[(numberOfLines_+1)* sizeof(char*)];
    if (!lines_) {
        throw std::runtime_error("Memory alloc fails");
    }

    int currentLineStartPos = 0;
    int currentLineNum = 0;
    for (int i = 0; i < fileSize_+1; i++) {
        if (text_[i] == '\0') {
            lines_[currentLineNum]=&text_[currentLineStartPos];
            currentLineNum += 1;
            currentLineStartPos = i+1;
        }
    }
}
