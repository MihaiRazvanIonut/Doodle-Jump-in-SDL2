#include "MyFramework.h"
#include <string>
#include <iostream>

void windowInit(int argc, char *argv[], int &windowWidth, int &windowHeight, bool &fullscreen);


int main(int argc, char *argv[])
{

    int windowWidth=1920, windowHeight=1080;
    bool fullscreen=true;
    windowInit(argc, argv, windowWidth, windowHeight, fullscreen);
    return run(new MyFramework(windowWidth, windowHeight, fullscreen));
}

void windowInit(int argc, char *argv[], int &windowWidth, int &windowHeight, bool &fullscreen){

    if (argc==3){
        std::string windowParam(argv[1]);
        if (windowParam=="-window") {
            std::string windowSize(argv[2]);
            size_t xPos = windowSize.find('x');
            if (xPos != std::string::npos){
                windowWidth=atoi(windowSize.substr(0, xPos).c_str());
                windowHeight=atoi(windowSize.substr(xPos+1).c_str());

            }
            else{
                std::cout<<"Invalid window size format."<< std::endl;
                exit(0);
            }
        }
    }
    else
        if (argc==2){
            std::string windowParam(argv[1]);
            if (windowParam=="-fullscreen"){
                fullscreen=true;
                windowWidth=1920;
                windowHeight=1080;
            }

        }
    else if (argc>3){
        std::cout<<"Usage: ./Doodle_Jump -window <width>x<height> OR ./Doodle_Jump OR ./Doodle_Jump -fullscreen\n";
        exit(0);

    }


}

/*
 *
 *
  X Error of failed request:  BadValue (integer parameter out of range for operation)
  Major opcode of failed request:  140 (RANDR)
  Minor opcode of failed request:  7 (RRSetScreenSize)
  Value in failed request:  0xb4
  Serial number of failed request:  328
  Current serial number in output stream:  329
 *
 *
 *
 *
 */