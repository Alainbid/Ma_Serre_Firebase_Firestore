#ifndef LIRE_LES_CONSIGNES_H
#define LIRE_LES_CONSIGNES_H

#include <Arduino.h> // Include any necessary libraries


class LireLesConsignes{

      public : 

struct Consignes
{
  int duree;
  int  frequence;
};

     Consignes lirelesconsignes();
};

#endif // LIRE_LES_CONSIGNES_H