#ifndef _OBSTACLES_
#define _OBSTACLES_
#include<utility>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>
#include"Camera.h"
#include"Pixel.h"


class Obstacle{
    public:
    virtual std::pair<bool,double> Compute_Intersection(Camera,Pixel)=0;
    virtual char Obstacle_type()=0;
    virtual ~Obstacle(){}
};
class Square : public Obstacle{
    private:
    double _center_x,_center_y; //Center Coordinates of Square
    double _side; //Side Length of Square

    public:
    Square(double,double,double);
    std::pair<bool,double> Compute_Intersection(Camera,Pixel) override;
    char Obstacle_type() override;
};

class Wall : public Obstacle{
    private:
    double _X,_Y; //Horizontal or vertical Walls
    char _orientation;

    public:
    Wall(double,char);
    std::pair<bool,double> Compute_Intersection(Camera,Pixel) override;
    char Obstacle_type() override;
};

class Circle : public Obstacle{
    private:
    double _center_x,_center_y;
    double _radius;

    public:
    Circle(double,double,double);
    std::pair<bool,double> Compute_Intersection(Camera,Pixel) override;
    char Obstacle_type() override; 
};
#endif