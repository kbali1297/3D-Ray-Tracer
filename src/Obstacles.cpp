#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include<chrono>
#include<ctime>
#include"Camera.h"
#include"Pixel.h"
#include"Obstacles.h"

Square::Square(double center_x,double center_y,double side)
:_center_x(center_x),_center_y(center_y),_side(side){
}

Wall::Wall(double v,char orientation){
        _orientation = orientation;//Horizontal or Vertical Wall
        switch (_orientation)
        {
        case 'H':
            _Y = v;
            break;
        
        default:
            _X = v;
        }
        /*
        if(_orientation=='H')
            _Y = v;
        else if(_orientation=='V')
            _X = v;*/
}

Circle::Circle(double center_x,double center_y,double radius)
:_center_x(center_x),_center_y(center_y),_radius(radius){

}

//Returns bool ---> intersection successful true, else false
//Returns double ----> intersection parameter t
std::pair<bool,double> Square::Compute_Intersection(Camera e,Pixel p){
    
    bool check_intersect = false;
    
    double t = 0;
    std::vector<double> sidex(2),sidey(2);
    sidex[0] = _center_x - _side/2.;
    sidex[1] = _center_x + _side/2.;
    
    sidey[0] = _center_y - _side/2.;
    sidey[1] = _center_y + _side/2.;

    double temp;
    if(sidex[0]>sidex[1]){
        temp = sidex[1];
        sidex[1] = sidex[0];
        sidex[0] = temp;
    }
    if(sidey[0]>sidey[1]){
        temp = sidey[1];
        sidey[1] = sidey[0];
        sidey[0] = temp;
    }    

    double intersection_x,intersection_y;
    double intersection_distance;

    double ex = e.get_x();
    double ey = e.get_y();
    double px = p.get_x();
    double py = p.get_y();

    double min_dist=1e6;
    double min_t;
    for(int i=0;i<sidex.size();i++){
        t = sidex[i] - ex;
        t/= (px - ex);
        intersection_x = ex + t*(px - ex);
        intersection_y = ey + t*(py - ey);
        //Check if Object lies in front of the screen
        if( (intersection_x - ex) * (px - ex) + (intersection_y - ey) * (py - ey) > 0){
            //Check if Point of intersection lies on the square
            if ( (intersection_y <= sidey[1]) && (intersection_y >= sidey[0]) ) { 
                check_intersect = true;
                intersection_distance = e.distance(intersection_x,intersection_y);
                if(min_dist > intersection_distance){
                    min_dist = intersection_distance;
                    min_t = t;
                }
            }
        }
    }
    for(int i=0;i<sidey.size();i++){
        t = sidey[i] - ey;
        t/= (py - ey);
        intersection_x = ex + t*(px - ex);
        intersection_y = ey + t*(py - ey);
        //Check if Object lies in front of the screen
        if( (intersection_x - ex) * (px - ex) + (intersection_y - ey) * (py - ey) > 0){
            //Check if Point of intersection lies on the square
            if( (intersection_x <= sidex[1]) && (intersection_x >= sidex[0]) ){
                check_intersect = true;
                intersection_distance = e.distance(intersection_x,intersection_y);
                if(min_dist > intersection_distance){
                    min_dist = intersection_distance;
                    min_t = t;
                }
            }
        }
    }
    std::pair<bool,double> intersect_pair;
    intersect_pair.first = check_intersect;
    intersect_pair.second = min_t;
    return(intersect_pair);
}

