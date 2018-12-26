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
    unsigned long int size;


    clusters_number=num_ofcentroids;
    ungrouped_points=points_number;
    multiflag_occurances=0;
    dimension=dim;
    metric= std::move(m);

    Centroids.resize(clusters_number);
    Clusters.resize(clusters_number);
    Points.resize(ungrouped_points);

    calcdist_size=(ungrouped_points+ungrouped_points)*(ungrouped_points+ungrouped_points+1)/2+ungrouped_points;

    size=calcdist_size;
    while(true)                                             //use static sized vector to save distances and use minimum size to fit maximum number of distances,else use dynamic map
    {
        try
        {
            CalculatedDist.resize(size,nan(""));
            calcdist_size=size;
        }
        catch(std::bad_alloc& exc)
        {
            size=size/2;
            continue;
        }
        break;
    }




}



void ClusterManagement::InsertPoint(Point P)
{
    static unsigned  int i=0;

    Points[i]=P;

    i++;
}


void ClusterManagement::SelectRandom_k()
{

    random_device                  rand_dev;
    mt19937                        generator(rand_dev());
    uniform_int_distribution<int>  distr(0, static_cast<int>(Points.size() - 1));

    vector<int> picked_ints;
    picked_ints.resize(clusters_number);


    Point* p;
    for(unsigned int i=0;i<=clusters_number-1;i++)
    {
        int random=distr(generator);
        if(find(picked_ints.begin(),picked_ints.end(),random)!=picked_ints.end())
        {
            i--;
            continue;
        }
        picked_ints[i]=random;

        p=&Points[random];
        p->SetGroupFlag(i);


        Centroids[i]=&*p;
        Centroids[i]->SetGroupFlag(i);

        Clusters[i].push_back(p);
       ungrouped_points--;

    }

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


void ClusterManagement::SetFlagsNum(unsigned int num)
{
    multiflag_occurances=num;
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

double ClusterManagement::SetRadius()                   //get minimum radius based on centroids distance
{
    unsigned int i,j;
    double min=DBL_MAX,dist;

    for(i=0;i<Centroids.size();i++)
    {
        for(j=0;j<Centroids.size();j++)
        {
            if(i!=j)
            {
                dist=GetDistance(*Centroids.at(i),*Centroids.at(j));
                if(dist<min)
                {
                    min=dist;
                }
            }
        }
    }

    return min;
}

void ClusterManagement::AssignUngroupedPoint(Point *P)
{

    unsigned int j;
    int min_index=0;
    double dist,min_dist;


    min_dist=DBL_MAX;
    for(j=0;j<Centroids.size();j++)
    {
        dist=GetDistance(*P,*Centroids[j]);
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

void ClusterManagement::AssignMultiflaggedPoint(Point *P)
{
    unsigned int j;
    int min_index=0;
    double dist,min_dist;

    vector<int> flags=P->GetGroups();
    min_dist=DBL_MAX;
    for(j=0;j<flags.size();j++)
    {
        dist=GetDistance(*P,*Centroids[flags[j]]);
        if(dist<min_dist)
        {
            min_dist=dist;
            min_index=flags[j];
        }
    }
    P->SetGroupFlag(min_index);
    ungrouped_points--;

    Clusters[min_index].push_back(P);
    if(flags.size()>1)
    { multiflag_occurances-=flags.size();}



}

void ClusterManagement::PartitionAroundMedoids()
{
    unsigned int i,j,k,best_point=0;
    double cost,min_cost;

    for(i=0;i<Clusters.size();i++)
    {

        min_cost=DBL_MAX;

        for(j=0;j<Clusters[i].size();j++)
        {
            cost=0.0;
            for(k=0;k<Clusters[i].size();k++)
            {
                if(k!=j)
                {
                   cost=cost+GetDistance(*Clusters[i][j],*Clusters[i][k]);
                }
            }
            if(cost<min_cost)
            {
                min_cost=cost;
                best_point=j;
            }
        }
        Centroids[i]=&*Clusters[i][best_point];
        Centroids[i]->SetGroupFlag(i);
    }

}


void ClusterManagement::AssignCurrentlyFlagged()
{
    unsigned int i;
    Point* P;
    vector<int> groups;

    for(i=0;i<CurrentlyFlagged_ANN.size();i++)
    {
        P=CurrentlyFlagged_ANN[i];
        groups=P->GetGroups();
        if(groups.size()==1)
        {
            P->SetGroupFlag(groups[0]);
            ungrouped_points--;
        }
        else
        {
            AssignMultiflaggedPoint(P);
        }
    }
    CurrentlyFlagged_ANN.clear();
}

void ClusterManagement::InsertInFlagged(Point *P)
{
    CurrentlyFlagged_ANN.push_back(P);
}

void ClusterManagement::DirectAssignPointsToClusters()
{
    unsigned int i;
    vector<int> groups;
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

void ClusterManagement::Restart(bool kmeans)
{

    unsigned int i;

    for(i=0;i<Points.size();i++)
    {
        Points[i].SetGroupFlag(-1);

    }

    CalculatedDistMap.clear();

    for(i=0;i<calcdist_size;i++)
    {
        CalculatedDist[i]=nan("");
    }


    if(kmeans)
    {
        for(i=0;i<Centroids.size();i++)
        {
            delete Centroids[i];
        }
    }


    for(i=0;i<Clusters.size();i++)
    {

        Clusters[i].clear();

    }
    ungrouped_points= static_cast<unsigned int>(Points.size());
    multiflag_occurances=0;
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

    multiflag_occurances=0;
}

double ClusterManagement::SilhouetteOfPoint(Point& P)
{
    int min_flag=0,flag=P.GetGroupFlag();
    unsigned int i;
    double dist,sum_dist=0,min_dist=DBL_MAX;
    double a_i,b_i;


    for(i=0;i<Clusters[flag].size();i++)
    {
        if(Clusters[flag][i]->GetIndex()!=P.GetIndex())
        {
            dist=GetDistance(P,*Clusters[flag][i]);
        }
        else
        {
            dist=0;
        }

        sum_dist+=dist;
    }
    a_i=sum_dist/(double)(Clusters[flag].size());


    for(i=0;i<Centroids.size();i++)
    {
        if((int)i!=flag)
        {
            dist=GetDistance(P,*Centroids[i]);
            if(dist<min_dist)
            {
                min_dist=dist;
                min_flag=i;
            }
        }
    }
    sum_dist=0;

    for(i=0;i<Clusters[min_flag].size();i++)
    {
        dist=GetDistance(P,*Clusters[min_flag][i]);
        sum_dist+=dist;
    }
    b_i=sum_dist/(double)(Clusters[min_flag].size());
    if(b_i!=b_i)
    {return nan("");}

    return (b_i-a_i)/max(a_i,b_i);

}

void ClusterManagement::SilhouetteAndPrint(ofstream& out,bool complete,bool kmeans,double time)
{
    unsigned int i,j;
    long double stotal=0,p_sil;
    vector<long double> sk(Clusters.size()+1,0.0);

    for(i=0;i<Clusters.size();i++)
    {
        out<<"  CLUSTER-"<<i<<" {size:"<<Clusters[i].size()<<",centroid:";
        string name=Centroids[i]->GetName();
        if(kmeans)
        {
            vector<double> d=Centroids[i]->GetVector();

            for (double k : d) {out<<k<<" ";}
        }
        else
        {
            out<<name;
        }
        out<<"}"<<endl;
        if(complete){ out<<"  {";}
        if(Clusters[i].empty())
        {
            if(complete){out<<"}"<<endl<<endl;}
            sk[i]=nan("");
            continue;
        }


        for(j=0;j<(unsigned int)Clusters[i].size();j++)
        {

            if(complete)
            {
                if(j==(Clusters[i].size()-1))
                {
                    out<<Clusters[i][j]->GetName()<<"}"<<endl<<endl;
                }
                else
                {
                    out<<Clusters[i][j]->GetName()<<",";
                }
            }
            p_sil=SilhouetteOfPoint(*Clusters[i][j]);
            if(isnan(p_sil))
            {
                sk[i]=nan("");
                continue;
            }
            stotal+=p_sil;
            sk[i]+=p_sil;
        }
        sk[i]=sk[i]/(double)Clusters[i].size();
    }
    stotal=stotal/(double)Points.size();

    sk[Clusters.size()]=stotal;

    out<<"  clustering_time:"<<time<<endl;
    out<<"  Silhouette:[";
    for(i=0;i<=Clusters.size();i++)
    {
        if(i==(Clusters.size()))
        {
            out<<sk[i]<<"]"<<endl<<endl;
            break;
        }
        out<<sk[i]<<",";

    }


}


unsigned long int GetKey2(unsigned int a, unsigned int b)            //function from NXN->N
{
    unsigned int tmp;

    if(a>b)
    {
        tmp=b;
        b=a;
        a=tmp;
    }
    return (((a+b)*(a+b+1))/2+b);
}



double ClusterManagement::GetDistance(Point& Pa, Point& Pb)             //save distance and pair (a,b) to map or distances with keys
{

    double dist=0;
    bool usingmap=false;



    unsigned long int key=GetKey2(Pa.GetIndex(),Pb.GetIndex());

    if(Pa.GetIndex()>Points.size() || Pb.GetIndex()>Points.size())
    {

        if (metric == "euclidean") {
            dist = CalculateEuclideanDistance(Pa.GetVector(), Pb.GetVector());
        } else if (metric == "cosine") {
            dist = CalculateSimilarity(Pa.GetVector(), Pb.GetVector());
        }
        return dist;
    }

    if(key>=calcdist_size)
    {
        map<unsigned long int,double>::iterator it;
        it=CalculatedDistMap.find(key);
        if(it!=CalculatedDistMap.end())
        {
            return it->second;
        }
        usingmap=true;

    }
    else
    {

        if(!isnan(CalculatedDist[key]))
        {
            return CalculatedDist[key];
        }

    }


    if (metric == "euclidean") {
        dist = CalculateEuclideanDistance(Pa.GetVector(), Pb.GetVector());
    } else if (metric == "cosine") {
        dist = CalculateSimilarity(Pa.GetVector(), Pb.GetVector());
    }

    if(usingmap)
    {
        try {
            CalculatedDistMap.insert(make_pair(key,dist));
        }
        catch(std::bad_alloc& exc){

        }


    }
    else if(key<calcdist_size)
    {
        CalculatedDist[key]=dist;
    }
    return dist;

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


            Centroids[i]=new Point(Centroids[i]->GetVector(),name,i);
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


        Centroids[i]=new Point(new_v,name,i);
        Centroids[i]->SetGroupFlag(i);


    }



}

vector<Point>& ClusterManagement::GetPoints()
{
    return Points;
}

vector<Point*>& ClusterManagement::GetCentroids()
{
    return Centroids;
}


unsigned int ClusterManagement::GetUngroupedpoints_num(){
    return ungrouped_points;
}

unsigned int ClusterManagement::GetMultiFlagged_Num()
{
    return multiflag_occurances;
}


