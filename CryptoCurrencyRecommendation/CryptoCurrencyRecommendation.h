//
// Created by greg on 21/12/2018.
//

#ifndef SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
#define SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H

#include <map>

#include "../Point/Distances.h"
#include "../HashManagementLSH/HashManagementLSH.h"
#include "../ClusterManagement/ClusterManagement.h"
#include "Tweet/Tweet.h"
#include "User/User.h"

class CryptoCurrencyRecommendation {

    vector<User*> Users;
    vector<Point> UsersP;
    vector<Point> VUsers;



    vector<Tweet> Tweets;


    Distances *D=nullptr;

    HashManagementLSH* CosineLSH=nullptr;
    ClusterManagement* VUCluster=nullptr;

    unsigned int users_num=0;
    unsigned int tweets_num=0;
    unsigned int cc_num=0;

    unsigned int P=0;




    multimap<string,unsigned int> dictionary_cc;
    vector<string> CC_names;
    map<string,double> words_scores;

public:
    CryptoCurrencyRecommendation()=default;

    void SetUsersNum(unsigned int);
    void SetTweetsNum(unsigned int);
    void SetCCNum(unsigned int);
    void SetPNeighbours(unsigned int);

    vector<Point>& GetUsers();
    vector<Point>& GetVirtualUsers();
    set<pair<double,unsigned int>,CompFun> GetNNDist(Point&,vector<Point>&);
    void ResetUsersResults();

    void AddUser(User*);
    void AddTweet(Tweet);
    void Addcc_key(string,unsigned int);
    void Addcc_name(string);
    void Add_word(string,double);

    void SetTweetScore(vector<string>, unsigned int);
    void SetCCScores();
    vector<pair<double,unsigned int>> SetNanScores(set<pair<double,unsigned int>,CompFun>&, unsigned int,User&,vector<Point>&,string);
    Tweet* GetTweet(unsigned int);
    User* GetUser(unsigned int);

    void CosineLSHSearchUsers(vector<Point>&,unsigned int);
    void HashUsersLSH();
    void ClusterTweets();
    void ClusterUsers();
    void ClusterVirtualUsers();


    vector<vector<pair<unsigned int,unsigned int>>>  CreateValidationSets();
    double ValidationCosineLSH_A();
    double ValidationCosineLSH_B();
    double ValidationClustering_A();
    double ValidationClustering_B();

    vector<string> OutResults(vector<pair<double,unsigned int>>&, unsigned int);
    void PrintResults(ofstream&);

    ~CryptoCurrencyRecommendation();




};


#endif //SOFTWARE_ALGORITHMS_3_CRYPTOCURRENCYRECOMMENDATION_H
