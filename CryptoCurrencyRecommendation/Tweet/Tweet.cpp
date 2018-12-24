//
// Created by greg on 23/12/2018.
//

#include "Tweet.h"



Tweet::Tweet(const vector<double>& vd, unsigned int id):t_v(vd),id(id)
{}

void Tweet::AddCC(unsigned int cc_id)
{
    cc_ref.push_back(cc_id);
}

void Tweet::SetScore(double newscore)
{
    Score=newscore;
}

double Tweet::GetScore()
{
    return Score;
}