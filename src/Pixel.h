#ifndef _PIXEL_
#define _PIXEL_

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>

class Pixel
{
    private:
    double _x,_y;

    public:
    
    double get_x();
    double get_y();
    void Set_Position(double,double);
};
#endif
