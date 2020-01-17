#include "Calibration.h"

Calibration::Calibration(Motor &m, size_t hall_pin) : motor(m), hall_pin(hall_pin)
{
  pinMode(hall_pin, INPUT_PULLUP);
}

void Calibration::start_calibration()
{
  this->steps = 0;
  this->state = FINDMAGNET;
#ifdef DEBUG
  Serial.println("Starte Kalibierung");
#endif
}

// Funktionsweise der Kalibierung:
// 0. Der Motor muss das Magnetfeld finden (FINDMAGNET) und zählt die Steps
// 1. Der Motor muss sich mindestens MIN_STEPS_OUTSIDE_FIELD Steps außerhalb des Magnetfelds drehen, damit die initalisierung richtig funktioniert. (LEAVEMAGNET)
// 2. Der Motor muss das Magnetfeld finden (FINDMAGNET) und zählt die Steps
// 3. Der Motor durchwandert das Magnetfeld (INFIELD) und zählt die Steps
// 4. Der Motor verlässt das Magnetfeld nimmt die Stepps von INFIELD/2 und läuft diese zurück (CENTERING)
// 5. Der Motor ist kalibiert (CALIBRATED)
bool Calibration::calibrate()
{
#ifdef SKIPCALIBRATION
#ifdef DEBUG
  Serial.println("Calibration >> Die Kalibierung wurde aufgrund von 'SKIPCALIBRATION' übersprungen.");
#endif
  return true;
#endif

  if (this->state == CALIBRATED)
    return true;

  bool in_field = isInField();

  switch (this->state)
  {
  case FINDMAGNET:
    if (in_field && this->steps <= MIN_STEPS_OUTSIDE_FIELD)
    {
      this->steps = MIN_STEPS_OUTSIDE_FIELD + 1;
      this->state = LEAVEMAGNET;
#ifdef DEBUG
      Serial.println("Calibration >> Magnet in unter 20 Schritten gefunden. Drehe rückwärts..");
#endif
    }
    else if (in_field && this->steps > MIN_STEPS_OUTSIDE_FIELD)
    {
      this->steps = 0;
      this->state = INFIELD;
#ifdef DEBUG
      Serial.println("Calibration >> Magnet in über 20 Schritten gefunden. Durchlaufe das Feld.");
#endif
    }
    else
    {
      this->motor.stepForward();
      this->steps++;
    }
    break;

  case LEAVEMAGNET:
    if (this->steps == 0)
    {
      this->state = FINDMAGNET;
#ifdef DEBUG
      Serial.println("Calibration >> Aus dem Feld gedreht.. Suche Magnet erneut");
#endif
    }
    else
    {
      this->motor.stepBackward();
      this->steps--;
    }
    break;

  case INFIELD:
    if (!in_field)
    {
      this->state = CENTERING;
      this->steps = this->steps / 2;
#ifdef DEBUG
      Serial.println("Calibration >> Magnetfeld durchlaufen.. Zentriere den Zeiger");
#endif
    }
    else
    {
      this->motor.stepForward();
      this->steps++;
    }
    break;

  case CENTERING:
    if (this->steps == 0)
    {
      this->state = CALIBRATED;
      this->motor.reset();

#ifdef DEBUG
      Serial.println("Calibration >> Zeiger zentriert. Kalibierung abgeschlossen");
#endif
    }
    else
    {
      this->steps--;
      this->motor.stepBackward();
    }
    break;

  default:
    break;
  }

  return false;
}

bool Calibration::isInField()
{
  return analogRead(this->hall_pin) < 100;
}