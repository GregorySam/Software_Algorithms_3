//
// Created by greg on 21/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
#define SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H

#include <map>

#include "../Point/Distances.h"
#include "../HashManagementLSH/HashManagementLSH.h"
#include "Tweet/Tweet.h"
#include "User/User.h"

class CryptoCurrencyRecommendation {

    vector<User*> Users;
    vector<Tweet> Tweets;

    Distances *D=nullptr;

    unsigned int users_num=0;
    unsigned int tweets_num=0;
    unsigned int cc_num=0;



    multimap<string,unsigned int> dictionary_cc;
    vector<string> CC_names;
    map<string,double> words_scores;

public:
    CryptoCurrencyRecommendation()=default;

    void SetUsersNum(unsigned int);
    void SetTweetsNum(unsigned int);
    void SetCCNum(unsigned int);

    void AddUser(User*);
    void AddTweet(Tweet);
    void Addcc_key(string,unsigned int);
    void Addcc_name(string);
    void Add_word(string,double);

    void SetTweetScore(vector<string>, unsigned int);
    void SetCCScores();
    void SetNanScores(set<pair<double,unsigned int>,CompFun>&, unsigned int,User&,vector<Point>&);
    Tweet* GetTweet(unsigned int);
    User* GetUser(unsigned int);

    void CosineLSHSearch(unsigned int);

    ~CryptoCurrencyRecommendation()= default;




};


#endif //SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
