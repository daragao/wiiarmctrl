cmake_minimum_required(VERSION 2.6)
  if (COMPILE_DEVICE_LIBRARY)
    # Declare a device called "WiimoteDevice", with C++ type 
    # "wiimoteController" defined in wiimoteController.h.  This code sets
    # up a CMake option for selecting the device, and prepares
    # some initialization boilerplate code if needed.
    # If the device is not selected, "SKIP_foo" will be defined.
    PREPARE_DEVICE(WiimoteDevice TYPE wiimoteController INCLUDE wiimoteController.h)
  endif (COMPILE_DEVICE_LIBRARY)
#  if (NOT SKIP_WiimoteDevice)
    # Find any libraries needed - just YARP for this example
    find_package(YARP REQUIRED)
    include_directories(${YARP_INCLUDE_DIRS} wiiuse)
    # ... find and use other needed libraries ...
    add_library(WiimoteDevice wiimoteController.cpp wiimoteController.h wiiuse)
    target_link_libraries(WiimoteDevice ${YARP_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR}/wiiuse/wiiuse.lib )
	
	set_target_properties(WiimoteDevice PROPERTIES LINK_INTERFACE_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/wiiuse/wiiuse.dll)
  
    if (COMPILE_DEVICE_LIBRARY)
      add_executable(testWiimoteDevice testWiimoteDevice.cpp)
      target_link_libraries(testWiimoteDevice WiimoteDevice)
    endif (COMPILE_DEVICE_LIBRARY)
#  endif (NOT SKIP_WiimoteDevice)