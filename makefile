#
#MakeFile
#
#Brett Binnersley, V00776751
#Csc 360, Assignment #1
#

all: makeMain

clean:
	cleanItems

makeMain:
	gcc main.c -lreadline  -o main

cleanItems:
	-rm -rf *.o *.exe
