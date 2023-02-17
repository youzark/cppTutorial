exceptionHandler: exceptionHandling.o Printer.o
	g++ exceptionHandling.o Printer.o -o exceptionHandler

exceptionHandling.o: exceptionHandling.cpp
	g++ -c exceptionHandling.cpp -o exceptionHandling.o

Printer.o: Printer.cpp
	g++ -c Printer.cpp -o Printer.o

clean:
	rm -f *.o
