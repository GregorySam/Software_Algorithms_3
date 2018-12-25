//
// Created by greg on 23/12/2018.
//

#include <cmath>
#include "User.h"



User::User(unsigned int cc_num)
{
    static unsigned int id=0;
    unsigned int i=0;

    user_id=id;
    cc_scores.resize(cc_num,make_pair(nan(""),0));
    for(i=0;i<cc_num;i++)
    {
        cc_scores[i].second=i;
    }

    id++;
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
            if(isnan(cc_scores[cc_index].first))
            {
                cc_scores[cc_index].first=Tweets[i]->GetScore();
                not_nan++;
                continue;
            }
            cc_scores[cc_index].first+=Tweets[i]->GetScore();
        }

    }

    for(i=0;i<cc_scores.size();i++)
    {
        if(isnan(cc_scores[i].first))
        {
            cc_scores[i].first=0;
            NanCC.push_back(&cc_scores[i]);
            continue;
        }
        sum+=cc_scores[i].first;
    }

    mean_value=sum/(double)not_nan;

    for(i=0;i<cc_scores.size();i++)
    {
        cc_scores[i].first=cc_scores[i].first-mean_value;

    }


}