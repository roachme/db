#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "input.h"
#include "engine.h"

static int in_chknum(const char *s)
{
	if (s == NULL)
		return INPUT_NULL;

	while (*s != '\0')
		if (!isdigit(*s++))
			return INPUT_ERROR_INT;
		
	return	INPUT_OK;
}

static int in_chkword(const char *s)
{
	const char *p = s;

	if (s == NULL)
		return INPUT_NULL;

	if (isdigit(*p++)) {
		fprintf(stderr, "ERROR in_chkid: id can't start with digit [%d]\n", *p);
		return INPUT_ERROR_ID;
	}


	while (*p) {
		if (!(isalnum(*p) || *p == '_') ) {
			printf("ERROR in_chkid: Unacceptable character [%c]\n", *p);
			return INPUT_ERROR_ID;
		}
		p++;
	}

	return	INPUT_OK;
}


int in_getnum(int *num)
{
	char buffer[MAX_NAME_LENGTH], *p = buffer;
	int flag;

	if ((flag = in_getstr(p) != INPUT_OK) )
		return flag;

	if ( (flag = in_chknum(p) ) != INPUT_OK)
		return flag;

	*num = atoi(p);
	return DB_OK;
}

int in_getword(char *s)
{
	int c, flag, size = 0;
	char *p = s;

	while ((c = getchar()) != EOF && c != '\n' && size < MAX_NAME_LENGTH) {
		*p++ = c;
		size++;
	}
	*p = '\0';

	/* empty input */
	if (strlen(s) == 0) {
		return INPUT_NULL;
	}

	if ((flag = in_chkword(s)) != INPUT_OK)
		return flag;

	return INPUT_OK;
}

int in_getstr(char *s)
{
	int c, size = 0;
	char *p = s;

	while ((c = getchar()) != EOF && c != '\n' && (size + 1) < MAX_NAME_LENGTH) {
		*p++ = c;
		size++;
	}
	*p = '\0';

	if (strlen(s) == 0)
		return INPUT_NULL;

	return INPUT_OK;
}

int in_getquery(char *s)
{
	int c, size = 0;
	char *p = s;

	while ((c = getchar()) != EOF && c != '\n' && (size + 1) < MAX_QUERY_LENGTH) {
		*p++ = c;
		size++;
	}
	*p = '\0';

	if (strlen(s) == 0)
		return INPUT_NULL;

	return INPUT_OK;
}

#ifdef __cplusplus
}
#endif
