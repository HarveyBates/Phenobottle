#ifndef PARSER_H
#define PARSER_H

#include "Arduino.h"
#include "lights.h"

#define MAX_INPUT_SIZE 20 

/**
 * Functions that parse incoming charater arrays into usable values.
 * 
 * @param MAX_INPUT_SIZE Maximum size of char* that can be processed by the serial port.
 */
class Parser{

  public:
    void lights(Lights lights, char* input);
    
};


#endif
