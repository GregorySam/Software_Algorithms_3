//
// Created by greg on 12/11/2018.
//

#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <cfloat>
#include <fstream>
#include <utility>
#include "ClusterManagement.h"
#include "../HashFunctions/Euclidean/Euclidean.h"
#include "../HashFunctions/Cosine/Cosine.h"
#include "../Point/Point.h"


ClusterManagement::ClusterManagement(unsigned int num_ofcentroids,unsigned int points_number,unsigned int dim,string m)
{

    clusters_number=num_ofcentroids;
    ungrouped_points=points_number;

    dimension=dim;
    metric= std::move(m);

    Centroids.resize(clusters_number);
    Clusters.resize(clusters_number);
    Points.resize(ungrouped_points);

    D=new Distances(ungrouped_points);


}

static unsigned  int i=0;

void ClusterManagement::InsertPoint(Point P)
{

    Points[i]=P;

    i++;
}



typedef struct PointDist
{
    Point* r;
    double P_i;

    bool operator<(const PointDist& PD)
    {
        return (this->P_i < PD.P_i);
    }

}PointDist;


double EuclideanDistanceSquared(const vector<double> &a,const vector<double> &b)
{
    unsigned int i;
    double dist=0;

    for(i=0;i<a.size();i++)
    {
        dist=dist+(a[i]-b[i])*(a[i]-b[i]);
    }

    return dist;
}

void CalculatePartialSums(map<double ,PointDist>& mPD,Point& centroid,vector<Point>& P)    //Caclulate P(r)s
{
    unsigned int i;
    double dist_e,prev_dist=0;



    for(i=0;i<P.size();i++)
    {
        if(P[i].GetGroupFlag()==-1)
        {

            PointDist pd;
            pd.r=&P[i];
            dist_e=EuclideanDistanceSquared(P[i].GetVector(),centroid.GetVector());

            pd.P_i=prev_dist+dist_e;

            mPD.insert(pair <double ,PointDist> (pd.P_i,pd));
            prev_dist=pd.P_i;
        }

    }


}




Point* SelectNewCentroid(map<double ,PointDist>& mPD)               //select r P(r-1)<=d<=P(r)
{


    double max_dist=mPD.rbegin()->first;

    random_device                  rand_dev;
    mt19937                        generator(rand_dev());
    uniform_real_distribution<double> distr_d(0,max_dist);


    PointDist tmp;
    tmp.r= nullptr;
    tmp.P_i=distr_d(generator);

    auto const it =  mPD.upper_bound(tmp.P_i);

    return it->second.r;


}

void ClusterManagement::KmeansPlusPlus()
{
    unsigned int cur_centroid=0;
    Point* newCentroid;

    random_device                  rand_dev;
    mt19937                        generator(rand_dev());
    uniform_int_distribution<unsigned int>  distr_u(0,(unsigned int)Points.size()-1);



    newCentroid=&Points[distr_u(generator)];
    newCentroid->SetGroupFlag(cur_centroid);



    Centroids[cur_centroid]=&*newCentroid;
    Clusters[cur_centroid].push_back(newCentroid);

    ungrouped_points--;

    cur_centroid++;
    while(cur_centroid<=clusters_number-1)
    {
       map<double ,PointDist> D;

        CalculatePartialSums(D,*newCentroid,Points);

        newCentroid=SelectNewCentroid(D);
        newCentroid->SetGroupFlag(cur_centroid);


        Centroids[cur_centroid]=&*newCentroid;
        Clusters[cur_centroid].push_back(newCentroid);
        ungrouped_points--;
        cur_centroid++;

        D.clear();

    }


}


void ClusterManagement::AssignUngroupedPoint(Point *P)
{

    unsigned int j;
    int min_index=0;
    double dist,min_dist;


    min_dist=DBL_MAX;
    for(j=0;j<Centroids.size();j++)
    {
        if(metric=="euclidean")
        {
            dist=D->GetDistance(*P,*Centroids[j],metric);
        }
        else
        {
            dist=1-D->GetDistance(*P,*Centroids[j],metric);
        }

        if(dist<min_dist)
        {
            min_dist=dist;
            min_index=j;
        }
    }
    P->SetGroupFlag(min_index);
    ungrouped_points--;
    Clusters[min_index].push_back(P);


}




void ClusterManagement::DirectAssignPointsToClusters()
{
    unsigned int i;

    Point* P;

    for(i=0;i<Points.size();i++)
    {
        P=&Points[i];

        if(P->GetGroupFlag()==-1)
        {
            AssignUngroupedPoint(P);
        }

    }
}

bool ClusterManagement::CentroidsChange(vector<Point*> &OldCentroids)
{

    for(unsigned int i=0;i<OldCentroids.size();i++)
    {
        if(OldCentroids[i]->GetVector()!=Centroids[i]->GetVector())
        {
            return true;
        }
    }
    return false;
}


ClusterManagement::~ClusterManagement()
{
    i=0;
}

vector<double> DivideVectorByNum(const vector<double> &a ,const double b)
{
    vector<double> c;

    c.resize(a.size());
    for(unsigned int i=0;i<a.size();i++)
    {
        c[i]=a[i]/b;
    }
    return c;
}


vector<double> AddVectors(const vector<double>& a ,const vector<double>& b)
{
    vector<double> c;

    c.resize(a.size());
    for(unsigned int i=0;i<a.size();i++)
    {
        c[i]=a[i]+b[i];
    }
    return c;
}


void ClusterManagement::Reset(bool kmeans)
{
    unsigned int i;


    for(i=0;i<Points.size();i++)
    {
        Points[i].SetGroupFlag(-1);

    }


    for(i=0;i<Clusters.size();i++)
    {
        Centroids[i]->SetGroupFlag(i);
        Clusters[i].clear();
        ungrouped_points= static_cast<unsigned int>(Points.size());

        if(!kmeans)
        {
            Clusters[i].push_back(Centroids[i]);
            ungrouped_points-=clusters_number;
        }

    }

}


vector<vector<Point*>>& ClusterManagement::GetClusters() {
    return Clusters;
}

void ClusterManagement::KmeansUpdate()
{
    vector<vector<double>> means;
    vector<double> zeros(dimension,0.0);



    unsigned int i,j;

    means.resize(clusters_number);


    for(i=0;i<clusters_number;i++)
    {
        means[i]=zeros;
        if(Clusters[i].empty())
        {
            string name="C_mean";
            name.append(to_string(i));


            Centroids[i]=new Point(Centroids[i]->GetVector(),name);
            Centroids[i]->SetGroupFlag(i);
            continue;
        }
        else
        {
            for(j=0;j<Clusters[i].size();j++)
            {

                means[i]=AddVectors(means[i],Clusters[i][j]->GetVector());
            }
        }

        vector<double> new_v;
        new_v=DivideVectorByNum(means[i],(double)Clusters[i].size());

        string name="C_mean";
        name.append(to_string(i));


        Centroids[i]=new Point(new_v,name);
        Centroids[i]->SetGroupFlag(i);


    }



}


vector<Point*>& ClusterManagement::GetCentroids()
{
    return Centroids;
}

