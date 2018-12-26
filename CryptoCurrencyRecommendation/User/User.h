//
// Created by greg on 23/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_USER_H
#define SOFTWARE_ALGORITHMS_3_USER_H

#include "../Tweet/Tweet.h"

class User {
    unsigned  int user_id;
    vector<double> cc_scores;
    vector<pair<double*,unsigned int>> NanCC;

    vector<Tweet*> Tweets;
public:
    explicit  User(unsigned int);
    vector<double>& GetCC_Scores();
    vector<pair<double*,unsigned int>>& GetNanScores();

    void AddTweet(Tweet*);
    unsigned  int GetUserId();
    void SetCCScores();

    ~User()= default;
};


#endif //SOFTWARE_ALGORITHMS_3_USER_H
