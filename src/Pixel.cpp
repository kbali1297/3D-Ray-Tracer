#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>
#include"Pixel.h"

double Pixel::get_x(){
    return(_x);
}

double Pixel::get_y(){
    return(_y);
}

void Pixel::Set_Position(double X,double Y){
    _x = X;
    _y = Y;
}

