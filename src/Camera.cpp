#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>
#include"Camera.h"

Camera::Camera(double X,double Y):_x(X),_y(Y){
}

void Camera::Set_Coordinates(double X,double Y){
    _x = X;
    _y = Y;
}
void Camera::Set_Camera_Normal(double Xfinal,double Yfinal){
    _nX = Xfinal - _x;
    _nY = Yfinal - _y;
}

const double Camera::distance(double Xfinal,double Yfinal){
    return(sqrt( (Xfinal - _x) * (Xfinal - _x) + (Yfinal - _y) * (Yfinal - _y) ) );
}

double Camera::get_x(){
    return(_x);
}

double Camera::get_y(){
    return(_y);
}

double Camera::get_nX(){
    return(_nX);
}

double Camera::get_nY(){
    return(_nY);
}