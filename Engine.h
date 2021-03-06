#include <boost/container_hash/extensions.hpp>
#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <thread>
#include "boost/unordered_set.hpp"
#include "boost/functional/hash.hpp"
#include <csignal>
#include <fstream>
#include "stupidpars.h"
/* 
Sources for:
-understanding math behind rotations of points and mapping 3D to 2D: https://www.a1k0n.net/2011/07/20/donut-math.html and https://www.a1k0n.net/2021/01/13/optimizing-donut.html
-understanding STL file format: https://all3dp.com/what-is-stl-file-format-extension-3d-printing/

============
TODO:
	* add sfml support
*/

namespace Engine{
void clearScreen()
  {
    #ifdef __linux__ 
        std::system("clear");
    #elif _WIN32
        std::system("cls");
    #endif

  }
using Clock = std::chrono::steady_clock;
struct point
  {
    public:
     double x;
     double y;
     double z;
     void rotate(int axisVar, double degrees);
     point(double x, double y, double z);
     static point& find(double x, double y, double z);
     static boost::unordered::unordered_set<point*> all_points;
     static void add(point);
     bool operator==(point &spoint)
      {
	       return (x == spoint.x && y == spoint.y && z == spoint.z);
     	}
  };

size_t hash_value(const point &p)
  {
    size_t seed =0;
    boost::hash_combine(seed, p.x);
    boost::hash_combine(seed, p.y);
    boost::hash_combine(seed, p.z);
    return seed;
  }
boost::unordered_set<point*> point::all_points;
void point::rotate(int axisVar, double degrees)
  {
    double cos = std::cos(degrees);
    double sin= std::sin(degrees);
    double x = this->x;
    double y = this->y;
    double z = this->z;
    switch(axisVar){
        case 1: // Clockwise rotation around Z axis
            this->x = (x * cos) - (y*sin);       
            this->y = (x * sin) + (y * cos);
            break;
        case 2: //Clockwise rotation in plane around Y axis
            this->z = (z*cos) - (x*sin);
            this->x = (z*sin) + (x*cos);
            break;
        case 3:// rotation around X axis
            this->z = (z*cos) - (y*sin);
            this->y = (z*sin) + (y*cos);
            break;
      }
    }
point::point(double x, double y, double z)
  {
   this->x=x; 
   this->y=y; 
   this->z=z; 
   all_points.insert(this);
  }
struct line
  {
    point* p1;
    point* p2;
    double normx,normy,normz;
    line(point* p1, point* p2);
    line(point* p1, point* p2,double nx,double ny, double nz);
    static std::vector<line*> all_lines;
    void draw_all();
  };

line::line(point* p1, point* p2,double nx,double ny, double nz)
  {
    this->p1 = p1;
    this->p2 = p2;
    this->normx = nx;
    this->normy = ny;
    this->normz = nz;
    all_lines.push_back(this);
 }
std::vector<line*> line::all_lines;
line::line(point* p1, point* p2)
  {
    this->p1 = p1;
    this->p2 = p2;
    all_lines.push_back(this);
  }
struct buffer 
    {
    char** data;
    int width;
    int height;
    int offsetx;
    int offsety;
    buffer(const int, const int);
    ~buffer();
    void render_basic();
    void plotline( point p1, point p2);
    void plotline(line l);
    void plotlineB(point p1, point p2);
    void render_advanced();
    void plot(point p);
    void plot(int x, int y);
    void clear();
    void setOffset(int offx, int offy);
    };
buffer::~buffer(){
    int arrSize = sizeof(this->data)/sizeof(this->data[0]);
    for(int x=0;x<arrSize;x++){
        delete[] this->data[x];
    }
    delete[] this->data;
    }
buffer::buffer(int W, int H){
    this->height = H;
    this->width = W;
    this->data = new char* [H];
    for(int x=0;x<H;++x){
        this->data[x] = new char[W];
        for(int xx=0;xx<W;xx++){
            this->data[x][xx] = ' ';
        }
    }
    setOffset(W/2, H/2);
    }

void buffer::clear()
    {
    for(int y=0;y<this->height;++y){
        for(int x=0;x<this->width;x++){
            this->data[y][x] = ' ';
        }
    }
    }

void buffer::render_basic(){
    for(int i=0;i<this->height;++i){
        for(int j=0;j<this->width;++j){
            std::cout<< this->data[i][j];
        }
        std::cout<<'\n';
    }
    }
void buffer::setOffset(int offx, int offy)
    {
        this->offsetx = offx;
        this->offsety = offy;
    }

void buffer::plot(int x, int y)
    {
        if(y<offsety && y > -1*offsety && x > -1*offsetx && x < offsetx){
        data[y +offsety][x + offsetx] = '#';
        }else
            std::cout<<"plotfail";
    }
void buffer::plot(point p)
    {
        if(p.y<offsety && p.y > -1*offsety && p.x > -1*offsetx && p.x < offsetx){
        data[(int)p.y +offsety][(int)p.x + offsetx] = '#';
        }else{
            std::cout<<"plotfail";
        }
    }

void buffer::plotline(point p1, point p2)
    { 
        double deltax,deltay;
        if(p1.x>p2.x){
            point tmp = p1;
            p1 = p2;
            p2 = tmp;
        }
        deltax = p2.x-p1.x;
        deltay = p2.y-p1.y;

	    if((int)p1.x == (int)p2.x)
            {
            if(p1.y>p2.y){
            point tmp = p1;
            p1 = p2;
            p2 = tmp;
            }
		    for(int y=p1.y;y<p2.y;y++){
		    	plot(p1.x,y);		
		    }	
		    return;
	       }
	    double delta_error = deltay / deltax;
	    if (delta_error < 0) delta_error = -delta_error;
	    double error = 0;
        if(delta_error<1){
	    int y = p1.y;
	    for (int x = p1.x; x <= p2.x; ++x)
	    {
	    	plot(x, y);
	    	error += delta_error;
	    	if (error >= 0.5)
	    	{
	    		if (deltay > 0) ++y;
	    		else --y;
	    		--error;
	    	}
	    }}else{
            delta_error = (1/delta_error);
            int wsp = 1;
            if(p1.y>p2.y){                    
                wsp=-1;
            }
            int x=p1.x;
            for(int y=(int)p1.y;std::round(y)!=std::round(p2.y);y+=wsp){
                plot(x,y);
                error += delta_error;
                if(error >= 0.5){
                    if(deltax > 0) ++x;
                    else --x;
                    --error;
                }
            }
        }
    }

 
void buffer::plotline(line l){
    this->plotline(*(l.p1),*(l.p2));
    }

void loadStl(std::string name)
  { 
        std::vector<float> coords;
	      auto tic = Clock::now();
        int tris=0;
        stupidpars::parse_stl(name, coords, tris);
        point *pts[3];
        for(int i=0;i<tris;i++){
          for(int j=0;j<3;j++){
                 pts[j] = new Engine::point(coords[i*9+3*j],coords[i*9+3*j + 1],coords[i*9+3*j + 2]);
        }
        /* Version for SFML, with triangle class;
          new triangle(pts[0],pts[1],pts[2]);
        */
        new line(pts[0],pts[1]);
        new line(pts[0],pts[2]);
        new line(pts[1],pts[2]);
        }
	auto toc= Clock::now();
	std::cout << "Engine.h -> load_stl() 	Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(toc - tic).count() <<"\tAll lines size: "<<line::all_lines.size()<<"\t All points size: "<<Engine::point::all_points.size() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
}


void centerShape()
  {
    double averagex= 0,averagey = 0,averagez = 0;
    for(auto itr = point::all_points.begin();itr!=point::all_points.end();++itr){
        averagex+= (*itr)->x;
        averagey+=(*itr)->y;
        averagez+=(*itr)->z;
    }
    averagex = averagex / point::all_points.size();
    averagey = averagey / point::all_points.size();
    averagez = averagez / point::all_points.size();

    for(auto itr = point::all_points.begin();itr!=point::all_points.end();++itr){
        (*itr)->x = (*itr)->x - averagex;
        (*itr)->y = (*itr)->y - averagey;
        (*itr)->z = (*itr)->z - averagez;
    }
  }

void scaleShape(double s_factor)
  {
    for(auto itr = point::all_points.begin();itr!=point::all_points.end();++itr){
       (*itr)->x = (*itr)->x * s_factor;
       (*itr)->y = (*itr)->y * s_factor;
       (*itr)->z = (*itr)->z * s_factor;
       }
  }

void moveShape(double x, double y, double z)
  {
    for(auto itr = point::all_points.begin();itr!=point::all_points.end();++itr){
       (*itr)->x = (*itr)->x + x;
       (*itr)->y = (*itr)->y + y;
       (*itr)->z = (*itr)->z + z;
    }
  }
void resizeForBuffer(int bufsiz)
  {
    double max_score =0;
    for(auto itr = point::all_points.begin();itr!=point::all_points.end();++itr){
        double iLen=std::sqrt(std::pow((*itr)->x, 2)+ std::pow((*itr)->y, 2) + std::pow((*itr)->z, 2));
        if(iLen>max_score) max_score = iLen;
    }
    scaleShape( (bufsiz-1) / (max_score) );
  }
void clearMem()
  {
    line::all_lines.clear();
    point::all_points.clear();
  }
}
