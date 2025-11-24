#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Config {
public:
    int size;
    int winLength;
    int depth;

    Config(string configname);
};
#endif //_SETTINGS_H
