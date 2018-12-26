//
// Created by greg on 12/10/2018.
//

#include "HashTable.h"
#include "../../HashFunctions/Euclidean/Euclidean.h"
#include "../../HashFunctions/Cosine/Cosine.h"

#include <random>
#include <numeric>
#include <cmath>
#include <utility>
#include <algorithm>
#include <iostream>


vector<int> HashTable::R;

void HashTable::InitializeR(unsigned int num)
{
    unsigned int j;
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());

    uniform_int_distribution<int> u_distribution(1,num);     //create R of φ
    R.resize(num);
    for(j=0;j<num;j++)
    {
        HashTable::R[j]=u_distribution(gen);

    }
}

HashTable::HashTable(unsigned int k,unsigned int table_size,vector<HashFunction*> HF):TableSize(table_size)
{

    Points.resize(TableSize);

    R.resize(k);
    Hush_F.resize(k);
    hashf_num=k;

    Hush_F= std::move(HF);


}

long long CalculateMod(long long a,long long b)  //Calc MOD
{
    return (a % b + b) %b;
}

long long HashTable::CalculateFAndH(HashTableElement& H)
{
    unsigned int i;
    int h;

    vector <double> p=H.GetPoint()->GetVector();
    for(i=0;i<Hush_F.size();i++)            //Set g(p)
    {
        h=Hush_F[i]->CalculateValue(p);

        H.SetG(i,h);
    }
    vector<int> v=H.GetG();


    int i_product=inner_product(begin(v), end(v), begin(R), 0);

    long long mod1=CalculateMod(i_product,M);


    return CalculateMod(mod1,TableSize);        //return φ

}


void HashTable::SavePoint(Point& P)
{
    long long f;



    HashTableElement HE(P,hashf_num);
    f=CalculateFAndH(HE);

    Points[f].push_back(HE);

}



void HashTable::SetNNPointsPoint(Point &P, set<pair<double,unsigned int>,CompFun>& nearest_neighbours,double radius,unsigned int& max_n,Distances& D)
{
    unsigned int i;
    long long f;

    HashTableElement* pH= nullptr;
    string min_name;
    double dist=0;
    unsigned int id= P.GetIndex();

    HashTableElement HE(P,hashf_num);
    f=CalculateFAndH(HE);



    for(i=0;i<Points[f].size();i++)
    {
        pH=&Points[f][i];

        vector<int> groups=pH->GetPoint()->GetGroups();

        if(find(groups.begin(),groups.end(),id)!=groups.end()){ continue;}

        if(pH->GetG()==HE.GetG()) {                                                                         //if g(p)==g(q)

            dist=1-D.GetDistance(P,*pH->GetPoint(),"cosine");

            if (dist <= radius || radius==0) {

                nearest_neighbours.insert(make_pair(dist,pH->GetPoint()->GetIndex()));
                pH->GetPoint()->Addgroupflag((int)id);
            }
        }
    }





}

void HashTable::SetNNPointsCluster(Point &q, double Radius, ClusterManagement &CM)   //find in hashtable NN of point q within Radius
{
    unsigned int i;
    long long f;

    HashTableElement* pH= nullptr;
    string min_name;
    double dist=0;
    int flag=q.GetGroupFlag();

    HashTableElement HE(q,hashf_num);
    f=CalculateFAndH(HE);



    for(i=0;i<Points[f].size();i++)
    {
        pH=&Points[f][i];
        if(pH->GetPoint()->GetGroupFlag()!=-1){continue;}

        vector<int> groups=pH->GetPoint()->GetGroups();

        if(find(groups.begin(),groups.end(),flag)!=groups.end()){ continue;}

        if(pH->GetG()==HE.GetG()) {                                                                         //if g(p)==g(q)

            dist=CM.GetDistance(q,*pH->GetPoint());

            if (dist <= Radius || Radius==0) {

                if(!groups.empty())                                     //Add to group flags the flag of centroid near point
               {
                    if(groups.size()==1)
                    {
                        CM.SetFlagsNum(CM.GetMultiFlagged_Num()+1);
                    }
                   CM.SetFlagsNum(CM.GetMultiFlagged_Num()+1);

                   pH->GetPoint()->Addgroupflag(flag);

                   continue;
               }
                pH->GetPoint()->Addgroupflag(flag);
                CM.InsertInFlagged(pH->GetPoint());
            }
        }
    }
}

HashTable::~HashTable()
{
    R.clear();
    Points.clear();

}
