//
// Created by greg on 23/12/2018.
//

#include <cmath>
#include "User.h"



User::User(unsigned int cc_num)
{
    static unsigned int id=0;

    user_id=id;
    cc_scores.resize(cc_num,nan(""));

    id++;
}

void User::AddTweet(Tweet* T)
{
    Tweets.push_back(T);
}