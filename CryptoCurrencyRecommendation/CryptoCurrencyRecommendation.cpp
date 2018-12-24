//
// Created by greg on 21/12/2018.
//

#include <fstream>
#include <cmath>
#include "CryptoCurrencyRecommendation.h"



void CryptoCurrencyRecommendation::AddTweet(Tweet T)
{
    Tweets.push_back(T);
}

void CryptoCurrencyRecommendation::AddUser(User U)
{
    Users.push_back(U);
}

Tweet* CryptoCurrencyRecommendation::GetTweet(unsigned int id)
{
    return &Tweets.at(id-1);
}

void CryptoCurrencyRecommendation::SetTweetScore(vector<string> s, unsigned int tweetid)
{
    unsigned int i;
    double score=0;

    multimap<string,unsigned int>::iterator it_cc;
    map<string,double>::iterator it;

    for(i=0;i<s.size();i++)
    {
        it=words_scores.find(s[i]);
        if(it==words_scores.end())
        {
            it_cc=dictionary_cc.find(s[i]);
            if(it_cc!=dictionary_cc.end())
            {
                Tweets[tweetid-1].AddCC(it_cc->second);
            }
            continue;
        }
        score=score+it->second;
    }
    score=score/sqrt(score*score+15);
    Tweets[tweetid-1].SetScore(score);
}

void CryptoCurrencyRecommendation::Addcc(string key, unsigned int value)
{
    dictionary_cc.insert(pair<string,unsigned int>(key,value));
}

void CryptoCurrencyRecommendation::Add_word(string word, double score)
{
    words_scores.insert(pair<string,double>(word,score));
}