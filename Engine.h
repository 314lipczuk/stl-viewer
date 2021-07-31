#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <thread>
//#include "stl_reader.h"
#include <csignal>
#include <fstream>
#include "stupidpars.h"
/* 
Sources for:
-understanding math behind rotations of points and mapping 3D to 2D: https://www.a1k0n.net/2011/07/20/donut-math.html and https://www.a1k0n.net/2021/01/13/optimizing-donut.html
-understanding STL file format: https://all3dp.com/what-is-stl-file-format-extension-3d-printing/
- header file that parses STL files into points and lines: https://github.com/sreiter/stl_reader

============
TODO:
	* figure out optimization for loading stl, sth is fucked up there
	* add sfml support
*/

namespace Engine{
using Clock = std::chrono::steady_clock;
struct point{
    double x;
    double y;
    double z;
    void rotate(int axisVar, double degrees);
    point(double x, double y, double z);
    static bool alreadyExists(double x, double y, double z);
    static point& find(double x, double y, double z);
    static std::vector<point*> all_points;
    };
std::vector<point*> point::all_points;

point& point::find(double x, double y, double z){
  for(int p=0;p<all_points.size();p++){
    if(all_points[p]->x == x &&all_points[p]->y == y && all_points[p]->z == z){
              return *all_points[p];
          }
      }
  }

bool point::alreadyExists(double x, double y, double z){
    for(int p=0;p<all_points.size();p++){
        if(all_points[p]->x == x &&all_points[p]->y == y && all_points[p]->z == z){
            return true;
        }
    }
    return false;
}
void point::rotate(int axisVar, double degrees){
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
point::point(double x, double y, double z){
   this->x=x; 
   this->y=y; 
   this->z=z; 
   all_points.push_back(this);
    }
struct line{
    point* p1;
    point* p2;
    double normx,normy,normz;
    line(point* p1, point* p2);
    line(point* p1, point* p2,double nx,double ny, double nz);
    static std::vector<line*> all_lines;
    void draw_all();
    };

 line::line(point* p1, point* p2,double nx,double ny, double nz){

    this->p1 = p1;
    this->p2 = p2;
    this->normx = nx;
    this->normy = ny;
    this->normz = nz;
    all_lines.push_back(this);
 }
std::vector<line*> line::all_lines;
line::line(point* p1, point* p2){
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

void loadStl(std::string name){ //using external library to parse STL file into my own points and lines. Doesn't bother to clean the 'support lines' from stl format and because of that is a bit dirty to look at
        std::vector<double> coords;
	auto tic = Clock::now();
        int tris;
        stupidpars::parse_stl(name, coords, tris);
        point *pts[3];
	int indexnum;
        for(int i=0;i<tris;i++){
	
          for(int j=0;j<3;j++){ //corners

             if(point::alreadyExists(coords[i*9+3*j],coords[i*9+3*j+1],coords[i*9+3*j+2])){
                 pts[j] = &(point::find(coords[i*9+3*j],coords[i*9+3*j+1],coords[i*9+3*j+2]));
             }else{
                 pts[j] = new Engine::point(coords[i*9+3*j],coords[i*9+3*j + 1],coords[i*9+3*j + 2]);
             }
        }
          /* Version for SFML, with triangle class;
        new triangle(pts[0],pts[1],pts[2]);
        */
        //version without triangle class, for barebones ascii
        new line(pts[0],pts[1]);
        new line(pts[0],pts[2]);
        new line(pts[1],pts[2]);

        }
	auto toc= Clock::now();
	std::cout << "Engine.h -> load_stl() 	Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(toc - tic).count() << std::endl;
}
        /*
          const size_t numTris = tris.size() / 3;
          for(int itri = 0; itri < numTris; ++itri) {
            for(int icorner = 0; icorner < 3; ++icorner) {
                double* c = &coords[3 * tris [3 * itri + icorner]];
                new point(c[0],c[1],c[2]);
            }
            std::vector<point*>::iterator itr = point::all_points.end();
            new line(*(itr-1),*(itr-2));
            new line(*(itr-3),*(itr-2));
            new line(*(itr-1),*(itr-3));
          }
        }
        catch (std::exception& e) {
          std::cout << e.what() << std::endl;
        }
    }
    */

void centerShape(){
    double averagex,averagey,averagez;
    for(int i=0;i<point::all_points.size();i++){
        averagex+=point::all_points[i]->x;
        averagey+=point::all_points[i]->y;
        averagez+=point::all_points[i]->z;
    }
    averagex = averagex / point::all_points.size();
    averagey = averagey / point::all_points.size();
    averagez = averagez / point::all_points.size();

    for(int i=0;i<point::all_points.size();i++){
        point::all_points[i]->x = point::all_points[i]->x - averagex;
        point::all_points[i]->y = point::all_points[i]->y - averagey;
        point::all_points[i]->z = point::all_points[i]->z - averagez;
    }
}

void scaleShape(double s_factor){
    for(int i=0;i<point::all_points.size();i++){
       point::all_points[i]->x = point::all_points[i]->x * s_factor;
       point::all_points[i]->y = point::all_points[i]->y * s_factor;
       point::all_points[i]->z = point::all_points[i]->z * s_factor;
       }
}

void moveShape(int x, int y, int z){
    for(int i=0;i<point::all_points.size();i++){
       point::all_points[i]->x = point::all_points[i]->x + x;
       point::all_points[i]->y = point::all_points[i]->y + y;
       point::all_points[i]->z = point::all_points[i]->z + z;
    }
}
void resizeForBuffer(int bufsiz){
    double max_score =0;
    for(int i=0;i<point::all_points.size();i++){
        double iLen=std::sqrt(std::pow(point::all_points[i]->x, 2)+ std::pow(point::all_points[i]->y, 2) + std::pow(point::all_points[i]->z, 2));
        if(iLen>max_score) max_score = iLen;
    }
    scaleShape( (bufsiz-1) / (max_score) );
}
void clearMem(){
    line::all_lines.clear();
    point::all_points.clear();
}
void screenshotSignal(int num){ //Acutally i saw the screenshot requirement 2 hours before project end so its a bit of a hack, not sure if makes any sense and not sure if works
    std::cout<<"Taking a screenshot!";
    std::ofstream screenshot;
    screenshot.open("SC.txt");
    buffer sc(150,70);
    if(point::all_points.size()<500){
    for(int i=0;i<line::all_lines.size();i++){
        sc.plotline(*line::all_lines[i]);
    }
    }else{
    for(int i=0;i<point::all_points.size();i++){
        sc.plot(*point::all_points[i]);
    }}

    for(int i=0;i<sc.height;++i){
        for(int j=0;j<sc.width;++j){
            screenshot<<sc.data[i][j];
        }
        screenshot<<'\n';
    }
    screenshot.close();
    exit(num);
    }

}
