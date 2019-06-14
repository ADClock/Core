#ifndef _NUMBERIMAGE_H_
#define _NUMBERIMAGE_H_
#include "Arduino.h"

#define MAX_STEPS 2050

class NumberImage
{
public:
  uint16_t *getImageInSteps(char c)
  {
    uint16_t *steps = getImageInDegree(c);

    for (size_t i = 0; i < 36; i++)
    {
      steps[i] = steps[i] * (MAX_STEPS / 360);
    }

    return steps;
  }

  uint16_t *getImageInDegree(char c)
  {
    switch (c)
    {
    case '0':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 180, 180, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '1':
      uint16_t degrees[36] = {315, 315, 90, 225, 270, 180, 45, 180, 225, 180, 0, 180, 0, 45, 0, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 90, 0, 270};
      return degrees;
    case '2':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '3':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '4':
      uint16_t degrees[36] = {90, 180, 270, 180, 270, 180, 0, 180, 0, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 90, 270, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 90, 0, 270};
      return degrees;
    case '5':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 180, 0, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '6':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 180, 0, 180, 180, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '7':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 90, 270, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 180, 0, 180, 315, 315, 0, 90, 0, 270};
      return degrees;
    case '8':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 180, 180, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 180, 180, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;
    case '9':
      uint16_t degrees[36] = {90, 180, 270, 90, 270, 180, 0, 180, 180, 180, 0, 180, 0, 180, 0, 0, 0, 180, 0, 90, 270, 180, 0, 180, 90, 180, 0, 270, 0, 180, 0, 90, 270, 90, 0, 270};
      return degrees;

    default:
      break;
    }
    return NULL;
  }
};

#endif