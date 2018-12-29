//
// Created by greg on 12/10/2018.
//

#include "Point.h"
#include <utility>
#include <iostream>

static unsigned int id=0;

Point::Point(const vector<double>& dv,string n):v(dv),name(n)
{

    index=id;

    group_flag=-1;
    id++;

}

void Point::Reset()
{
    id=0;
}

vector <double> Point::GetVector()
{
    return v;
}


void Point::SetId(unsigned int i)
{
    index=i;
}

string Point::GetName()
{
    return name;
}

void Point::SetVector(vector<double> newv)
{
    v= std::move(newv);
}

void Point::SetGroupFlag(int gf)
{
    group_flag=gf;
    group_flags.clear();

}

int Point::GetGroupFlag()
{
    return group_flag;
}

void Point::Addgroupflag(int flag)
{
    group_flags.insert(flag);
}

set<int> Point::GetGroups()
{
    return group_flags;
}

unsigned int Point::GetIndex()
{
    return index;
}
