//
// Created by greg on 14/10/2018.
//

#include "Cosine.h"
#include <random>
#include <numeric>

Cosine::Cosine(unsigned int dim)
{
    random_device rd;                                       //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());

    normal_distribution<double> n_distribution(0,1.0);
    R.resize(dim);

    for(unsigned int i=0;i<dim;i++)
    {
        R[i]=n_distribution(gen);                           //generate r_is for inner product
    }
}

int Cosine::CalculateValue(vector<double> p)
{
    return (inner_product(begin(p), end(p), begin(R), 0.0)>=0);     //Retrun 0 or 1
}

vector<double> Cosine::Get_Parameters()
{
    return R;
}

double CalculateCosine(vector<double> a, vector<double> b) {
    double numerator,denominator,norma,normb;

    numerator=inner_product(begin(a), end(a), begin(b), 0.0);

    norma=sqrt(inner_product(begin(a), end(a), begin(a), 0.0));
    normb=sqrt(inner_product(begin(b), end(b), begin(b), 0.0));
    denominator=norma*normb;

    return 1-(numerator/denominator);
}


double CalculateSimilarity(vector<double> a,vector<double> b)
{
    double numerator,denominator,norma,normb;

    numerator=inner_product(begin(a), end(a), begin(b), 0.0);

    norma=sqrt(inner_product(begin(a), end(a), begin(a), 0.0));
    normb=sqrt(inner_product(begin(b), end(b), begin(b), 0.0));
    denominator=norma*normb;

    return (numerator/denominator);
}