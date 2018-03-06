# legato-util

Small collection of helper functions required by some of our open source code

## API

### File I/O

Functions to handle boilerplate when reading and writing from files.

```C
le_result_t readFromFile (const char *path, void *value,
  int (*scanCallback) (FILE *f, void *val));
int scanIntCallback (FILE *f, void *value);
int scanDoubleCallback (FILE *f, void *value);
le_result_t ioutil_readIntFromFile (const char *path, int *value);
le_result_t ioutil_readDoubleFromFile (const char *filePath, double *value);
le_result_t ioutil_writeToFile (const char *path, void *value, size_t size, size_t count);
```

### Time
```C
int util_getUnixDatetime ();
time_t util_getMTime (char *path);
uint64_t GetCurrentTimestamp (void);
```

### Other
```C
le_result_t util_flattenRes (le_result_t *res, int nRes);
```