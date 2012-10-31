#ifndef FIELD_H
#define FIELD_H

#include <QVector>

enum Cell
{
    CL_CLEAR=0,
    CL_X,
    CL_O
};

class Field
{
public:
    Field();
    void setField(int x, int y, Cell value);
    Cell getField(int x, int y);
    Cell isWin();
private:
    QVector<Cell> field;
    Cell checkDiagonal();
};

#endif // FIELD_H
