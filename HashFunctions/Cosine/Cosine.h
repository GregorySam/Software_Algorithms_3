//
// Created by greg on 14/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_COSINE_H
#define SOFWARE_ALGORITHMS_1_COSINE_H

#include "../HashFunction.h"
#include <numeric>
#include <cmath>

class Cosine: public HashFunction {
    vector<double> R;        //r_i
public:
    explicit  Cosine(unsigned int);
    ~Cosine() override = default;;

    int CalculateValue(vector<double>) override;
    vector<double> Get_Parameters() override;
};

double CalculateCosine(vector<double> a,vector<double> b);
double CalculateSimilarity(vector<double> a,vector<double> b);


#endif //SOFWARE_ALGORITHMS_1_COSINE_H
