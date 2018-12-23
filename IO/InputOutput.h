//
// Created by greg on 13/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_INPUTOUTPUT_H
#define SOFWARE_ALGORITHMS_1_INPUTOUTPUT_H

#include <string>
#include "../CryptoCurrencyRecommendation/CryptoCurrencyRecommendation.h"



using namespace std;
class InputOutput {

public:
    string tweets_input_file="";
    string vector_tweets_input_file="";
    string output_file="";
    string ccDict="";
    string vaderDict="";

    bool Validate;

    unsigned int users_number;
    unsigned int tweets_number;
    unsigned int cc_number;
    unsigned int dimensions;
    unsigned int P;

    InputOutput(int,char**);
    ~InputOutput()= default;

    void ReadFiles(CryptoCurrencyRecommendation&);

};


#endif //SOFWARE_ALGORITHMS_1_INPUTOUTPUT_H
