//
// Created by greg on 21/12/2018.
//

#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "CryptoCurrencyRecommendation.h"





void CryptoCurrencyRecommendation::AddTweet(Tweet T)
{
    Tweets.push_back(T);
}

void CryptoCurrencyRecommendation::AddUser(User* U)
{
    Users.push_back(U);
}


Tweet* CryptoCurrencyRecommendation::GetTweet(unsigned int id)
{
    return &Tweets.at(id-1);
}


vector<Point>& CryptoCurrencyRecommendation::GetVirtualUsers()
{
    return VUsers;
}

void CryptoCurrencyRecommendation::ClusterPoints(ClusterManagement &CM, vector<Point> &Ps)
{

    unsigned int i,x=0;

    for(i=0;i<Ps.size();i++)
    {
        CM.InsertPoint(Ps.at(i));
    }



    CM.KmeansPlusPlus();
    while(true)
    {
        CM.DirectAssignPointsToClusters();
        vector<Point*> OLdCentroids= CM.GetCentroids();
        CM.KmeansUpdate();

        if(!CM.CentroidsChange(OLdCentroids) || x==50){                  //if centroids have chenged

            ////Free mem/////////

            for(i=0;i<OLdCentroids.size();i++)
            {
                delete OLdCentroids[i];

            }

            /////////
            break;
        }
        //Free mem/////////
        if(x!=0)
        {
            for(i=0;i<OLdCentroids.size();i++)
            {
                delete OLdCentroids[i];

            }
        }
        ///////
        CM.Reset(true);
        x++;
    }

}

void CryptoCurrencyRecommendation::ClusterUsers()
{

    unsigned int j,i,k;



    ClusterManagement* UsersCluster;
    UsersCluster=new ClusterManagement(users_num/P,users_num,(unsigned int)UsersP.at(0).GetVector().size(),"euclidean");

    ClusterPoints(*UsersCluster,UsersP);

    vector<vector<Point*>>* UC;

    User** Up;
    UC=&UsersCluster->GetClusters();

    for(i=0;i<UC->size();i++)
    {
        if((int)(UC->at(i).size()-1)<=0){ continue;}
        for(j=0;j<UC->at(i).size();j++)
        {
            set<pair<double,unsigned int>,CompFun> NN;
            vector<pair<double,unsigned int>> UserNan_scores;
            vector<string> cc_ResultsName;

            Up=&Users[UC->at(i)[j]->GetIndex()];
            for(k=0;k<UC->at(i).size();k++)
            {
                if(k!=j)
                {
                    NN.insert(make_pair(0,UC->at(i)[k]->GetIndex()));
                }
            }

            UserNan_scores=SetNanScores(NN,(unsigned int)UC->at(i).size()-1,**Up,UsersP);
            sort(UserNan_scores.begin(),UserNan_scores.end(),std::greater<>());

            cc_ResultsName=OutResults(UserNan_scores,5);
            (*Up)->AddCCResults(cc_ResultsName,false);
        }
    }

}



void CryptoCurrencyRecommendation::ResetUsersResults()
{
    unsigned int i;

    for(i=0;i<Users.size();i++)
    {
        Users[i]->ResetResults();
    }
}


void CryptoCurrencyRecommendation::ClusterTweets()
{
    unsigned int j,i;


    Point::Reset();

    vector<Point> TweetsP;

    TweetsP=vector<Point>(tweets_num);
    for(i=0;i<tweets_num;i++)
    {
        Point Po(Tweets[i].GetVector(),"");
        TweetsP[i]=Po;
    }

    ClusterManagement* TweetsCluster;
    TweetsCluster=new ClusterManagement(10,tweets_num,(unsigned int)TweetsP.at(0).GetVector().size(),"cosine");


    ClusterPoints(*TweetsCluster,TweetsP);

    vector<User> VirtualUsers(10,User(cc_num));


    vector<vector<Point*>>* TC;

    TC=&TweetsCluster->GetClusters();

    for(i=0;i<TC->size();i++)
    {
        for(j=0;j<TC->at(i).size();j++)
        {
            unsigned int tweet_id;

            tweet_id=TC->at(i)[j]->GetIndex();

            VirtualUsers[i].AddTweet(&Tweets.at(tweet_id));
        }
        VirtualUsers[i].SetCCScores();
    }
    /////////////////////////////////////////////////////////////////////////////////

    VUsers=vector<Point>(VirtualUsers.size());



    for(i=0;i<VUsers.size();i++)
    {
        VUsers[i]=Point(VirtualUsers[i].GetCC_Scores(),"");
        VUsers[i].SetGroupFlag(i);
    }
    delete TweetsCluster;


}


