#include "field.h"
#include <QDebug>

Field::Field()
{
    field.fill(CL_CLEAR, 225);
}

void Field::setField(int x, int y, Cell value)
{
    const int i = x+y*15;
    field[i] = value;
}

Cell Field::getField(int x, int y)
{
    const int i = x+y*15;
    return field.at(i);
}

Cell Field::isWin()
{
    int countX,countO,i,j;
    for (i=0;i<15;i++){
        countX=0;countO=0;
        for (j=0;j<15;j++){
            if (getField(i, j) == CL_X)
                countX++;else countX=0;
            if (getField(i, j) == CL_O)
                countO++;else countO=0;
            if (countX>=5) return CL_X;
            else if (countO>=5) return CL_O;}}

        for (j=0;j<15;j++){
        countX=0;countO=0;
        for (i=0;i<15;i++){
            if (getField(i, j)== CL_X)
                countX++;else countX=0;
            if (getField(i, j) == CL_O)
                countO++;else countO=0;
        if (countX>=5) return CL_X;
        else if (countO>=5) return CL_O;}}

        countX=0;countO=0;
        for (i=0;i<15;i++){
        if (getField(i, i) == CL_X)
            countX++;else countX=0;
        if (getField(i, i) == CL_O)
            countO++;else countO=0;
        if (countX>=5) return CL_X;
        else if (countO>=5) return CL_O;}

    countX=0;countO=0;
    for (i=0;i<15;i++){
        if (getField(i, 14-i)== CL_X)
            countX++;else countX=0;
        if (getField(i, 14-i)== CL_O)
            countO++;else countO=0;
        if (countX>=5) return CL_X;
        else if (countO>=5) return CL_O;}

    Cell r = checkDiagonal();
    qDebug() << r;
    if (r == CL_X) return CL_X;
    if (r == CL_O) return CL_O;
    return CL_CLEAR;
}

Cell Field::checkDiagonal()
{
    int countX = 0;
    int countO = 0;
    int k=0;
    for (int j=0;j<16;j++){
        k=0;
        countX = 0; countO = 0;
        for (int i=0;i<16;i++)
        {
            if (i+j >= 15) break;
                if (getField(k, i+j) == CL_X){
                    countX++;
                    qDebug() << k << i+j << countX << countO;
                }else countX=0;
                if (getField(k, i+j) == CL_O)
                    countO++;else countO=0;
                k++;
                if (countX>=5) return CL_X;
                if (countO>=5) return CL_O;
        }
    }
    for (int j=0;j<16;j++){
        k=0;
        countX = 0; countO = 0;
        for (int i=0;i<16;i++)
        {
            if (i+j >= 15) break;
                if (getField(i+j, k) == CL_X)
                    countX++;else countX=0;
                if (getField(i+j, k) == CL_O)
                    countO++;else countO=0;
                k++;
                if (countX>=5) return CL_X;
                else if (countO>=5) return CL_O;
        }
    }

    for (int j=1;j<16;j++){
        k=14;
        countX = 0; countO = 0;
        for (int i=0;i<16;i++)
        {
            if (i+j >= 15) break;
                if (getField(k, i+j) == CL_X)
                    countX++;else countX=0;
                if (getField(k, i+j) == CL_O)
                    countO++;else countO=0;
                k--;
                if (countX>=5) return CL_X;
                if (countO>=5) return CL_O;
        }
    }

    for (int j=1;j<16;j++){
        k=1;
        countX = 0; countO = 0;
        for (int i=14;i>1;i--)
        {
            if (i-j < 0) break;
                if (getField(k, i-j) == CL_X)
                    countX++;else countX=0;
                if (getField(k, i-j) == CL_O)
                    countO++;else countO=0;
                k++;
                if (countX>=5) return CL_X;
                if (countO>=5) return CL_O;
        }
    }
    return CL_CLEAR;
}
