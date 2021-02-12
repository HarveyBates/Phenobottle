#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "lights.h"

#define MAX_INPUT_SIZE 20

class Parser{

  public:
    void lights(Lights lights, char* cmd);
    
};


#endif
