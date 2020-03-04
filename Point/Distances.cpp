//
// Created by gregsam on 15/1/2019.
//

#include "Distances.h"






Distances::Distances(unsigned int size)
{







    size=(size+size)*(size+size+1)/2+size;

    while(true)                                             //use static sized vector to save distances and use minimum size to fit maximum number of distances,else use dynamic map
    {
        try
        {
            CalculatedDist.resize(size,nan(""));
            max_size=size;
        }
        catch(std::bad_alloc& exc)
        {
            size=size/2;
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


double Distances::GetDistance(Point &Pa, Point &Pb, string metric)
{

    double dist=0;
    bool usingmap=false;



    unsigned long int key=GetKey(Pa.GetIndex(),Pb.GetIndex());

    if(Pa.GetIndex()>max_size || Pb.GetIndex()>max_size)
    {

        if (metric == "euclidean") {
            dist = CalculateEuclideanDistance(Pa.GetVector(), Pb.GetVector());
        } else if (metric == "cosine") {
            dist = CalculateSimilarity(Pa.GetVector(), Pb.GetVector());
        }
        return dist;
    }

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
        dist = CalculateEuclideanDistance(Pa.GetVector(), Pb.GetVector());
    } else if (metric == "cosine") {
        dist =  CalculateSimilarity(Pa.GetVector(), Pb.GetVector());
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