# Eng-Vie Dictionary

A small C program for looking up English-Vietnamese words from the terminal - add, edit, delete and search.

Words are kept in a sorted linked list, with recursive search/insert, and saved to `tudien.txt` automatically.

## Run it

Needs gcc (MinGW/MSYS2 works fine):

```
gcc tudien.c -o tudien.exe
tudien.exe
```

Menu (the actual program prints it in Vietnamese):

```
1. Add a word
2. Edit meaning / example
3. Delete a word
4. Look up a word
5. Show the whole dictionary
6. Save now
0. Quit
```

`tudien.txt` already has a few common words to try out. The program loads it on start and saves on exit.
