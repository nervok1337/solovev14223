#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Config {
public:
    string filename;
    int countAnts;
    int maxIter;
    int iterToStop;
    double epsilon;
    double alpha,beta,rho;
    double Q;

    Config(string configname);
};
#endif //_SETTINGS_H
