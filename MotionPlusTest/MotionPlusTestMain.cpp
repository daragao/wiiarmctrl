#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include "Gui.h"

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

//YARP_DECLARE_PLUGINS(DeviceTestPlugin);

int main(int argc, char *argv[]) {
  Network yarp;
  //YARP_REGISTER_PLUGINS(DeviceTestPlugin);
  PolyDriver dd("WiimoteDevice");
  if (!dd.isValid()) {
    printf("wiimoteControllerName not available\n");
    return 1;
  }
  
  printf("Was able to instantiate a wiimoteControllerName\n");
  IWiimote *wiiSensor;
  Vector wiiData;
  dd.view(wiiSensor);
  Gui gui(wiiSensor);
  gui.gladeCartesianGui();
  return 0;
}