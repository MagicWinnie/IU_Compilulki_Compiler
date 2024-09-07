#ifndef SPAN_H
#define SPAN_H

class Span {
    long line_num;
    int pos_begin, pos_end;

public:
    Span(long line_num, int pos_begin, int pos_end);
};

#endif //SPAN_H
