#ifndef __INPUT_H
#define __INPUT_H

#define MAX_NAME_LENGTH		50 /* max значение для `name' столбца */
#define MAX_QUERY_LENGTH	1000


enum INPUT_ERROR_TYPE
{
	INPUT_OK,
	INPUT_ERROR_INT,
	INPUT_ERROR_STR,
	INPUT_ERROR_ID,
	INPUT_NULL
};



/*
	* прочитать и обработать вводимую строку
	* при вводе не допустимого значение система дает
		возможность ввести данные снова
*/
int  in_getstr(char *s);



/*
	* прочитать и обработать вводимое число
	* при вводе не допустимого значение система дает
		возможность ввести данные снова
*/
int  in_getnum(int *num);



/*
	* прочитать и обработать вводимое слово
	* при вводе не допустимого значение система дает
		возможность ввести данные снова
*/
int  in_getword(char *s);


/*
	* read query from console
*/
int in_getquery(char *s);

#endif