//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_HASHMANAGEMENTLSH
#define SOFWARE_ALGORITHMS_1_HASHMANAGEMENTLSH

#include "HashTable/HashTable.h"
#include "../ClusterManagement/ClusterManagement.h"
#include "../HashFunctions/HashFunction.h"




class HashManagementLSH {
    unsigned int hash_tables_num;      //L parameter
    unsigned int hash_tables_size;     //n/2
    unsigned int functions_number;      //k
    double Radius;                      //R



    vector<HashFunction*> HashFunctions;    //H={h1,h2,h3....}
    vector<HashTable*> HT;                  //HashTables
public:

    HashManagementLSH(unsigned int, unsigned int,unsigned  int, unsigned int, string);
    ~HashManagementLSH();

    void InsertInHashTables(vector<Point>&);         //Insert p in evry hash table
    void SearchNNPoint(Point&,bool);

    void SearchNNCluster(Point&,ClusterManagement&);         //Search NN of q in evry hash table
                                                        //Search all hash tables for real nearest of q
    void SetRadius(double);
};


#endif //SOFWARE_ALGORITHMS_1_HASHMANAGEMENT_H
