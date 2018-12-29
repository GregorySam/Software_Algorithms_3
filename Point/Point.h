//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_POINT_H
#define SOFWARE_ALGORITHMS_1_POINT_H

#include <vector>
#include <string>
#include <set>

using namespace std;
class Point {
    vector<double> v;
    string name;
    set<int> group_flags;       //all possible clusters that the point can be assigned
    unsigned int index{};
    int group_flag{};               //the flag of cluster the point belongs to




public:
    Point(const vector<double>&,string);
    static void Reset();
    Point()= default;
    ~Point()= default;


    vector<double> GetVector();
    string GetName();


    unsigned int GetIndex();
    void SetId(unsigned int);
    void SetGroupFlag(int);
    int GetGroupFlag();
    void Addgroupflag(int);
    set<int> GetGroups();
    void SetVector(vector<double>);



};


#endif //SOFWARE_ALGORITHMS_1_POINT_H
