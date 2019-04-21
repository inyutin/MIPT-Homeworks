#pragma once

#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <cctype>

class Text
{
public:
    Text() = delete;

    Text(const char* file_path, std::vector<char*>& text_lines) {

        file_descriptor_ = open(file_path, O_RDWR);

        if (file_descriptor_ == -1) {
            std::cerr << "Error while opening file" << std::endl;
            exit(EXIT_FAILURE);
        }

        file_size_ = GetFileLength(file_descriptor_);

        mapped_memory_ = static_cast<char*>(mmap(nullptr, file_size_ + 1, PROT_READ | PROT_WRITE,
                                                 MAP_SHARED, file_descriptor_, 0));
        close(file_descriptor_);

        if (mapped_memory_ == MAP_FAILED) {
            std::cerr <<  "while memory maping the file" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < file_size_; i++) {
            if (mapped_memory_[i] == '\n' || mapped_memory_[i] == '\0')
            {
                mapped_memory_[i] = '\0';
                ++lines_quantity_;
            }
        }
        mapped_memory_[file_size_] = '\0';

        size_t lines_iter = 1;

        text_lines.assign(lines_quantity_, nullptr);
        text_lines[0] = mapped_memory_;

        for (size_t i = 0; i < file_size_; i++) {
            if (mapped_memory_[i] == '\0')
            {
                text_lines[lines_iter] = mapped_memory_ + i + 1;
                lines_iter++;
            }
        }
    }

    ~Text() {
        for (size_t i = 0; i < file_size_; i++) {
            if (mapped_memory_[i] == '\0') {
                mapped_memory_[i] = '\n';
            }
        }

        if (munmap(mapped_memory_, file_size_) == -1) {
            std::cerr << "while un-memory mapping the file" << std::endl;
        }
        close(file_descriptor_);
    }

private:
    size_t GetFileLength(int file_descriptor) {
        struct stat stat_buff{};
        if (fstat(file_descriptor_, &stat_buff) < 0)
        {
            close(file_descriptor_);
        }
        return static_cast<size_t>(stat_buff.st_size);
    }

    size_t lines_quantity_ = 1;
    size_t file_size_ = 0;
    char* mapped_memory_ = nullptr;
    int file_descriptor_ = 0;
};