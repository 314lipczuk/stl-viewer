#include "Engine.h"
#include <chrono>
#include <cstdlib>
#include <csignal>
#include <ctime>
int main()
  {
    Engine::buffer b(150,70);
    Engine::loadStl("cube.stl");
    Engine::centerShape();
    //Engine::scaleShape(3);
    //Engine::moveShape(0, 50, 50);
    Engine::resizeForBuffer(b.offsety);
    bool tooBigForLines = false;
    if(Engine::point::all_points.size()>500) tooBigForLines = true;
    for(;;){
      b.clear();
      for(int x=0;x<Engine::line::all_lines.size();x++){
          b.plotline(*Engine::line::all_lines[x]);
      }
    if(tooBigForLines){
      for(auto itr = Engine::point::all_points.begin();itr != Engine::point::all_points.end();++itr){
          b.plot((*(*itr)));
      }
    }
      b.render_basic();
      for(auto itr = Engine::point::all_points.begin();itr != Engine::point::all_points.end();++itr){
         (*itr)->rotate(1, 3.14/30);
         (*itr)->rotate(2, 3.14/40);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
      Engine::clearScreen();
    }
  }
