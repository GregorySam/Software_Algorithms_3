//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_POINT_H
#define SOFWARE_ALGORITHMS_1_POINT_H

#include <vector>
#include <string>

using namespace std;
class Point {
    vector<double> v;
    vector<int> group_flags;       //all possible clusters that the point can be assigned
    unsigned int index{};
    int group_flag{};               //the flag of cluster the point belongs to


public:
    Point(const vector<double>&);
    Point()= default;
    ~Point()= default;


    vector<double> GetVector();


    unsigned int GetIndex();
    void SetGroupFlag(int);
    int GetGroupFlag();
    void Addgroupflag(int);
    vector<int> GetGroups();
    void SetVector(vector<double>);



};


#endif //SOFWARE_ALGORITHMS_1_POINT_H
