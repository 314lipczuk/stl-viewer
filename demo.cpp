#include "Engine.h"
#include <cstdlib>
#include <csignal>
#include <ctime>
int main(){//Every run of this shows a random model out of 4 : taurus, cube, tetrohedron, and some random model from my tinkercad. You can also load your own model by calling loadStl() and having file.stl in this folder.
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::system_clock;
    using namespace Engine;
    Engine::buffer b(150,70);
    loadStl("acube.stl");
    //centerShape();
    resizeForBuffer(b.offsety);
    bool tooBigForLines = false;
    if(Engine::point::all_points.size()>500) tooBigForLines = true;
    for(;;){ // If nothing shows you might want to start it again, it does that sometimes
      b.clear();
      /*
      for(int x=0;x<line::all_lines.size();x++){
          b.plotline(*line::all_lines[x]);
      }
      */
      
    if(tooBigForLines){
      for(int x=0;x<point::all_points.size();x++){
          b.plot((*point::all_points[x]));
      }
    }
      b.render_basic();
      for(int x=0;x<point::all_points.size();x++){
          point::all_points[x]->rotate(1, 3.14/30);
          point::all_points[x]->rotate(2, 3.14/40);
      //    std::cout<<point::all_points[x]->x<<"x "<<point::all_points[x]->y<<"y "<<point::all_points[x]->z<<"z\n";
      }

      sleep_for(50ms);
      system("clear");
    }

}