void CryptoCurrencyRecommendation::HashUsersLSH() {

    CosineLSH=new HashManagementLSH(cc_num,7,5,(unsigned int)Users.size(),"cosine");

    unsigned int i;

    Point::Reset();

    UsersP=vector<Point>(Users.size());
    vector<double>* p_scores;


    /////////////////////////A    /////////////////////////////////////////////////////
    for(i=0;i<Users.size();i++)
    {
        p_scores=&Users[i]->GetCC_Scores();

        Point Po(*p_scores,"");
        Po.SetGroupFlag((int)i);
        UsersP[i]=Po;
    }

    CosineLSH->InsertInHashTables(UsersP);

}


void CryptoCurrencyRecommendation::SetTweetScore(vector<string> s, unsigned int tweetid)
{
    unsigned int i;
    double score=0;

    multimap<string,unsigned int>::iterator it_cc;
    map<string,double>::iterator it;

    for(i=0;i<s.size();i++)
    {
        it=words_scores.find(s[i]);
        if(it==words_scores.end())
        {
            it_cc=dictionary_cc.find(s[i]);
            if(it_cc!=dictionary_cc.end())
            {
                Tweets[tweetid-1].AddCC(it_cc->second);
            }
            continue;
        }
        score=score+it->second;
    }
    score=score/sqrt(score*score+15);
    Tweets[tweetid-1].SetScore(score);
}


vector<pair<double,unsigned int>> CryptoCurrencyRecommendation::SetNanScores(set<pair<double,unsigned int>,CompFun> &NN, unsigned int max_n,User& U,vector<Point>& Points)
{
    set<pair<double,unsigned int>,CompFun>::iterator it;
    unsigned int j,i,nan_scoreindex;

    vector<pair<double,unsigned int>> nan_scores;
    double z=0,sum=0;

    nan_scores=U.GetNanScores();


    it=NN.begin();
    for(i=0;i<max_n;i++)
    {
        if(it==NN.end()){break;}
        z=z+abs(D->GetDistance(Points[it->second],UsersP[U.GetUserId()],"cosine"));
        it++;

    }
    z=1.0/z;



    for(j=0;j<nan_scores.size();j++)
    {
        it=NN.begin();
        nan_scoreindex=nan_scores.at(j).second;
        for(i=0;i<max_n;i++)
        {
            if(it==NN.end()){break;}
            sum=sum+D->GetDistance(Points[it->second],UsersP[U.GetUserId()],"cosine")*Points[it->second].GetVector().at(nan_scoreindex);
            it++;

        }

        nan_scores.at(j).first=z*sum;
        sum=0;
    }
    return nan_scores;

}


vector<Point>& CryptoCurrencyRecommendation::GetUsers()
{
    return UsersP;
}



User* CryptoCurrencyRecommendation::GetUser(unsigned int id)
{
    return Users[id];
}


vector<string> CryptoCurrencyRecommendation::OutResults(vector<pair<double, unsigned int> >&UserNan_scores, unsigned int maxres)
{

    unsigned int j;
    vector<string> cc_ResultsName(maxres);

    if(UserNan_scores.size()<maxres)
    {
        maxres=(unsigned int)UserNan_scores.size();
    }
    for(j=0;j<maxres;j++)
    {
        cc_ResultsName[j]=CC_names[UserNan_scores[j].second];
    }

    return cc_ResultsName;

}


