//
// Created by greg on 23/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_USER_H
#define SOFTWARE_ALGORITHMS_3_USER_H

#include <set>
#include "../Tweet/Tweet.h"

class User {
    unsigned  int user_id;
    vector<double> cc_scores;
    vector<pair<double,unsigned int>> NanCC;
    vector<unsigned int> KnownCC;
    set<string> OutResultsNanCC;
    string name;

    vector<Tweet*> Tweets;
public:

    User()= default;

    explicit  User(unsigned int,string);
    vector<double>& GetCC_Scores();
    vector<pair<double,unsigned int>>& GetNanScores();
    string GetName();
    vector<unsigned int> GetKnownCC();

    void AddTweet(Tweet*);
    unsigned  int GetUserId();
    void SetCCScores();
    void SetMeanValues();
    void AddCCResults(vector<string>);
    void ResetResults();
    void SetCCScore(unsigned int,double);


    set<string> GetCCRes();
    ~User()= default;
};


#endif //SOFTWARE_ALGORITHMS_3_USER_H
