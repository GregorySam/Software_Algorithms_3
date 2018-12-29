//
// Created by gregsam on 26/12/2018.
//

#include <cmath>
#include "Distances.h"


Distances::Distances(unsigned int n)
{

    max_size=(n+n)*(n+n+1)/2+n;


    while(true)                                             //use static sized vector to save distances and use minimum size to fit maximum number of distances,else use dynamic map
    {
        try
        {
            CalculatedDist.resize(max_size,nan(""));
        }
        catch(std::bad_alloc& exc)
        {
            max_size=max_size/2;
            continue;
        }
        break;
    }
}

unsigned long int GetKey(unsigned int a, unsigned int b)            //function from NXN->N
{
    unsigned int tmp;

    if(a>b)
    {
        tmp=b;
        b=a;
        a=tmp;
    }
    return (((a+b)*(a+b+1))/2+b);
}




double Distances::GetDistance(Point& P1,Point& P2,string metric)
{

    double dist=0;
    bool usingmap=false;


    if(P1.GetIndex()>max_size || P2.GetIndex()>max_size)
    {

        if (metric == "euclidean") {
            dist = CalculateEuclideanDistance(P1.GetVector(), P2.GetVector());
        } else if (metric == "cosine") {
            dist =  CalculateSimilarity(P1.GetVector(), P2.GetVector());
        }

        return dist;
    }


    unsigned long int key=GetKey(P1.GetIndex(),P2.GetIndex());





    if(key>=max_size)
    {
        map<unsigned long int,double>::iterator it;
        it=CalculatedDistMap.find(key);
        if(it!=CalculatedDistMap.end())
        {
            return it->second;
        }
        usingmap=true;

    }
    else
    {

        if(!isnan(CalculatedDist[key]))
        {
            return CalculatedDist[key];
        }

    }


    if (metric == "euclidean") {
        dist = CalculateEuclideanDistance(P1.GetVector(), P2.GetVector());
    } else if (metric == "cosine") {
        dist = CalculateSimilarity(P1.GetVector(), P2.GetVector());
    }

    if(usingmap)
    {
        try {
            CalculatedDistMap.insert(make_pair(key,dist));
        }
        catch(std::bad_alloc& exc){

        }


    }
    else if(key<max_size)
    {
        CalculatedDist[key]=dist;
    }
    return dist;
}