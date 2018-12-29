//
// Created by greg on 23/12/2018.
//

#include <cmath>
#include "User.h"



unsigned  int User::GetUserId()
{
    return user_id;
}


set<string> User::GetCCRes()
{
    return OutResultsNanCC;
}

void User::AddCCResults(vector<string> cc_names,bool Beta)
{
    unsigned int i;

    if(Beta)
    {
        BetaResults=cc_names;
    }
    for(i=0;i<cc_names.size();i++)
   {
       OutResultsNanCC.insert(cc_names[i]);
   }
}

vector<pair<double,unsigned int>>& User::GetNanScores()
{
    return NanCC;
}

User::User(unsigned int cc_num)
{
    static unsigned int id=0;


    user_id=id;
    id++;
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

void User::SetCCScores()
{
    unsigned int i,j,cc_index,not_nan=0;
    double sum=0,mean_value;

    vector<unsigned int>* cc_refs;

    for(i=0;i<Tweets.size();i++)
    {
        cc_refs=&Tweets[i]->GetCC();
        for(j=0;j<cc_refs->size();j++)
        {
            cc_index=cc_refs->at(j);
            if(isnan(cc_scores[cc_index]))
            {
                cc_scores[cc_index]=Tweets[i]->GetScore();
                continue;
            }
            cc_scores[cc_index]+=Tweets[i]->GetScore();
            not_nan++;
        }

    }

    for(i=0;i<cc_scores.size();i++)
    {
        if(isnan(cc_scores[i]))
        {
            cc_scores[i]=0;
            NanCC.push_back(make_pair(cc_scores[i],i));
            continue;
        }
        sum+=cc_scores[i];
    }
    if(not_nan==0)
    {
        not_nan=1;
    }

    mean_value=sum/(double)not_nan;

    for(i=0;i<cc_scores.size();i++)
    {
        cc_scores[i]=cc_scores[i]-mean_value;

    }


}