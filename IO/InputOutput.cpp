//
// Created by greg on 13/10/2018.
//

#include "InputOutput.h"

#include <getopt.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>



void Print_Usage()
{
    cout<<"$./recommendation –i <input file> -v <input file vectors> -b <bitcoins> -d <vader dict> -o <output file> -validate\n-validate is not mandatory "<<endl;
}

InputOutput::InputOutput(int argc,char** argv)
{
    int c;
    int par_num=argc;
    FILE* fp;

   Validate=false;
   users_number=0;
   bitcoins_number=0;
   tweets_number=0;

   if(argc==12)                                    //if -complete
    {
        par_num=argc-1;
        Validate=true;
    }
    while((c=getopt(par_num,argv,"i:v:b:d:o:"))!=-1)      //manage parameters
    {
        switch(c) {
            case 'i':
                tweets_input_file=optarg;
                fp=fopen(optarg,"r");
                if(fp== nullptr){
                    cout<<"Tweets string file does not exist\n";
                    exit(EXIT_FAILURE);
                }
                fclose(fp);
                break;
            case 'v':
                vector_tweets_input_file=optarg;
                fp=fopen(optarg,"r");
                if(fp== nullptr){
                    cout<<"Tweets vector file does not exist\n";
                    exit(EXIT_FAILURE);
                }
                fclose(fp);
                break;
            case 'b':
                bitcoinsDict=optarg;
                fp=fopen(optarg,"r");
                if(fp== nullptr){
                    cout<<"Bitcoins keys file does not exist\n";
                    exit(EXIT_FAILURE);
                }
                fclose(fp);
                break;
            case 'd':
                vaderDict=optarg;
                fp=fopen(optarg,"r");
                if(fp== nullptr){
                    cout<<"Vader file does not exist\n";
                    exit(EXIT_FAILURE);
                }
                fclose(fp);
                break;
            case 'o':
                output_file=optarg;
                break;
            default:
                Print_Usage();
                exit(EXIT_FAILURE);
        }
    }

    if(tweets_input_file.empty() || vector_tweets_input_file.empty() || output_file.empty() ||  vaderDict.empty() ||  bitcoinsDict.empty())
    {Print_Usage();exit(EXIT_FAILURE);}


}

vector<double> GetVector(const string& line)      //convert string of numbers to vector
{
    vector<double> numbers;
    double number;
    string token;

    stringstream iss(line);

    while(getline(iss, token, ',')) {
        number=stod(token);
        numbers.push_back(number);
    }

    return numbers;

}

void InputOutput::CheckInputFiles()     //read number of lines=number of vectors to save and radius from data file and check config file
{
    ifstream in(input_file);
    string line,num;

    number_of_points=0;
    dimension=0;
    vector<double> numbers;

    getline(in,line);

    if(!line.empty())
    {
        number_of_points++;
        num=line.substr(line.find(',')+1,line.length());
        numbers=GetVector(num);
        dimension=(unsigned int)numbers.size();

    }


    while(getline(in,line))
    {
        number_of_points++;
    }

    in.close();

    ifstream conf(configuration_file);

    getline(conf,line);if(line.empty()){cout<<"Error with config file"<<endl;exit(EXIT_FAILURE);}
    number_of_clusters=(unsigned int)stoul(line.substr(line.find(':')+1,line.length()));

    getline(conf,line);if(line.empty()){cout<<"Error with config file"<<endl;exit(EXIT_FAILURE);}
    number_of_hash_functions=(unsigned int)stoul(line.substr(line.find(':')+1,line.length()));

    getline(conf,line);if(line.empty()){cout<<"Error with config file"<<endl;exit(EXIT_FAILURE);}
    number_of_hash_tables=(unsigned int)stoul(line.substr(line.find(':')+1,line.length()));

    conf.close();
}

//
//void InputOutput::PrintToFile(ClusterManagement &CM,int init,int assign,int update,double time)
//{
//    static ofstream out(output_file);
//
//
//    out<<"Algorithm:I"<<init<<"A"<<assign<<"U"<<update<<endl;
//    out<<"Metric:"<<metric<<endl;
//    CM.SilhouetteAndPrint(out,DisplayClusters,update==1,time);
//
//
//}
//
//
//void InputOutput::ReadVectorsFile(ClusterManagement& CM){
//    ifstream in(input_file);
//    string line;
//    string name;
//    string num;
//
//    vector<double> numbers;
//
//    while(getline(in,line))
//    {
//        name=line.substr(0,line.find(','));
//        num=line.substr(line.find(',')+1,line.length());
//        numbers=GetVector(num);
//
//        Point P(numbers,name);
//        CM.InsertPoint(P);
//    }
//
//}

