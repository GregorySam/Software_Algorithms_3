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
    cout<<"$./recommendation –i <input file> -v <input file vectors> -b <cccoins> -d <vader dict> -o <output file> -validate\n-validate is not mandatory "<<endl;
}

InputOutput::InputOutput(int argc,char** argv)
{
    int c;
    int par_num=argc;
    FILE* fp;

   Validate=false;
   users_number=0;
   cc_number=0;
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
                ccDict=optarg;
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

    if(tweets_input_file.empty() || vector_tweets_input_file.empty() || output_file.empty() ||  vaderDict.empty() ||  ccDict.empty())
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





vector<string> GetKeyWords(const string& line)
{
    vector<string> keywords;

    string token;

    stringstream iss(line);

    while(getline(iss, token,'\t')) {
        keywords.push_back(token);
    }

    return keywords;
}


void InputOutput::ReadFiles(CryptoCurrencyRecommendation& CCR)
{
    string line;

    ///////////////tweets vectors////////////////////////////////

    {
        ifstream tweets_v(vector_tweets_input_file);
        string num;
        vector<double> numbers;


        getline(tweets_v,line);
        num=line.substr(line.find(',')+1,line.length());
        numbers=GetVector(num);
        dimensions=(unsigned int)numbers.size();
        CCR.AddTweet(Tweet(numbers,tweets_number+1));
        tweets_number++;

        while(getline(tweets_v,line))
        {

            num=line.substr(line.find(',')+1,line.length());
            numbers=GetVector(num);

            CCR.AddTweet(Tweet(numbers,tweets_number+1));
            tweets_number++;
        }
        tweets_v.close();
    }


    ///////////////////////////////////////////////////////////

    ///////////////////words scores//////////////////////////
    {
        ifstream words_scores(vaderDict);

        string word;
        double score;

        while(getline(words_scores, line)) {

            word= line.substr(0,line.find('\t'));
            score = stod(line.substr(line.find('\t')+1,line.length()));
            CCR.Add_word(word,score);


        }
        words_scores.close();
    }
    ////////////////////////////////////////////////////

    /////////////////////cc dictionary///////////////////////////////
    {
        ifstream cc(ccDict);

        vector<string> keywords;
        unsigned int i;

        unsigned int val=0;
        while(getline(cc, line)) {
            keywords=GetKeyWords(line);

            CCR.Addcc_name(keywords[0]);
            for(i=1;i<keywords.size();i++)
            {
                CCR.Addcc_key(keywords[i],val);
            }

            val++;
        }
        cc_number=val;
    }
    /////////////////////////////////////////////////////////

    /////////////////////tweets ///////////////////////////////
    {
        ifstream tweets(tweets_input_file);
        string userid,newuserid,tweet,tmp;
        unsigned int tweetid;

        getline(tweets, line);
        P=(unsigned int)stoul(line.substr(line.find(':')+1,line.length()));

        getline(tweets, line);
        userid=line.substr(0,line.find('\t'));                  ///get string parts//////
        tmp=line.substr(line.find('\t')+1,line.length());
        tweetid=(unsigned int)stoul(tmp.substr(0,tmp.find('\t')));
        tweet=tmp.substr(tmp.find('\t')+1,tmp.length());


        User* U=new User(cc_number);                                      ///Add user and tweet to user

        users_number++;
        CCR.AddUser(U);

        U->AddTweet(CCR.GetTweet(tweetid));
        CCR.SetTweetScore(GetKeyWords(tweet),tweetid);


        while(getline(tweets, line)) {

            newuserid=line.substr(0,line.find('\t'));

            tmp=line.substr(line.find('\t')+1,line.length());
            tweetid=(unsigned int)stoul(tmp.substr(0,tmp.find('\t')));
            tweet=tmp.substr(tmp.find('\t')+1,tmp.length());

            CCR.SetTweetScore(GetKeyWords(tweet),tweetid);

            if(newuserid!=userid)
            {
                U=new User(cc_number);
                CCR.AddUser(U);
                userid=newuserid;
                users_number++;
            }
            U->AddTweet(CCR.GetTweet(tweetid));
        }
    }
    ////////////////////////////////////////////////////////////////




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