//Returns bool ---> intersection successful true, else false
//Returns double ----> intersection parameter t
std::pair<bool,double> Wall::Compute_Intersection(Camera e,Pixel p){
    double t = 0;
    double intersection_x;
    double intersection_y;

    double ex = e.get_x();
    double ey = e.get_y();
    double px = p.get_x();
    double py = p.get_y();

    bool check_intersect = false;
    switch (_orientation)
    {
    case 'H':
        t = _Y - ey;
        t/= (py - ey);
        intersection_x = ex + t * (px - ex);
        intersection_y = ey + t * (py - ey);
        if( (intersection_x - ex) * (px - ex) + (intersection_y - ey) * (py - ey) > 0)
            check_intersect = true;
        break;
    
    case 'V':
        t = _X - ex;
        t/= (px - ex);
        intersection_x = ex + t * (px - ex);
        intersection_y = ey + t * (py - ey);
        if( (intersection_x - ex) * (px - ex) + (intersection_y - ey) * (py - ey) > 0)
            check_intersect = true;
    }
    /*
    if(_orientation=='H'){
        t = _Y - e.get_y();
        t/= (p.get_y() - e.get_y());
        double intersection_x = e.get_x() + t * (p.get_x() - e.get_x());
        double intersection_y = e.get_y() + t * (p.get_y() - e.get_y());
        if( (intersection_x - e.get_x()) * (p.get_x() - e.get_x()) + (intersection_y - e.get_y()) * (p.get_y() - e.get_y()) > 0)
            check_intersect = true;
    }
    else if(_orientation=='V'){
        t = _X - e.get_x();
        t/= (p.get_x() - e.get_x());
        double intersection_x = e.get_x() + t * (p.get_x() - e.get_x());
        double intersection_y = e.get_y() + t * (p.get_y() - e.get_y());
        if( (intersection_x - e.get_x()) * (p.get_x() - e.get_x()) + (intersection_y - e.get_y()) * (p.get_y() - e.get_y()) > 0)
            check_intersect = true;
    }*/
    std::pair<bool,double> intersect_pair;
    intersect_pair.first = check_intersect;
    intersect_pair.second = t;
    return(intersect_pair);
}

//Returns bool ---> intersection successful true, else false
//Returns double ----> intersection parameter t
std::pair<bool,double> Circle::Compute_Intersection(Camera e,Pixel p){
    
    bool check_intersect = false;
    double min_t = 0;

    double t;

    double D1 = (_center_x - e.get_x())*(p.get_x() - e.get_x()) + (_center_y - e.get_y())*(p.get_y() - e.get_y()); //(center_v - camera_v).(pixel_v - camera_v)
    double D2 = (_center_x - e.get_x())*(_center_x - e.get_x()) + (_center_y - e.get_y())*(_center_y - e.get_y()); //|center_v - camera_v|^2
    double D3 = (p.get_x() - e.get_x())*(p.get_x() - e.get_x()) + (p.get_y() - e.get_y())*(p.get_y() - e.get_y()); //|pixel_v - camera_v|^2

    double Discriminant = D1*D1 - D3*(D2 - _radius*_radius);

    double ex = e.get_x();
    double ey = e.get_y();
    double px = p.get_x();
    double py = p.get_y();

    if(Discriminant>=0){// Check if Intersection between ray and obstacle possible

        std::vector<double> t(2);
        t[0] = D1 - sqrt(Discriminant);
        t[0]/= D3;

        t[1] = D1 + sqrt(Discriminant);
        t[1]/= D3;

        double intersection_x, intersection_y;
        double intersection_distance;
        double min_dist = 1e6;
        for(int i=0;i<t.size();i++){
            intersection_x = ex + t[i]*(px - ex);
            intersection_y = ey + t[i]*(py - ey);
            //point of intersection should lie in front of the screen
            if( (intersection_x - ex) * (px - ex) + (intersection_y - ey) * (py - ey) > 0){
                check_intersect = true;
                intersection_distance = e.distance(intersection_x,intersection_y);
                if(min_dist>intersection_distance){
                    min_dist = intersection_distance;
                    min_t = t[i];
                }
            }
        }
    }
    std::pair<bool,double> intersect_pair;
    intersect_pair.first = check_intersect;
    intersect_pair.second = min_t;
    return(intersect_pair);
}

char Square::Obstacle_type(){
    return('S');
}

char Wall::Obstacle_type(){
    return('W');
}

char Circle::Obstacle_type(){
    return('C');
}