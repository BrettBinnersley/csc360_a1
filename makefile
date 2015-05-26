#
#MakeFile
#
#Brett Binnersley, V00776751
#Csc 360, Assignment #1
#

all: makeRsi

clean: cleanItems

makeRsi:
	gcc rsi.c -o rsi

cleanItems:
	-rm -rf *.o *.exe