void CryptoCurrencyRecommendation::PrintResults(ofstream& out)
{
    unsigned int i,j;
    set<string> cc;
    set<string>::iterator it_cc;

    for(i=0;i<Users.size();i++)
    {
        out<<i<<" ";
        cc=Users[i]->GetCCRes();
        if(cc.empty())
        {
            out<<"(default recommandation)"<<" ";
            for(j=0;j<7;j++)
            {
                out<<CC_names[j]<<" ";
            }
        }
        else
        {
            it_cc=cc.begin();
            for(it_cc=cc.begin();it_cc!=cc.end();it_cc++)
            {
                out<<*it_cc<<" ";
            }
        }
        out<<endl;
    }
}


set<pair<double,unsigned int>,CompFun> CryptoCurrencyRecommendation::GetNNDist(Point &P,vector<Point>& Po)
{
    set<int> nn_indexes;
    unsigned int i;
    double dist;

    set<int>::iterator it;

    set<pair<double,unsigned int>,CompFun> NN;

    nn_indexes=P.GetGroups();
    it=nn_indexes.begin();

    for(i=0;i<nn_indexes.size();i++)
    {
        dist=1-D->GetDistance(P,Po[*it],"cosine");
        NN.insert(make_pair(dist,(unsigned int)*it));
        it++;
    }
    return NN;


}


void CryptoCurrencyRecommendation::CosineLSHSearchUsers(vector<Point>& S, unsigned int max_out,bool Beta)
{
    unsigned int i;


    vector<double> zeros(cc_num,0);


    vector<pair<double,unsigned int>> UserNan_scores;
    set<pair<double,unsigned int>,CompFun> NN;


    vector<string> cc_ResultsName;
    /////////////////////////A    /////////////////////////////////////////////////////



    for(i=0;i<S.size();i++)
    {
        CosineLSH->SearchNNPoint(S[i],P);
    }
    for(i=0;i<UsersP.size();i++)
    {

        NN=GetNNDist(UsersP[i],S);
        if(UsersP[i].GetVector()==zeros)
        {
            UsersP[i].SetGroupFlag(-1);
            continue;
        }
        UserNan_scores=SetNanScores(NN,P,*Users[i],S);
        sort(UserNan_scores.begin(),UserNan_scores.end(),std::greater<>());
        cc_ResultsName=OutResults(UserNan_scores,max_out);
        Users[i]->AddCCResults(cc_ResultsName,Beta);
        NN.clear();
        UsersP[i].SetGroupFlag(-1);
    }



    ///////////////////////////////////////////////////////////////////////
}


void CryptoCurrencyRecommendation::SetPNeighbours(unsigned int n)
{
    P=n;
}

void CryptoCurrencyRecommendation::SetCCScores()
{
    unsigned int i;

    for(i=0;i<Users.size();i++)
    {
        Users[i]->SetCCScores();
    }


}


CryptoCurrencyRecommendation::~CryptoCurrencyRecommendation()
{
    unsigned int i;
    for(i=0;i<users_num;i++)
    {
        delete Users[i];
    }
    delete D;
    delete CosineLSH;
}

void CryptoCurrencyRecommendation::SetCCNum(unsigned int n)
{
    cc_num=n;
}

void CryptoCurrencyRecommendation::SetUsersNum(unsigned int n)
{
    users_num=n;
    D=new Distances(users_num);
}

void CryptoCurrencyRecommendation::SetTweetsNum(unsigned int n)
{
    tweets_num=n;
}

void CryptoCurrencyRecommendation::Addcc_name(string name)
{
    CC_names.push_back(name);
}


void CryptoCurrencyRecommendation::Addcc_key(string key, unsigned int value)
{

    dictionary_cc.insert(pair<string,unsigned int>(key,value));
}

void CryptoCurrencyRecommendation::Add_word(string word, double score)
{
    words_scores.insert(pair<string,double>(word,score));
}

