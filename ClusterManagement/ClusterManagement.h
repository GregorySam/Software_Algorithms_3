//
// Created by greg on 12/11/2018.
//

#ifndef SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H
#define SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H

#include "../Point/Point.h"
#include "../Point/Distances.h"

#include <set>
#include <map>

using namespace std;


class ClusterManagement {

    vector<Point> Points;
    vector<Point*> Centroids;

    vector<vector<Point*>> Clusters;



    unsigned int dimension;
    unsigned int clusters_number;           //centroids number
    unsigned int ungrouped_points;          //number of points that dont belontg to any cluster
                                            //total number of flags in all points who belong to more than one cluster
    string metric;


public:
    ClusterManagement(unsigned int,unsigned int,unsigned int,string);
    ~ClusterManagement();
    void InsertPoint(Point);

    void KmeansPlusPlus();

    void AssignUngroupedPoint(Point *);             //Assign point to right cluster

    void DirectAssignPointsToClusters();        //Direct assignment

    void KmeansUpdate();

    bool CentroidsChange(vector<Point*> &);
    void Reset(bool);                           //Resets algorithm for next iteration for current algorithm



    vector<Point*>& GetCentroids();
    vector<vector<Point*>>& GetClusters();

};


#endif //SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H
