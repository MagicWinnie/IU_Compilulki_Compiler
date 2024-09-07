#ifndef SPAN_H
#define SPAN_H

#include <string>

class Span {
    long line_num;
    int pos_begin, pos_end;

public:
    Span(long line_num, int pos_begin, int pos_end);

    // to string
    std::string to_string();
};

#endif //SPAN_H
