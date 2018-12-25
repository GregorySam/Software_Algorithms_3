//
// Created by greg on 12/10/2018.
//

#include "Euclidean.h"

#include <random>
#include <numeric>

Euclidean_h::Euclidean_h(unsigned int dim)
{
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());

    w=4;
    uniform_real_distribution<double> u_distribution(0.0,w);       //generate t

    t=u_distribution(gen);
                                                            //generate v
    normal_distribution<double> n_distribution(0,1.0);
    v.resize(dim);


    for(unsigned int i=0;i<dim;i++)
    {
        v[i]=n_distribution(gen);
    }

}


int Euclidean_h::CalculateValue(vector<double> d)
{

    return (int)(floor((inner_product(begin(v), end(v), begin(d), 0.0)+t)/w));              //calculate h(p)
}


vector<double> Euclidean_h::Get_Parameters()
{
    vector<double> d;

    d.push_back(t);
    d.insert(d.end(),v.begin(),v.end());

    return d;
}

double CalculateEuclideanDistance(vector<double> a, vector<double> b) {
    unsigned int i;
    double dist=0;

    for(i=0;i<a.size();i++)
    {
        dist=dist+(a[i]-b[i])*(a[i]-b[i]);
    }

    return sqrt(dist);
}
