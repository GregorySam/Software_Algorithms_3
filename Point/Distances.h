//
// Created by gregsam on 15/1/2019.
//

#ifndef SOFTWARE_ALGORITHMS_3_DISTANCES_H
#define SOFTWARE_ALGORITHMS_3_DISTANCES_H

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


inline double GetDistance(Point &Pa, Point &Pb, const string& metric)
{

    double dist=0;



    if (metric == "euclidean") {
        dist = CalculateEuclideanDistance(Pa.GetVector(), Pb.GetVector());
    } else if (metric == "cosine") {
        dist = CalculateSimilarity(Pa.GetVector(), Pb.GetVector());
    }
    return dist;
}


#endif //SOFTWARE_ALGORITHMS_3_DISTANCES_H
