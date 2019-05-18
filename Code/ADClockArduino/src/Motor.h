#include "Arduino.h"

#define MAX_STEPS 1050

class Motor
{
public:
  Motor(size_t pin1, size_t pin2, size_t pin3, size_t pin4);
  void step();
  void set_target_pos(size_t pos);
  void try_step();
  void reset_position();

private:
  size_t pin1;
  size_t pin2;
  size_t pin3;
  size_t pin4;
  size_t current_pos;
  size_t target_pos;
  unsigned long last_step;
  size_t step_delay;
  size_t coil_state;
};