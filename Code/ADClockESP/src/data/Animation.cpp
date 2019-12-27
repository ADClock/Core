#include "Animation.h"

Animation::Animation(String name)
{
  // Zur Sicherheit... (Tritt auf bei Start des Mikrocontrollers)
  if (name.isEmpty())
  {
    this->no_more_step = true;
    return;
  }

  Serial.println("Animation: Loading '" + name + "'");
  auto file = SPIFFS.open("/animations/" + name + ".ani");
  if (!file)
  {
    this->no_more_step = true;
    Serial.println("Error while opening file... :(");
    return;
  }
  this->data = file.readString();
  this->data_pos = 0;
  Serial.println("Animation String '" + this->data + "'");
  file.close();

  if (this->data.isEmpty())
  {
    Serial.println("Es sind keine Daten für diese Animation hinterlegt.");
    this->no_more_step = true;
    return;
  }

  this->no_more_step = false;
  this->reset_animation();
  this->load_next_step();
}

void Animation::tick()
{
  // TODO Cleanup
  if (this->no_more_step)
    return;

  // If current Step is a delay we wait...
  if (this->current_step.type == StepType::DELAY)
  {
    if ((unsigned long)(millis() - this->last_load) < this->current_step.value)
    {
      return;
    }
  }

  if (this->current_step.type == StepType::WAIT_UNTIL_PLAN_SEND)
  {
    if (_manager.hasPendingPlan())
      return;
  }

  if (this->current_step.type == StepType::WAIT_UNTIL_CLOCKS_STAND_STILL)
  {
    if (_manager.hasPendingMoves())
      return;
  }

  this->execute_step();
  this->load_next_step();
}

bool Animation::has_pending_step()
{
  // TODO Cleanup
  return !no_more_step;
}

void Animation::reset_animation()
{
  this->x = 0;
  this->y = 0;
  this->hand = HandSelection::BOTH;
  this->clock = ClockSelection::ALL;
  this->data_pos = 0;
}

void Animation::load_next_step()
{

  if (this->data_pos >= this->data.length())
  {
    Serial.println("There are no more steps in this animation!");
    no_more_step = true;
    return;
  }

  String step_string = String("");

  while (this->data_pos < this->data.length())
  {
    if (this->data.charAt(data_pos) == ';')
    {
      this->data_pos++;
      break;
    }

    step_string += this->data.charAt(data_pos);
    this->data_pos++;
  }

  if (step_string.isEmpty())
  {
    Serial.println("[Animation] Empty StepString in file!");
    return;
  }

  this->set_current_step(step_string);
  this->last_load = millis();
}

void Animation::set_current_step(String &step_string)
{
  this->current_step = {static_cast<StepType>(step_string.substring(0, step_string.indexOf("/")).toInt()),
                        (unsigned int)step_string.substring(step_string.indexOf("/") + 1).toInt()};
}

void Animation::execute_step()
{
  Serial.println("[Animation] Executing Step.. type=" + String(this->current_step.type) + " value=" + String(this->current_step.value));

  switch (this->current_step.type)
  {
  case INIT:
    _manager.init();
    Serial.println("Executed init..");
    break;

  case REPEAT:
    this->reset_animation();
    break;

  case SELECT_CLOCK:
    this->clock = static_cast<ClockSelection>(this->current_step.value);
    break;

  case SELECT_X:
    this->x = this->current_step.value;
    break;

  case SELECT_Y:
    this->x = this->current_step.value;
    break;

  case SELECT_HAND:
    this->hand = static_cast<HandSelection>(this->current_step.value);
    break;

  case TARGET_POS:
    if (this->clock == ALL)
      planned.set_position(this->hand, this->current_step.value);
    else
      planned.set_position(this->x, this->y, this->hand, this->current_step.value);
    break;

  case DIRECTION:
    if (this->clock == ALL)
      planned.set_direction(this->hand, this->current_step.value);
    else
      planned.set_direction(this->x, this->y, this->hand, this->current_step.value);
    break;

  case STEP_DELAY:
    if (this->clock == ALL)
      planned.set_step_delay(this->hand, this->current_step.value);
    else
      planned.set_step_delay(this->x, this->y, this->hand, this->current_step.value);
    break;

  case WAIT_STEPS:
    if (this->clock == ALL)
      planned.set_wait_degree(this->hand, this->current_step.value);
    else
      planned.set_wait_degree(this->x, this->y, this->hand, this->current_step.value);
    break;

  case MOVE_IN_SYNC:
    // TODO
    break;

  case SHOW_CURRENT_TIME_ON_EACH:
    if (!_time.is_time_set())
    {
      _time.load_time();
    }
    planned.show_time(_time.get_hour(), _time.get_minute());
    break;

  case SHOW_CURRENT_TIME:
    // TODO
    break;

  default:
    break;
  }
}
