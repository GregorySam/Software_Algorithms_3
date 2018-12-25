//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_EUCLIDEAN_H_FUNCTION_H
#define SOFWARE_ALGORITHMS_1_EUCLIDEAN_H_FUNCTION_H

#include <vector>
#include <cmath>
#include "../HashFunction.h"

using namespace std;

class Euclidean_h:public HashFunction {
    double t;
    vector <double> v;
    int w;

public:
    explicit Euclidean_h(unsigned int);
    ~Euclidean_h() override = default;

     int CalculateValue(vector<double>) override;
    vector<double> Get_Parameters() override;

};

double CalculateEuclideanDistance(vector<double> a,vector<double> b);


#endif //SOFWARE_ALGORITHMS_1_EUCLIDEAN_H_FUNCTION_H
