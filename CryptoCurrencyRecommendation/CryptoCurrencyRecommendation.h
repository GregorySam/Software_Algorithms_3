//
// Created by greg on 21/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
#define SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H

#include <map>
#include "Tweet/Tweet.h"
#include "User/User.h"

class CryptoCurrencyRecommendation {

    vector<User> Users;
    vector<Tweet> Tweets;

    multimap<string,unsigned int> dictionary_cc;
    map<string,double> words_scores;

public:
    CryptoCurrencyRecommendation()=default;

    void AddUser(User);
    void AddTweet(Tweet);
    void Addcc(string,unsigned int);
    void Add_word(string,double);

    void SetTweetScore(vector<string>, unsigned int);
    Tweet* GetTweet(unsigned int);

    ~CryptoCurrencyRecommendation()= default;




};


#endif //SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
