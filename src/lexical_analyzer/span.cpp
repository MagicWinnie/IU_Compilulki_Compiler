#include "span.h"

Span::Span(const long line_num, const int pos_begin, const int pos_end) {
    this->line_num = line_num;
    this->pos_begin = pos_begin;
    this->pos_end = pos_end;
}

std::string Span::to_string() {
    return "Span(" + std::to_string(line_num) + ", " + std::to_string(pos_begin) + ", " + std::to_string(pos_end) + ")";
}