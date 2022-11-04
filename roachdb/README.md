### roachdb

Here is dababase written from scratch. It is simlpe but at same time
shows main priciples that base actual databases lile MySQL, MariaDB and all.

roachdb supports few commands like add and delete entry, and can cotain more
than one entry at ones. All values are stored as CSV so every time you run
a program you do not have to fill your database all over again. 


Here are example of commands this database supports
-c  Create new entry.
-p  Print the latest entry.
-t  Print all entries sorted by title.
-d  Print all entries sorted by date.
-a  Print all entries sorted by author.


It uses simple linked list to store database entries.


TODO:
1) Allow the user to name column titles.
2) Add more commands and also envent a simple db language
