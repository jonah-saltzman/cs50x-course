# cs50x-course

A selection of programs I wrote for cs50x online course that I found most interesting/fun to write.

tideman.c is my code for the 'feeling more comfortable' week 3 problem set 'tideman', 
for which I wrote a c program to hold an election, given some number of candidates and voters, 
and produce a winner according to the tideman/ranked pairs electoral system.

helpers.c is the code I wrote (as opposed to distribution code) for the 'feeling more comfortable' 
week 4 problem set 'filters', for which I wrote a c program that takes a bmp as input, applies
one of four filters to the bitmap, and outputs the resulting bmp as a file. The four filters are
a grayscale filter, a horizontal reflection filter, a blur filter, and an edge detection filter
implementing the Sobel operator.

dictionary.c is the code I wrote for the week 5 problem set 'speller', for which I wrote a c program 
that takes as input a dictionary file and a text file, loads the dictionary into memory using a hash
table (handling collisions using linked lists), then spellchecks the content of the text file according 
to the given dictionary. The challenge for this pset was to load the dictionary into memory and check
the given text as quickly as possible (wall-clock time); the average performance (as time to load & check)
of my program was within 5% of that of the cs50 staff's model program.

dna.py is my code for the week 6 problem set 'dna', which takes as input a csv database of STRs used in
forensic science to match a person's identity with a DNA sample, and a text file containing a raw DNA
sequence. The program loads the database and sequence into memory, and for each STR given in the csv
database, counts the number of repeats of that STR in the DNA sequence. If the number of repeats of each
STR found in the DNA sequence matches the number of repeats for a person in the database, the DNA sequence
is reported to match that person.
