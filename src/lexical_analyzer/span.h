#ifndef SPAN_H
#define SPAN_H

#include <string>

class Span {
    size_t line_num;
    size_t pos_begin, pos_end;

public:
    Span(size_t line_num, size_t pos_begin, size_t pos_end);

    // to string
    std::string to_string() const;
};

#endif //SPAN_H
