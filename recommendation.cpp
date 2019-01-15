#include <iostream>
#include "IO/InputOutput.h"

int main(int argc,char** argv) {

    double total_ex_time_A,total_ex_time_B,total_ex_time;
    double A_MAE,B_MAE;
    clock_t begin_time,end_time;

    InputOutput IO(argc,argv);                              //Initialize parameters
    CryptoCurrencyRecommendation CCR;


    IO.ReadFiles(CCR);                                      //Read files

    CCR.SetCCScores();                                       //Set users cc scores


    /////////////////Cosine LSH Recommendation/////////////////////////
    ///////////////////////////////////////////////////////////////////////


        begin_time=clock();

        CCR.HashUsersLSH();
        CCR.CosineLSHSearchUsers(CCR.GetUsers(),5);                    ///A

        end_time=clock();

        total_ex_time_A=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
        A_MAE=CCR.ValidationCosineLSH_A();

//    ///////////////////////////////////////////////////////////////////

        begin_time=clock();

        CCR.ClusterTweets();
        CCR.CosineLSHSearchUsers(CCR.GetVirtualUsers(),2);             ///B

        end_time=clock();

        total_ex_time_B=(double)(end_time-begin_time)/CLOCKS_PER_SEC;
        B_MAE=CCR.ValidationCosineLSH_B();

        total_ex_time=total_ex_time_A+total_ex_time_B;
        IO.PrintResults(CCR,1,total_ex_time_A,A_MAE,B_MAE);
    /////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
        CCR.ResetUsersResults();



        /////////////Clustering Recommendation/////////////////////////////////

        begin_time=clock();

        CCR.ClusterUsers();                                     //A

        end_time=clock();
        A_MAE=CCR.ValidationClustering_A();

        total_ex_time_A=(double)(end_time-begin_time)/CLOCKS_PER_SEC;



        //////////////////////////////////////////////////////
        begin_time=clock();

        CCR.ClusterVirtualUsers();                              //B

        end_time=clock();

        B_MAE=CCR.ValidationClustering_B();
        total_ex_time_B=(double)(end_time-begin_time)/CLOCKS_PER_SEC;

        total_ex_time=total_ex_time_A+total_ex_time_B;
        IO.PrintResults(CCR,2,total_ex_time,A_MAE,B_MAE);
     //////////////////////////////////////////////////////////////////////////


}