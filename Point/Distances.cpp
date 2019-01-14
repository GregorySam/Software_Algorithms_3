//
// Created by gregsam on 26/12/2018.
//

#include <cmath>
#include "Distances.h"

double Distances::GetDistance(Point& P1,Point& P2,string metric)
{

    double dist=0;



    if (metric == "euclidean") {
        dist = CalculateEuclideanDistance(P1.GetVector(), P2.GetVector());
    } else if (metric == "cosine") {
        dist =  CalculateSimilarity(P1.GetVector(), P2.GetVector());
    }

    return dist;


  }