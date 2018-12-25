//
// Created by greg on 12/10/2018.
//

#ifndef SOFWARE_ALGORITHMS_1_HASHTABLE_H
#define SOFWARE_ALGORITHMS_1_HASHTABLE_H

#include <vector>
using namespace std;

#include "HashTableElement.h"
#include "../../ClusterManagement/ClusterManagement.h"
#include "../../HashFunctions/Euclidean/Euclidean.h"
#include "../../HashFunctions/Cosine/Cosine.h"
#include "../../HashFunctions/HashFunction.h"



class HashTable {
    static vector<int> R;                                        //ris of φ funstion
    static const long long M=4294967291;
    unsigned int TableSize;
    unsigned int hashf_num;

    vector<vector<HashTableElement>> Points;

    vector<HashFunction*> Hush_F;                           //Set of H functions
    long long CalculateFAndH(HashTableElement&);

public:
    HashTable(unsigned int,unsigned int,vector<HashFunction*>);
    ~HashTable();

    void SavePoint(Point&);
    static  void InitializeR(unsigned int);

    void SetNNPoints(Point&, double, ClusterManagement &);



};

#endif //SOFWARE_ALGORITHMS_1_HASHTABLE_H
