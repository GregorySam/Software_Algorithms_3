#include <iostream>
#include "IO/InputOutput.h"

int main(int argc,char** argv) {


    InputOutput IO(argc,argv);
    CryptoCurrencyRecommendation CCR;

    IO.ReadFiles(CCR);

    CCR.SetCCScores();

    CCR.HashUsersLSH();
    CCR.ClusterTweets();


    /////////////////Cosine LSH Recommendation/////////////////////////
    CCR.CosineLSHSearchUsers(CCR.GetUsers(),5,false);                    ///1
    CCR.CosineLSHSearchUsers(CCR.GetVirtualUsers(),2,true);             ///2


    /////////////////Print Results////////////////////////////////////
    IO.PrintResults(CCR,1);




}