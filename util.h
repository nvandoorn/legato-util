#ifndef UTIL_H
#define UTIL_H

#include "legato.h"

#define HIGH 1
#define LOW 0

// ioutil
le_result_t readFromFile (const char *path, void *value,
  int (*scanCallback) (FILE *f, void *val));
int scanIntCallback (FILE *f, void *value);
int scanDoubleCallback (FILE *f, void *value);
le_result_t ioutil_readIntFromFile (const char *path, int *value);
le_result_t ioutil_readDoubleFromFile (const char *filePath, double *value);
le_result_t ioutil_writeToFile (const char *path, void *value, size_t size, size_t count);

// gpio
void getGpioPath (char *outputStr, int pin, char *subDir);
le_result_t gpio_exportPin (int pin);
le_result_t gpio_unexportPin (int pin);
le_result_t gpio_setDirection (int pin, char *direction);
le_result_t gpio_setInput (int pin);
le_result_t gpio_setOutput (int pin);
le_result_t gpio_setActiveState (int pin, bool isActiveLow);
le_result_t gpio_isActive (int pin, bool *isActive);
le_result_t gpio_setValue (int pin, bool state);
le_result_t gpio_setLow (int pin);
le_result_t gpio_setHigh (int pin);

// time util
// TODO give these a quasi "namespace"
void delayMicro (unsigned long microsecs);
void delayMilli (unsigned long millisecs);
long getTimeMicrosecs ();

#endif
