//
// Created by greg on 21/12/2018.
//

#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <ctime>
#include <map>
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

void ClusterPoints(ClusterManagement &CM, vector<Point> &Ps)
{

    unsigned int i,x=0;

    for(i=0;i<Ps.size();i++)
    {
        CM.InsertPoint(Ps[i]);
    }



    CM.KmeansPlusPlus();
    while(true)
    {
        CM.DirectAssignPointsToClusters();
        vector<Point*> OLdCentroids= CM.GetCentroids();
        CM.KmeansUpdate();

        if(!CM.CentroidsChange(OLdCentroids) || x==30){                  //if centroids have chenged

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



    ClusterManagement UsersCluster(users_num/P,users_num,(unsigned int)UsersP.at(0).GetVector().size(),"euclidean");

    ClusterPoints(UsersCluster,UsersP);

    vector<vector<Point*>>* UC;

    User** Up;
    UC=&UsersCluster.GetClusters();

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

            UserNan_scores=SetNanScores(NN,(unsigned int)(UC->at(i).size()-1),**Up,UsersP,"euclidean");
            sort(UserNan_scores.begin(),UserNan_scores.end(),std::greater<>());

            cc_ResultsName=OutResults(UserNan_scores,5);
            (*Up)->AddCCResults(cc_ResultsName);
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


    ClusterManagement TweetsCluster(150,tweets_num,(unsigned int)TweetsP.at(0).GetVector().size(),"cosine");


    ClusterPoints(TweetsCluster,TweetsP);

    vector<User> VirtualUsers(150,User(cc_num,""));


    vector<vector<Point*>>* TC;

    TC=&TweetsCluster.GetClusters();

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
        VUsers[i].SetId(i);
    }



}

vector<Point> ConvertUsersToPoints(vector<User*>& U)
{
    unsigned int i;
    vector<double>* p_scores;

    vector<Point> UP(U.size());

    Point::Reset();

    for(i=0;i<U.size();i++)
    {
        p_scores=&U[i]->GetCC_Scores();

        Point Po(*p_scores,"");
        Po.SetGroupFlag((int)i);
        UP[i]=Po;
    }
    return UP;

}



void CryptoCurrencyRecommendation::HashUsersLSH() {

    CosineLSH=new HashManagementLSH(cc_num,7,5,(unsigned int)Users.size(),"cosine");



    Point::Reset();

    UsersP=vector<Point>(Users.size());

    UsersP=ConvertUsersToPoints(Users);


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


void CryptoCurrencyRecommendation::ClusterVirtualUsers()
{
    unsigned int i,j,k,best_cluster=0;
    double min_dist=MAXFLOAT,dist;


    VUCluster=new ClusterManagement(20,(unsigned int)VUsers.size(),(unsigned int)VUsers.at(0).GetVector().size(),"euclidean");

    for(i=0;i<VUsers.size();i++)
    {
        VUsers[i].SetGroupFlag(-1);
    }

    ClusterPoints(*VUCluster,VUsers);

    vector<vector<Point*>>* UC;
    Point* VUserP;

    User** Up;
    UC=&VUCluster->GetClusters();

    for(i=0;i<users_num;i++)
    {
        Up=&Users[i];

        for(j=0;j<UC->size();j++)
        {
            VUserP=UC->at(j)[0];
            dist=D->GetDistance(*VUserP,UsersP[i],"euclidean");
            if(dist<min_dist)
            {
                min_dist=dist;
                best_cluster=j;
            }

        }
        set<pair<double,unsigned int>,CompFun> NN;
        vector<pair<double,unsigned int>> UserNan_scores;
        vector<string> cc_ResultsName;

        for(k=0;k<UC->at(best_cluster).size();k++)
        {
            NN.insert(make_pair(0,UC->at(best_cluster)[k]->GetIndex()));
        }
        UserNan_scores=SetNanScores(NN,(unsigned int)(UC->at(best_cluster).size()),**Up,VUsers,"euclidean");

        sort(UserNan_scores.begin(),UserNan_scores.end(),std::greater<>());
        cc_ResultsName=OutResults(UserNan_scores,2);
        (*Up)->AddCCResults(cc_ResultsName);

    }

}


vector<pair<double,unsigned int>> CryptoCurrencyRecommendation::SetNanScores(set<pair<double,unsigned int>,CompFun> &NN, unsigned int max_n,User& U,vector<Point>& Points,string sim_metric)
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
        if(sim_metric=="euclidean")
        {
            z=z+1.0/(1+abs(D->GetDistance(Points[it->second],UsersP[U.GetUserId()],sim_metric)));
        }
        else
        {
            z=z+abs(D->GetDistance(Points[it->second],UsersP[U.GetUserId()],sim_metric));
        }

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
            if(sim_metric=="euclidean")
            {
                sum=sum+(1.0/(1+D->GetDistance(Points[it->second],UsersP[U.GetUserId()],sim_metric)))*Points[it->second].GetVector().at(nan_scoreindex);
            }
            else
            {
                sum=sum+D->GetDistance(Points[it->second],UsersP[U.GetUserId()],sim_metric)*Points[it->second].GetVector().at(nan_scoreindex);
            }

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
    unsigned int i,j,r;
    set<string> cc;
    set<string>::iterator it_cc;

    srand(time(NULL));
    for(i=0;i<Users.size();i++)
    {
        out<<Users[i]->GetName()<<" ";
        cc=Users[i]->GetCCRes();
        if(cc.empty())
        {
            out<<"(random recommandation)"<<" ";
            for(j=0;j<7;j++)
            {
                r=(unsigned int)rand()%100;
                out<<CC_names[r]<<" ";
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



vector<vector<pair<unsigned int,unsigned int>>> CryptoCurrencyRecommendation::CreateValidationSets()
{

    unsigned int i,j;
    unsigned int set_size,rem;

    vector<pair<unsigned int,unsigned int>> SiCi;

    vector<vector<pair<unsigned int,unsigned int>>> Sets;
    vector<unsigned int> user_knowncc;
    vector<double> zeros(0,cc_num);

    std::srand ( unsigned ( std::time(0) ) );



    {
        for(i=0;i<Users.size();i++)                                     /////////Set pairs u_i,s_i
        {

            if(Users[i]->GetCC_Scores()==zeros)
            {
                continue;
            }
            user_knowncc=Users[i]->GetKnownCC();

            for(j=0;j<user_knowncc.size();j++)
            {
                SiCi.emplace_back(make_pair(i,user_knowncc[j]));
            }
        }

    }

    {                                                                       //create sets

        set_size=(unsigned int)SiCi.size()/10;

        rem=(unsigned int)SiCi.size()%10;


        vector<pair<unsigned int,unsigned int>>::iterator it;


        random_shuffle(SiCi.begin(),SiCi.end());

        it=SiCi.begin();

        for(i=1;i<=10;i++)
        {
            if(i==10)
            {
                if(rem!=0)
                {
                    set_size=rem;
                }
            }
            vector<pair<unsigned int,unsigned int>> new_set(it,it+set_size);
            it=it+set_size;
            Sets.push_back(new_set);
        }

    }



    return  Sets;

}


double MAECalc(User& U, vector<pair<double,unsigned int>>& UserNan_scores)
{
    unsigned  int i;
    double error,sum=0;
    vector<double>* cc_scores;

    for(i=0;i<UserNan_scores.size();i++)
    {
        cc_scores=&U.GetCC_Scores();
        error=abs(cc_scores->at(UserNan_scores[i].second)-UserNan_scores[i].first);
        sum+=error;
    }
    return sum;

}





vector<User> CopyUsers(vector<User*>& U)
{
    unsigned int i;

    vector<User> Us;

    Us.resize(U.size());

    for(i=0;i<U.size();i++)
    {
        Us[i]=*U[i];
    }
    return Us;
}


vector<Point> CreateValidationPoints(vector<pair<unsigned int,unsigned int>>& Set,vector<User>& TestUsers,vector<Point>& TestUserP,set<unsigned int>& UserIdsRef)
{
    unsigned int j,user_id,cc_id;
    vector<Point> ValidationPoints;

     set<unsigned int>::iterator it;



    for(j=0;j<Set.size();j++)
    {
        user_id=Set[j].first;
        UserIdsRef.insert(user_id);

        cc_id=Set[j].second;
        TestUsers[user_id].SetCCScore(cc_id,nan(""));

    }
    ValidationPoints.resize(UserIdsRef.size());

    int a=0;
    for(it=UserIdsRef.begin();it!=UserIdsRef.end();it++)
    {
        user_id=*it;
        TestUsers[user_id].SetMeanValues();

        TestUserP[user_id].SetVector(TestUsers[user_id].GetCC_Scores());

        ValidationPoints[a]=TestUserP[user_id];
        a++;

    }
    return ValidationPoints;



}


double CryptoCurrencyRecommendation::ValidationCosineLSH_B()
{
    vector<vector<pair<unsigned int,unsigned int>>> Sets;
    unsigned int set_size,rem;
    unsigned int i,m;


    vector<User> TestUsers;
    vector<Point> TestUserPoints;

    vector<Point> ValidationPoints;
    set<unsigned int> UserIdsRef;
    set<unsigned int>::iterator it;

    vector<double> zeros(cc_num,0);

    double OverallError=0;
    double MeanOverallError=0;

    Sets=CreateValidationSets();

    for(i=0;i<Sets.size();i++)
    {
        TestUsers=CopyUsers(Users);
        TestUserPoints=UsersP;
        UserIdsRef.clear();

        ValidationPoints=CreateValidationPoints(Sets[i],TestUsers,TestUserPoints,UserIdsRef);

        HashManagementLSH TestHash(cc_num,7,5,(unsigned int)ValidationPoints.size(),"cosine");

        TestHash.InsertInHashTables(ValidationPoints);

        for(m=0;m<VUsers.size();m++)
        {
            if(VUsers[m].GetVector()==zeros)
            {
                continue;
            }
            TestHash.SearchNNPoint(VUsers[m],true);
        }
        double mean_error;
        double error=0;

        for(m=0;m<ValidationPoints.size();m++)
        {

            if(ValidationPoints[m].GetVector()==zeros)
            {
                continue;
            }
            vector<pair<double,unsigned int>> UserNan_scores;
            set<pair<double,unsigned int>,CompFun> NN;


            NN=GetNNDist( ValidationPoints[m],VUsers);


            if(NN.empty())
            {continue;}
            User* Up;
            Up=&TestUsers[ValidationPoints[m].GetIndex()];


            UserNan_scores=SetNanScores(NN,P,*Up,VUsers,"cosine");
            error+=MAECalc(*Users[ValidationPoints[m].GetIndex()],UserNan_scores);

            NN.clear();
        }
        mean_error=error/(double)Sets[i].size();
        OverallError+=mean_error;


    }
    MeanOverallError=OverallError/10;


    return MeanOverallError;



}


double CryptoCurrencyRecommendation::ValidationCosineLSH_A()
{
    unsigned int i,j,k,l,m;
    unsigned int user_id,cc_id;
    unsigned int set_size,rem,training_setsize;

    vector<pair<unsigned int,unsigned int>> SiCi;
    vector<vector<pair<unsigned int,unsigned int>>> Sets;
    vector<unsigned int> user_knowncc;

    set<unsigned int> UserIdsRef;
    set<unsigned int>::iterator it;
    vector<double> zeros(cc_num,0);



    Sets=CreateValidationSets();

    vector<User> TestUsers;
    vector<Point> TestUserPoints;

    vector<Point>ValidationPoints;
    vector<Point>TrainingPoints;




    double OverallError=0;
    double MeanOverallError=0;

    for(i=0;i<Sets.size();i++)
    {
        ////////Set train and validation points//////////////////////////////////
        TestUsers=CopyUsers(Users);
        TestUserPoints=UsersP;
        UserIdsRef.clear();


        ValidationPoints=CreateValidationPoints(Sets[i],TestUsers,TestUserPoints,UserIdsRef);
        int a;
        a=0;

        TrainingPoints.resize(users_num-ValidationPoints.size());
        for(k=0;k<users_num;k++)
        {
            if(UserIdsRef.find(k)==UserIdsRef.end())
            {
                TrainingPoints[a]=TestUserPoints[k];

                a++;
            }
        }
        training_setsize=(unsigned int)TrainingPoints.size();
        /////////////////////////////////////////////////////////////////////




        HashManagementLSH TestHash(cc_num,7,5,training_setsize,"cosine");

        TestHash.InsertInHashTables(TrainingPoints);

        for(m=0;m<ValidationPoints.size();m++)
        {

            if(ValidationPoints[m].GetVector()==zeros)
            {
                continue;
            }
            TestHash.SearchNNPoint(ValidationPoints[m],false);
        }

        double mean_error;
        double error=0;

        for(m=0;m<ValidationPoints.size();m++)
        {

            if(ValidationPoints[m].GetVector()==zeros)
            {
                continue;
            }
            vector<pair<double,unsigned int>> UserNan_scores;
            set<pair<double,unsigned int>,CompFun> NN;


            NN=GetNNDist( ValidationPoints[m],TestUserPoints);


            if(NN.empty())
            {continue;}
            User* Up;
            Up=&TestUsers[ValidationPoints[m].GetIndex()];


            UserNan_scores=SetNanScores(NN,P,*Up,TestUserPoints,"cosine");
            error+=MAECalc(*Users[ValidationPoints[m].GetIndex()],UserNan_scores);

            NN.clear();
        }
        mean_error=error/(double)Sets[i].size();
        OverallError+=mean_error;
    }
    MeanOverallError=OverallError/10;


    return MeanOverallError;

}


double CryptoCurrencyRecommendation::ValidationClustering_A()
{

    double dist,min_dist=MAXFLOAT,OverallError=0,MeanOverallError;
    vector<double> zeros(cc_num,0);

    vector<vector<pair<unsigned int,unsigned int>>> Sets;
    set<unsigned int> UserIdsRef;

    unsigned int i,k,j,best_cluster=0;

    Sets=CreateValidationSets();

    vector<User> TestUsers;
    vector<Point> TestUserPoints;


    vector<Point>ValidationPoints;
    vector<Point>TrainingPoints;

    for(i=0;i<Sets.size();i++) {

        TestUsers = CopyUsers(Users);
        TestUserPoints = UsersP;
        UserIdsRef.clear();

        ValidationPoints = CreateValidationPoints(Sets[i], TestUsers, TestUserPoints, UserIdsRef);

        int a;
        a = 0;

        TrainingPoints.resize(users_num - ValidationPoints.size());
        for (k = 0; k < users_num; k++) {
            if (UserIdsRef.find(k) == UserIdsRef.end()) {
                TrainingPoints[a] = TestUserPoints[k];

                a++;
            }
        }


        ClusterManagement TestCluster((unsigned int)TrainingPoints.size() / P, (unsigned int) TrainingPoints.size(),(unsigned int) UsersP.at(0).GetVector().size(), "euclidean");

        ClusterPoints(TestCluster, TrainingPoints);


        vector<vector<Point*>> *PC;
        PC = &TestCluster.GetClusters();

        double mean_error;
        double error=0;

        for (j = 0; j < ValidationPoints.size(); j++){


            if(ValidationPoints[j].GetVector()==zeros)
            {
                continue;
            }

            Point* testPoint=&ValidationPoints[j];

            for (k = 0; k < PC->size(); k++) {
                dist=D->GetDistance(*testPoint,*PC->at(k)[0],"euclidean");

                if(dist<min_dist)
                {
                    min_dist=dist;
                    best_cluster=k;
                }
            }

            set<pair<double,unsigned int>,CompFun> NN;
            vector<pair<double,unsigned int>> UserNan_scores;

            for(k=0;k<PC->at(best_cluster).size();k++)
            {
                NN.insert(make_pair(0,PC->at(best_cluster)[k]->GetIndex()));

            }


            if(NN.empty())
            {continue;}
            User* Up;
            Up=&TestUsers[ValidationPoints[j].GetIndex()];


            UserNan_scores=SetNanScores(NN,(unsigned int)NN.size(),*Up,TestUserPoints,"euclidean");
            error+=MAECalc(*Users[ValidationPoints[j].GetIndex()],UserNan_scores);

            NN.clear();

        }
        mean_error=error/(double)Sets[i].size();
        OverallError+=mean_error;

    }
    MeanOverallError=OverallError/10.0;


    return MeanOverallError;

}


double CryptoCurrencyRecommendation::ValidationClustering_B()
{

    vector<vector<pair<unsigned int,unsigned int>>> Sets;
    set<unsigned int> UserIdsRef;
    double dist,min_dist=MAXFLOAT,OverallError=0,MeanOverallError;
    vector<double> zeros(cc_num,0);

    vector<User> TestUsers;
    vector<Point>ValidationPoints;
    vector<Point> TestUserPoints;

    unsigned int i,k,j,best_cluster=0;

    Sets=CreateValidationSets();


    vector<vector<Point*>> *PC;
    PC = &VUCluster->GetClusters();

    for(i=0;i<Sets.size();i++) {

        TestUsers = CopyUsers(Users);
        TestUserPoints = UsersP;
        UserIdsRef.clear();

        ValidationPoints = CreateValidationPoints(Sets[i], TestUsers, TestUserPoints, UserIdsRef);


        double mean_error;
        double error=0;

        for (j = 0; j < ValidationPoints.size(); j++){


            if(ValidationPoints[j].GetVector()==zeros)
            {
                continue;
            }

            Point* testPoint=&ValidationPoints[j];

            for (k = 0; k < PC->size(); k++) {
                dist=D->GetDistance(*testPoint,*PC->at(k)[0],"euclidean");

                if(dist<min_dist)
                {
                    min_dist=dist;
                    best_cluster=k;
                }
            }

            set<pair<double,unsigned int>,CompFun> NN;
            vector<pair<double,unsigned int>> UserNan_scores;

            for(k=0;k<PC->at(best_cluster).size();k++)
            {
                NN.insert(make_pair(0,PC->at(best_cluster)[k]->GetIndex()));

            }


            if(NN.empty())
            {continue;}
            User* Up;
            Up=&TestUsers[ValidationPoints[j].GetIndex()];


            UserNan_scores=SetNanScores(NN,(unsigned int)NN.size(),*Up,TestUserPoints,"euclidean");
            error+=MAECalc(*Users[ValidationPoints[j].GetIndex()],UserNan_scores);

            NN.clear();

        }
        mean_error=error/(double)Sets[i].size();
        OverallError+=mean_error;

    }
    MeanOverallError=OverallError/10.0;


    return MeanOverallError;





}



void CryptoCurrencyRecommendation::CosineLSHSearchUsers(vector<Point>& S, unsigned int max_out)
{
    unsigned int i;


    vector<double> zeros(cc_num,0);


    vector<pair<double,unsigned int>> UserNan_scores;
    set<pair<double,unsigned int>,CompFun> NN;


    vector<string> cc_ResultsName;



    for(i=0;i<S.size();i++)
    {
        CosineLSH->SearchNNPoint(S[i],true);
    }
    for(i=0;i<UsersP.size();i++)
    {
        if(UsersP[i].GetVector()==zeros)
        {
            UsersP[i].SetGroupFlag(-1);
            continue;
        }

        NN=GetNNDist(UsersP[i],S);

        if(NN.empty())
        {continue;}

        UserNan_scores=SetNanScores(NN,P,*Users[i],S,"cosine");
        sort(UserNan_scores.begin(),UserNan_scores.end(),std::greater<>());
        cc_ResultsName=OutResults(UserNan_scores,max_out);
        Users[i]->AddCCResults(cc_ResultsName);
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

