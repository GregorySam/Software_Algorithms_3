//
// Created by greg on 23/12/2018.
//

#include "Tweet.h"



Tweet::Tweet(const vector<double>& vd, unsigned int id):id(id),t_v(vd)
{}

void Tweet::AddCC(unsigned int cc_id)
{
    cc_ref.insert(cc_id);
}

void Tweet::SetScore(double newscore)
{
    Score=newscore;
}

double Tweet::GetScore()
{
    return Score;
}

set<unsigned  int>& Tweet::GetCC()
{
    return cc_ref;
}

vector<double> Tweet::GetVector()
{
    return t_v;
}