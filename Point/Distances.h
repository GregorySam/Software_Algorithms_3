//
// Created by gregsam on 26/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_DISTANCE_H
#define SOFTWARE_ALGORITHMS_3_DISTANCE_H
#include "Point.h"
#include "../HashFunctions/Euclidean/Euclidean.h"
#include "../HashFunctions/Cosine/Cosine.h"

inline double GetDistance(Point& P1,Point& P2,const string& metric)
{

    double dist=0;

    if (metric == "euclidean") {
        dist = CalculateEuclideanDistance(P1.GetVector(), P2.GetVector());
    } else if (metric == "cosine") {
        dist =  CalculateSimilarity(P1.GetVector(), P2.GetVector());
    }

    return dist;


}


#endif //SOFTWARE_ALGORITHMS_3_DISTANCE_H
