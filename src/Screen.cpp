#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>
#include"Screen.h"

Screen::Screen(double X1, double Y1, double X2, double Y2, int n_pixels) 
: _x1(X1),_y1(Y1),_x2(X2),_y2(Y2){
    screen.resize(n_pixels);
}

void Screen::Append_to_Screen(int pixel_index,double obstacle_distance,char obstacle_type){
    screen[pixel_index].first = obstacle_distance;
    screen[pixel_index].second = obstacle_type;
}

double Screen::get_x1(){
    return(_x1);
}

double Screen::get_x2(){
    return(_x2);
}

double Screen::get_y1(){
    return(_y1);
}

double Screen::get_y2(){
    return(_y2);
}

void Screen::Set_Coordinates(double X1, double Y1, double X2, double Y2){
    _x1 = X1;
    _x2 = X2;
    _y1 = Y1;
    _y2 = Y2;
}