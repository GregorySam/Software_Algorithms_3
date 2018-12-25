//
// Created by greg on 14/11/2018.
//

#include "HashTableElement.h"



HashTableElement::HashTableElement(Point& P,unsigned int num_h)
{
    p=&P;
    g.resize(num_h,0);
}


vector<int> HashTableElement::GetG()
{
    return g;
}

void HashTableElement::SetG(int index, int value)
{
    g[index]=value;
}

Point* HashTableElement::GetPoint()
{
    return p;
}