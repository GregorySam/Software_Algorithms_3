#include <iostream>
#include "IO/InputOutput.h"

int main(int argc,char** argv) {


    InputOutput IO(argc,argv);

    CryptoCurrencyRecommendation CCR;

    IO.ReadFiles(CCR);



}