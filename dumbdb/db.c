#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct buffer
{
	char *buff;
	size_t capac;
	size_t len;
} buffer_t;


/* prompt message */
void db_prompt (void)
{
	printf ("db> ");
}


/* shut down database if fatal error happens */
void db_die (char *msg)
{
	fprintf (stderr, "err: %s\n", msg);
	exit (1);
}


/* initialize buffer */
buffer_t *db_init (size_t capac)
{
	buffer_t *buff = malloc (sizeof (buffer_t *));
	if (buff == NULL) {
		db_die ("could not allocate memory");
	}

	buff->buff = malloc (sizeof (char*) * capac);
	if (buff->buff == NULL) {
		db_die ("could not allocate memory");
	}

	buff->capac = capac;
	return buff;
}


/* get commands from user */
void db_get_command (buffer_t *buff)
{
	getline (&(buff->buff), &(buff->capac), stdin);
	buff->len = strlen (buff->buff) - 1;
	buff->buff[buff->len] = 0;
}

int main (void)
{
	size_t capac = 30;
	buffer_t *buff = db_init (capac);

	while (1) {
		db_prompt ();
		db_get_command (buff);
		if (buff->buff != NULL && strcmp (buff->buff, ".exit") == 0) {
			exit (1);
		}
		else {
			printf ("command '%s' is under development\n", buff->buff);
		}
	}
}
