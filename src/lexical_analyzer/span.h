#pragma once

#include <string>

class Span {
    size_t line_num;
    size_t pos_begin, pos_end;

public:
    Span(size_t line_num, size_t pos_begin, size_t pos_end);

    // to string
    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] int get_line_num() const;
    [[nodiscard]] int get_pos_begin() const;
    [[nodiscard]] int get_pos_end() const;
};
