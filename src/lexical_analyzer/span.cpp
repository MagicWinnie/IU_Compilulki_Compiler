#include "span.h"

Span::Span(const size_t line_num, const size_t pos_begin, const size_t pos_end) {
    this->line_num = line_num;
    this->pos_begin = pos_begin;
    this->pos_end = pos_end;
}

std::string Span::to_string() const {
    return "Span(" + std::to_string(line_num) + ", " + std::to_string(pos_begin) + ", " + std::to_string(pos_end) + ")";
}

int Span::get_line_num() const {
    return line_num;
}

int Span::get_pos_begin() const {
    return pos_begin;
}

int Span::get_pos_end() const {
    return pos_end;
}