#ifndef _CAMERA_
#define _CAMERA_

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>

class Camera{
    private:
    double _x,_y;
    double _nX,_nY;

    public:
    Camera(double,double);

    void Set_Camera_Normal(double,double);

    double get_x();
    double get_y();

    double get_nX();
    double get_nY();

    void Set_Coordinates(double,double);
    const double distance(double,double);
};

#endif