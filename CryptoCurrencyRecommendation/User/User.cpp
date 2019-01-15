//
// Created by greg on 23/12/2018.
//

#include <cmath>
#include <utility>
#include "User.h"



unsigned  int User::GetUserId()
{
    return user_id;
}


set<string> User::GetCCRes()
{

    return OutResultsNanCC;
}



void User::SetCCScore(unsigned int id, double new_score)
{
    cc_scores[id]=new_score;

}


void User::ResetResults()
{
    OutResultsNanCC.clear();
}


void User::AddCCResults(vector<string> cc_names)
{
    unsigned int i;

    for(i=0;i<cc_names.size();i++)
   {
       OutResultsNanCC.insert(cc_names[i]);
   }
}

string User::GetName()
{
    return name;
}

vector<pair<double,unsigned int>>& User::GetNanScores()
{
    return NanCC;
}

User::User(unsigned int cc_num,string n)
{
    static unsigned int id=0;


    user_id=id;
    id++;
    name= std::move(n);
    cc_scores.resize(cc_num,nan(""));


}





vector<double>& User::GetCC_Scores()
{
    return cc_scores;
}

void User::AddTweet(Tweet* T)
{
    Tweets.push_back(T);
}

vector<unsigned int> User::GetKnownCC()
{
    return KnownCC;
}


void User::SetMeanValues()
{
    double sum=0,mean_value;
    unsigned int i,not_nan=0;

    KnownCC.clear();
    NanCC.clear();

    for(i=0;i<cc_scores.size();i++)
    {
        if(isnan(cc_scores[i]))
        {
            cc_scores[i]=0;
            NanCC.emplace_back(cc_scores[i],i);
            continue;
        }
        KnownCC.push_back(i);
        sum+=cc_scores[i];
        not_nan++;
    }
    if(KnownCC.empty())
    {
        not_nan=1;
    }

    mean_value=sum/(double)not_nan;

    for(i=0;i<cc_scores.size();i++)
    {
        cc_scores[i]=cc_scores[i]-mean_value;

    }


}

void User::SetCCScores()
{
    unsigned int i,cc_index;

    set<unsigned int>* cc_refs;
    set<unsigned int>::iterator it;

    for(i=0;i<Tweets.size();i++)
    {
        cc_refs=&Tweets[i]->GetCC();
        for(it=cc_refs->begin();it!=cc_refs->end();it++)
        {
            cc_index=*it;
            if(isnan(cc_scores[cc_index]))
            {
                cc_scores[cc_index]=Tweets[i]->GetScore();
                continue;
            }
            cc_scores[cc_index]+=Tweets[i]->GetScore();

        }

    }
    SetMeanValues();

}