#include <iostream>
#include "IO/InputOutput.h"

int main(int argc,char** argv) {

    double B_ex_time,total_ex_time;
    clock_t begin_time,end_time;

    InputOutput IO(argc,argv);
    CryptoCurrencyRecommendation CCR;


    IO.ReadFiles(CCR);

    CCR.SetCCScores();


    /////////////////Cosine LSH Recommendation/////////////////////////
    ///////////////////////////////////////////////////////////////////////


        begin_time=clock();

        CCR.HashUsersLSH();
        CCR.CosineLSHSearchUsers(CCR.GetUsers(),5,false);                    ///1

        end_time=clock();

        total_ex_time=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
    /////////////////////////////////////////////////////////////////////

        begin_time=clock();


        CCR.ClusterTweets();
        CCR.CosineLSHSearchUsers(CCR.GetVirtualUsers(),2,true);             ///2

        end_time=clock();
        B_ex_time=(double)(end_time-begin_time)/CLOCKS_PER_SEC;

        total_ex_time+=B_ex_time;


        IO.PrintResults(CCR,1,total_ex_time);
    /////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////


        total_ex_time=0;
        CCR.ResetUsersResults();


        /////////////Clustering Recommendation/////////////////

        begin_time=clock();

        CCR.ClusterUsers();

        end_time=clock();

        total_ex_time=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
        total_ex_time+=B_ex_time;

        IO.PrintResults(CCR,2,total_ex_time);
     //////////////////////////////////////////////////////////////////////////


}