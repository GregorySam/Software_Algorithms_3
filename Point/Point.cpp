//
// Created by greg on 12/10/2018.
//

#include "Point.h"
#include <utility>
#include <iostream>


Point::Point(const vector<double>& dv,string n,unsigned int id):v(dv),name(n)
{

    index=id;

    group_flag=-1;

}


vector <double> Point::GetVector()
{
    return v;
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
    group_flags.push_back(flag);
}

vector<int> Point::GetGroups()
{
    return group_flags;
}

unsigned int Point::GetIndex()
{
    return index;
}
