//
// Created by greg on 14/11/2018.
//

#ifndef SOFTWARE_ALGORITMS2_HASHTABLEELEMENT_H
#define SOFTWARE_ALGORITMS2_HASHTABLEELEMENT_H

#include "../../Point/Point.h"

class HashTableElement {
    Point* p=nullptr;    //pointer to object point
    vector<int> g={};     //g of point

public:
    HashTableElement(Point&, unsigned int);

    ~HashTableElement()= default;

    Point* GetPoint();
    vector<int> GetG();
    void SetG(int,int);

};


#endif //SOFTWARE_ALGORITMS2_HASHTABLEELEMENT_H
