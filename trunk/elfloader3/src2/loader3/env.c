
#include "loader.h"


extern char *strchrnul (const char *__s, int __c);
char **l__environ = 0;

static char **l_last_environ = 0;


/* This function is used by `setenv' and `putenv'.  The difference between
   the two functions is that for the former must create a new string which
   is then placed in the environment, while the argument of `putenv'
   must be used directly.  This is all complicated by the fact that we try
   to reuse values once generated for a `setenv' call since we can never
   free the strings. [in uclibc, we do not]  */
__arm static int __add_to_environ(const char *name, const char *value,
 		int replace)
{
	register char **ep;
	register size_t size;
	char *var_val;
	char **new_environ;
	/* name may come from putenv() and thus may contain "=VAL" part */
	const size_t namelen = strchrnul(name, '=') - name;
	int rv = -1;

	/* We have to get the pointer now that we have the lock and not earlier
	   since another thread might have created a new environment.  */
	ep = l__environ;

	size = 0;
	if (ep != NULL) {
		while (*ep != NULL) {
			if (!strncmp(*ep, name, namelen) && (*ep)[namelen] == '=') {
				/* Found */
				if (!replace)
					goto DONE_OK;
				goto REPLACE;
			}
			++size;
			++ep;
		}
	}

	/* Not found, add at the end */

	/* We allocated this space; we can extend it.  */
	new_environ = realloc(l_last_environ, (size + 2) * sizeof(char *));
	if (new_environ == NULL) {
		//__set_errno(ENOMEM);
		goto DONE;
	}
	if (l__environ != l_last_environ) {
		memcpy(new_environ, l__environ, size * sizeof(char *));
	}
	l_last_environ = l__environ = new_environ;

	ep = &new_environ[size];
	/* Ensure env is NULL terminated in case malloc below fails */
	ep[0] = NULL;
	ep[1] = NULL;

 REPLACE:
	var_val = (char*) name;
	if (value != NULL) {
		const size_t vallen = strlen(value) + 1;

		var_val = malloc(namelen + 1 + vallen);
		if (var_val == NULL) {
			//__set_errno(ENOMEM);
			goto DONE;
		}
		memcpy(var_val, name, namelen);
		var_val[namelen] = '=';
		memcpy(&var_val[namelen + 1], value, vallen);
	}
	*ep = var_val;

 DONE_OK:
	rv = 0;

 DONE:

	return rv;
}


__arm int l_setenv(const char *name, const char *value, int replace)
{
	/* NB: setenv("VAR", NULL, 1) inserts "VAR=" string */
	return __add_to_environ(name, value ? value : "", replace);
}


__arm int l_unsetenv(const char *name)
{
	const char *eq;
	size_t len;
	char **ep;

	if (name == NULL || *name == '\0'
	 || *(eq = strchrnul(name, '=')) == '='
	) {
		//__set_errno(EINVAL);
		return -1;
	}
	len = eq - name; /* avoiding strlen this way */

	ep = l__environ;
	/* NB: clearenv(); unsetenv("foo"); should not segfault */
	if (ep)	while (*ep != NULL) {
		if (!strncmp(*ep, name, len) && (*ep)[len] == '=') {
			/* Found it.  Remove this pointer by moving later ones back.  */
			char **dp = ep;
			do {
				dp[0] = dp[1];
			} while (*dp++);
			/* Continue the loop in case NAME appears again.  */
		} else {
			++ep;
		}
	}

	return 0;
}


__arm char *l_getenv(const char *var)
{
    int len;
    char **ep;

    if (!(ep=l__environ))
	return NULL;
    len = strlen(var);
    while(*ep) {
	if (memcmp(var, *ep, len) == 0 && (*ep)[len] == '=') {
	    return *ep + len + 1;
	}
	ep++;
    }
    return NULL;
}

/* The `clearenv' was planned to be added to POSIX.1 but probably
   never made it.  Nevertheless the POSIX.9 standard (POSIX bindings
   for Fortran 77) requires this function.  */
__arm int l_clearenv(void)
{

	/* If we allocated this environment we can free it.
	 * If we did not allocate this environment, it's NULL already
	 * and is safe to free().  */
    if(!l_last_environ)
        return 1;

	int i = 0;
	while(l_last_environ[i])
	{
	  free(l_last_environ[i]);
          l_last_environ[i] = 0;
	  ++i;
	}

    free(l_last_environ);
	l_last_environ = NULL;
	l__environ = NULL;
	return 0;
}

