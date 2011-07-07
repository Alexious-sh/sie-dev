
#ifndef __ENV_H__
#define __ENV_H__


int l_setenv(const char *name, const char *value, int replace);
int l_unsetenv(const char *name);
char *l_getenv(const char *var);
int l_clearenv(void);

#endif