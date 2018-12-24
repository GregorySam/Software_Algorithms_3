//
// Created by greg on 23/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_TWEET_H
#define SOFTWARE_ALGORITHMS_3_TWEET_H

#include "../../Point/Point.h"

class Tweet{
    double Score=0;
    unsigned int id;
    vector<double> t_v;
    vector<unsigned  int> cc_ref;
public:
    Tweet()=default;
    explicit Tweet(const vector<double>&, unsigned int);
    void SetScore(double);
    void AddCC(unsigned int);
    double GetScore();
    ~Tweet()= default;

};


#endif //SOFTWARE_ALGORITHMS_3_TWEET_H