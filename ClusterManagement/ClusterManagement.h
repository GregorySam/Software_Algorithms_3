//
// Created by greg on 12/11/2018.
//

#ifndef SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H
#define SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H

#include "../Point/Point.h"

#include <set>
#include <map>

using namespace std;


class ClusterManagement {

    vector<Point> Points;
    vector<Point*> Centroids;

    vector<vector<Point*>> Clusters;

    vector<Point*> CurrentlyFlagged_ANN;

    map<unsigned long int,double> CalculatedDistMap;

    vector<double> CalculatedDist;
    unsigned  long int calcdist_size;

    unsigned int dimension;
    unsigned int clusters_number;           //centroids number
    unsigned int ungrouped_points;          //number of points that dont belontg to any cluster
    unsigned int multiflag_occurances;      //total number of flags in all points who belong to more than one cluster
    string metric;


public:
    ClusterManagement(unsigned int,unsigned int,unsigned int,string);
    ~ClusterManagement()=default;
    void InsertPoint(Point);


    void SelectRandom_k();
    void KmeansPlusPlus();

    void AssignUngroupedPoint(Point *);             //Assign point to right cluster
    void AssignMultiflaggedPoint(Point *);

    void InsertInFlagged(Point*);               //Insert point in flagged points in order to assing it later to proper cluster
    void AssignCurrentlyFlagged();              //Assign all flagged points to right cluster
    void DirectAssignPointsToClusters();        //Direct assignment

    void KmeansUpdate();
    void PartitionAroundMedoids();

    bool CentroidsChange(vector<Point*> &);
    void Reset(bool);                           //Resets algorithm for next iteration for current algorithm
    void Restart(bool);                         //Resets arrays and parameters for next algorithm

    void SilhouetteAndPrint(ofstream&,bool,bool,double);        //print to file results
    double SilhouetteOfPoint(Point&);


    vector<Point>& GetPoints();
    vector<Point*>& GetCentroids();



    void SetFlagsNum(unsigned int);


    unsigned int GetUngroupedpoints_num();
    unsigned int GetMultiFlagged_Num();

    double GetDistance(Point& ,Point&);


    double SetRadius();


};


#endif //SOFTWARE_ALGORITMS2_CLUSTERMANAGEMENT_H
