#include "parser.h"

void Parser::lights(Lights lights, char* input){
  // Takes an input char* and converts it into values for turning on and off LED's
  // Example input LR&I:50&F:20000 (Light Red Intensity:50 bits Frequency: 20000Hz
  // TODO convert intensity to a percentage instead of bits

  // Checks what color we want to control
  bool red = false, green = false, blue = false;
  switch(input[1]){
    case 'R':
      red = true;
      break;
    case 'G':
      green = true;
      break;
    case 'B':
      blue = true;
      break;
  }

  // Splits the input and updates the LED's 
  char* command  = strtok(input, "&");
  while(command != NULL){
    char* sep = strchr(command, ':');
    if(sep != 0){
      int value = atoi(++sep);
      if(command[0] == 'I'){
        if(red){
          lights.set_intensity("Red", value);
          lights.on("Red");
        }
        else if(green){
          lights.set_intensity("Green", value);
          lights.on("Green");
        }
        else if(blue){
          lights.set_intensity("Blue", value);
          lights.on("Blue");
        }
      }
      else if(command[0] == 'F'){
        if(red){
          lights.set_frequency("Red", value);
          lights.on("Red");
        }
        else if(green){
          lights.set_frequency("Green", value);
          lights.on("Green");
        }
        else if(blue){
          lights.set_frequency("Blue", value);
          lights.on("Blue");
        }
      }
    }
    command = strtok(NULL, "&");
  }
}
