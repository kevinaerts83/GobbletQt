#ifndef AIMOVE_H
#define AIMOVE_H

class aiMove
{
public:
    aiMove();

    int from() const;
    void setFrom(int value);

    int to() const;
    void setTo(int value);

private:
    // tiles 0 till 15, 16 is size 0 of stack -> 19 is size 4 of stack
    int m_from;
    int m_to;
};

#endif // AIMOVE_H
