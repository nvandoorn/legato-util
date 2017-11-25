#include "legato.h"
#include "util.h"

le_result_t readFromFile (const char *path, void *value,
  int (*scanCallback) (FILE *f, const char *formatter, void *val)) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    LE_WARN("Couldn't open '%s' - %m", path);
    return LE_IO_ERROR;
  }
  int numScanned = scanCallback(f, "%lf", value);
  if (numScanned != 1) return LE_FORMAT_ERROR;
  fclose(f);
  return LE_OK;
}

int scanIntCallback (FILE *f, const char *formatter, void *value) {
  int *val = value;
  return fscanf(f, "%d", val);
}

int scanDoubleCallback (FILE *f, const char *formatter, void *value) {
  double *val = value;
  return fscanf(f, "%lf", val);
}

// TODO camel case these methods
le_result_t ioutil_readIntFromFile (const char *path, int *value) {
  return readFromFile(path, value, scanIntCallback);
}

le_result_t ioutil_readDoubleFromFile (const char *path, double *value) {
  return readFromFile(path, value, scanDoubleCallback);
}

le_result_t ioutil_writeToFile (const char *path, void *value, size_t size, size_t count) {
  LE_INFO("Attempting to write to %s", path);
  FILE *f = fopen(path, "w");
  if (f == NULL) {
    LE_WARN("Failed to open %s for writing", path);
    return LE_IO_ERROR;
  }
  size_t nWritten = fwrite(value, size, count, f);
  LE_INFO("Wrote %d bytes", nWritten);
  fclose(f);
  return LE_OK;
}

le_result_t gpio_exportPin (int pin) {
  return ioutil_writeToFile ("/sys/class/gpio/export", &pin, sizeof(int), 1);
}

le_result_t gpio_unexportPin (int pin) {
  return ioutil_writeToFile ("/sys/class/gpio/unexport", &pin, sizeof(int), 1);
}

void getGpioPath (char *outputStr, int pin, char *subDir) {
  sprintf(outputStr, "/sys/class/gpio/gpio%d/%s", pin, subDir);
}

le_result_t gpio_setDirection (int pin, char *direction) {
  char path[50];
  getGpioPath(path, pin, "direction");
  return ioutil_writeToFile(path, direction, sizeof(char), strlen(direction));
}

le_result_t gpio_setInput (int pin) {
  return gpio_setDirection(pin, "in");
}

le_result_t gpio_setOutput (int pin) {
  return gpio_setDirection(pin, "out");
}

le_result_t gpio_setActiveState (int pin, bool isActiveLow) {
  // Any non zero value toggles the existing value
  // so we must read the existing value first
  char path[50];
  int isActiveLowSet;
  getGpioPath(path, pin, "active_low");
  le_result_t readRes = ioutil_readIntFromFile(path, &isActiveLowSet);
  le_result_t writeRes = LE_OK;
  if (isActiveLow != isActiveLowSet) {
    writeRes = ioutil_writeToFile(path, "1", sizeof(char), 1);
  }
  return readRes == LE_OK && writeRes == LE_OK ?
    LE_OK : LE_FAULT;
}

le_result_t gpio_isActive (int pin, bool *isActive) {
  char path[50];
  getGpioPath(path, pin, "value");
  int val;
  return ioutil_readIntFromFile(path, &val);
  LE_FATAL_IF(val != 0 || val != 1, "value is not boolean");
  *isActive = (bool)val;
}

le_result_t gpio_setValue (int pin, bool state) {
  char path[50];
  getGpioPath(path, pin, "value");
  return ioutil_writeToFile(path, &state, sizeof(bool), 1);
}

le_result_t gpio_setLow (int pin) {
  return gpio_setValue(pin, LOW);
}

le_result_t gpio_setHigh (int pin) {
  return gpio_setValue(pin, HIGH);
}

void delayMicro (unsigned long microsecs) {
  unsigned long retTime = getTimeMicrosecs() + microsecs;
  while (getTimeMicrosecs() < retTime);
}

void delayMilli (unsigned long millisecs) {
  delayMicro(millisecs * 1000);
}

long getTimeMicrosecs () {
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}