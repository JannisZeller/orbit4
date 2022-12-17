#ifndef STATE_H
#define STATE_H

#include <math.h>
#include <iostream>
#include "vec3D.h"

class State {

// Everything is public. Nothing needs to be hidden here.
  public:
    vec3D position;
    vec3D velocity;
    
};

#endif