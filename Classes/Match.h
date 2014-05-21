//
//  Match.h
//  TestCpp
//
//  Created by dozengame on 14-5-21.
//
//

#ifndef __TestCpp__Match__
#define __TestCpp__Match__

#include <iostream>
#include <string.h>
using namespace std;

class Match{
public:
    int mid;
    string host;
    string guest;
    string result;
    string comment;
    long long startTime;
    long long endTime;
    
    void getMatches(long long startTime, long long endTime);
    void getMatchById(int tid);
    
};

#endif /* defined(__TestCpp__Match__) */
