#include "Engine.h"
#include <cstdlib>
#include <csignal>
#include <ctime>
int main(){
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::system_clock;
    using namespace Engine;
    Engine::buffer b(150,70);
    loadStl("acube.stl");
    centerShape();
    resizeForBuffer(b.offsety);
    bool tooBigForLines = false;
    if(Engine::point::all_points.size()>500) tooBigForLines = true;
    for(;;){ // If nothing shows you might want to start it again, it does that sometimes
      b.clear();

      for(int x=0;x<line::all_lines.size();x++){
          b.plotline(*line::all_lines[x]);
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
      //    std::cout<<point::all_points[x]->x<<"x "<<point::all_points[x]->y<<"y "<<point::all_points[x]->z<<"z\n";
      }
      sleep_for(150ms);
      system("clear");
    }
}
