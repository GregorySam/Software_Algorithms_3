//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_HASHFUNCTION_H
#define SOFWARE_ALGORITHMS_1_HASHFUNCTION_H

#include <vector>
using namespace std;

class HashFunction {
public:
    HashFunction()= default;
    virtual ~HashFunction()= default;

    virtual int CalculateValue(vector<double>)=0;
    virtual vector<double> Get_Parameters()=0;
    bool operator==(HashFunction& H)
    {
        return H.Get_Parameters()==this->Get_Parameters();
    }
};


#endif //SOFWARE_ALGORITHMS_1_HASHFUNCTION_H
