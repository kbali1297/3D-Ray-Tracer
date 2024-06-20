#ifndef _SCREEN_
#define _SCREEN_

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>

class Screen{
    
    private:
    double _x1,_y1,_x2,_y2;
    public:
    std::vector<std::pair<double,char>> screen;
    Screen(double ,double , double , double ,int);
    
    double get_x1();
    double get_x2();
    double get_y1();
    double get_y2();

    void Set_Coordinates(double,double,double,double);
    void Append_to_Screen(int,double,char);
};

#endif