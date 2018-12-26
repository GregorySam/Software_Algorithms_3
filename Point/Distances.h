//
// Created by gregsam on 26/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_DISTANCE_H
#define SOFTWARE_ALGORITHMS_3_DISTANCE_H
#include <vector>
#include <map>

#include "Point.h"
#include "../HashFunctions/Euclidean/Euclidean.h"
#include "../HashFunctions/Cosine/Cosine.h"

using namespace std;

class Distances{

    map<unsigned long int,double> CalculatedDistMap;
    vector<double> CalculatedDist;

    unsigned long int max_size;

public:
    double GetDistance(Point&,Point&,string);
    explicit Distances(unsigned int);
    ~Distances()=default;



};


#endif //SOFTWARE_ALGORITHMS_3_DISTANCE_H
