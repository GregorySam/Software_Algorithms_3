//
// Created by greg on 21/12/2018.
//

#include <fstream>
#include "CryptoCurrencyRecommendation.h"



void CryptoCurrencyRecommendation::AddTweet(Tweet T)
{
    Tweets.push_back(T);
}

void CryptoCurrencyRecommendation::AddUser(User U)
{
    Users.push_back(U);
}

void CryptoCurrencyRecommendation::Addcc(string key, unsigned int value)
{
    dictionary_cc.insert(pair<string,unsigned int>(key,value));
}

void CryptoCurrencyRecommendation::Add_word(string word, double score)
{
    words_scores.insert(pair<string,double>(word,score));
}