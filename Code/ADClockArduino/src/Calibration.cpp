#include "Calibration.h"

Calibration::Calibration(Motor &m, size_t hall_pin) : motor(m), hall_pin(hall_pin)
{
  pinMode(hall_pin, INPUT_PULLUP);
}

void Calibration::start_calibration()
{
  this->calibrated = false;
  this->field_read = false;
  this->should_go_outside = false;
  this->was_in_field = false;
  this->ouside_steps = 0;
  this->calibrated_steps = 0;
}

bool Calibration::calibrate()
{
  if (this->calibrated)
    return true;

  bool in_field = isInField();

  // Schon im Feld, aber weniger als 20 Schritte draußen unterwegs?
  if (in_field && this->ouside_steps <= MIN_STEPS_OUTSIDE_FIELD)
  {
    this->should_go_outside = true;
    this->ouside_steps = 0;
  }

  // Motor rückwärts drehen, damit er mindestens einmal draußen war
  if (this->should_go_outside && this->ouside_steps < MIN_STEPS_OUTSIDE_FIELD)
  {
    this->motor.stepBackward();
    this->ouside_steps++;
    return false;
  }

  // Motor befindet sich im Magnetfeld & das Feld wurde bisher nicht vollständig gelesen
  if (in_field && !this->field_read)
  {
    this->motor.stepForward();
    this->was_in_field = true;
    this->calibrated_steps++;
    return false;
  }

  // Das Feld wurde wieder verlassen, nachdem es komplett durchlaufen wurde
  if (!in_field && this->was_in_field && !this->field_read)
  {
    this->field_read = true;
    this->calibrated_steps = this->calibrated_steps / 2;
    Serial.println("Steps needed backward " + String(this->calibrated_steps));
  }

  // Wir waren noch nicht im Feld & sind es auch nicht -> Solange vorwärts laufen bis es gefunden ist.
  if (!in_field && !this->field_read)
  {
    this->motor.stepForward();
    this->ouside_steps++;
    return false;
  }

  // Wir waren nun im Feld & zentrieren noch den Zeiger
  if (this->calibrated_steps > 0 && this->field_read)
  {
    Serial.println("going backwards");
    this->motor.stepBackward();
    this->calibrated_steps--;
    return false;
  }

  // Wir waren im Feld, wir haben das komplette Feld gelesen und die Steps zur Kalibrierung sind vollende -> Wir sind done!
  if (this->field_read && this->calibrated_steps == 0)
  {
    Serial.println("done");
    this->calibrated = true;
    return true;
  }

  return false;
}

bool Calibration::isInField()
{
  return analogRead(this->hall_pin) < 100;
}