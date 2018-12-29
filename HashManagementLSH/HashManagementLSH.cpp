//
// Created by greg on 12/10/2018.
//

#include "HashManagementLSH.h"
#include "HashTable/HashTable.h"

#include <algorithm>
#include <random>
#include <utility>
#include <iostream>


HashManagementLSH::HashManagementLSH(unsigned int dim, unsigned int L, unsigned int k, unsigned int n, string hash_name): hash_tables_num(L), hash_tables_size(n / 2), functions_number(k) {

    unsigned int i,j,l;

    HashFunctions.resize(functions_number);
    HT.resize(hash_tables_num);


    HashTable::InitializeR(k);                  //Create ris for φ function
    if(hash_name=="euclidean")                  //Create H
    {
        for (i = 0; i < k; i++)
        {
            HashFunctions[i]=new Euclidean_h(dim);
        }

    }
    else if(hash_name=="cosine")
    {
        for (i = 0; i < k; i++)
        {
            HashFunctions[i]=new Cosine(dim);
        }
    }

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());

    uniform_int_distribution<int> u_distribution(1,k);



    vector<vector<HashFunction*>> H;

    H.resize(L);

    H[0].resize(k);

    for(j=0;j<k;j++) {                              //Generate g functions checking for duplicate gs
        H[0][j] = HashFunctions[u_distribution(gen) - 1];
    }
    HT[0]=new HashTable(k,hash_tables_size,H[0]);

    for (i = 1; i < L;)
    {
        H[i].resize(k);
        for(j=0;j<k;j++) {
            H[i][j] = HashFunctions[u_distribution(gen) - 1];
        }
        for(l=0;l<i;l++) {
            if (H[l] == H[i]) {
                break;
            }
        }
        if(l==i)
        {
            HT[i] = new HashTable(k, hash_tables_size, H[i]);
            i++;
        }
    }

}

HashManagementLSH::~HashManagementLSH()
{
    for (unsigned int i = 0; i < functions_number; i++)
    {
        delete HashFunctions[i];

    }

    for (unsigned int i = 0; i < hash_tables_num; i++)
    {
       delete HT[i];
    }
}

void HashManagementLSH::InsertInHashTables(vector<Point>& Points)
{

    unsigned int i,j;


    for(j=0;j<Points.size();j++)
    {
        for(i=0;i< hash_tables_num;i++)
        {
            HT[i]->SavePoint(Points[j]);
        }

    }


}


void HashManagementLSH::SetRadius(double R)
{
    Radius=R;
}


void HashManagementLSH::SearchNNPoint(Point &P, unsigned int max)
{
    unsigned int i;
    unsigned int current_n=0,next_n;



    while(current_n<=max)
    {
        for(i=0;i< hash_tables_num;i++)
        {

            HT[i]->SetNNPointsPoint(P);

        }
        next_n=(unsigned int)P.GetGroups().size();
        if(current_n==next_n){break;}
        current_n=next_n;
    }


}

void HashManagementLSH::SearchNNCluster(Point& C,ClusterManagement& CM)
{
    unsigned int i;



    for(i=0;i< hash_tables_num;i++)             //Searach q in evry hash table
    {

        //HT[i]->SetNNPoints(C, Radius, CM);

    }


}
