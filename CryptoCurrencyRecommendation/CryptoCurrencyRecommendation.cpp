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

void CryptoCurrencyRecommendation::AddUser(User* U)
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


void CryptoCurrencyRecommendation::SetNanScores(set<pair<double,unsigned int>,CompFun> &NN, unsigned int max_n,User& U,vector<Point>& Points)
{
    set<pair<double,unsigned int>,CompFun>::iterator it;
    unsigned int j,i,nan_scoreindex;

    vector<pair<double*,unsigned int>>* nan_scores;
    double z=0,sum=0;

    nan_scores=&U.GetNanScores();

    it=NN.begin();


    for(j=0;j<nan_scores->size();j++)
    {
        nan_scoreindex=nan_scores->at(j).second;
        for(i=0;i<max_n;i++)
        {
            z=z+abs(D->GetDistance(Points[it->second],Points[U.GetUserId()],"cosine"));
            sum=sum+D->GetDistance(Points[it->second],Points[U.GetUserId()],"cosine")*Points[it->second].GetVector().at(nan_scoreindex);
            it++;
        }
        *nan_scores->at(j).first=(1/z)*sum;

    }


}


User* CryptoCurrencyRecommendation::GetUser(unsigned int id)
{
    return Users[id];
}

void CryptoCurrencyRecommendation::CosineLSHSearch(unsigned int P)
{
    unsigned int i;

    HashManagementLSH HM_LSH((unsigned int)cc_num,7,5,(unsigned int)Users.size(),"cosine");

    vector<Point> Points(Users.size());
    vector<double>* p_scores;
    vector<double> zeros(cc_num,0);

    set<pair<double,unsigned int>,CompFun> NN;

    for(i=0;i<Users.size();i++)
    {
        p_scores=&Users[i]->GetCC_Scores();

        Point Po(*p_scores,"",i);
        Points[i]=Po;
    }

    HM_LSH.InsertInHashTables(Points);

    for(i=0;i<Users.size();i++)
    {
        if(Points[i].GetVector()==zeros)
        {continue;}
        HM_LSH.SearchNNPoint(Points[i],NN,P,*D);
        SetNanScores(NN,P,*Users[i],Points);
        NN.clear();
    }
    //for evry user


}

void CryptoCurrencyRecommendation::SetCCScores()
{
    unsigned int i;

    for(i=0;i<Users.size();i++)
    {
        Users[i]->SetCCScores();
    }


}


void CryptoCurrencyRecommendation::SetCCNum(unsigned int n)
{
    cc_num=n;
}

void CryptoCurrencyRecommendation::SetUsersNum(unsigned int n)
{
    users_num=n;
    D=new Distances(users_num);
}

void CryptoCurrencyRecommendation::SetTweetsNum(unsigned int n)
{
    tweets_num=n;
}

void CryptoCurrencyRecommendation::Addcc_name(string name)
{
    CC_names.push_back(name);
}


void CryptoCurrencyRecommendation::Addcc_key(string key, unsigned int value)
{

    dictionary_cc.insert(pair<string,unsigned int>(key,value));
}

void CryptoCurrencyRecommendation::Add_word(string word, double score)
{
    words_scores.insert(pair<string,double>(word,score));
}

