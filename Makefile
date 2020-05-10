
debug: calculator_debug.o console_main_debug.o
	g++ -o debug console_main.o calculator.o -g

calculator_debug.o: calculator.cpp calculator.h
	g++ -c calculator.cpp -g

console_main_debug.o: console_main.cpp
	g++ -c console_main.cpp -g

console_calculator: calculator.o console_main.o
	g++ -o console_calculator console_main.o calculator.o

calculator.o: calculator.cpp calculator.h
	g++ -c calculator.cpp

console_main.o: console_main.cpp
	g++ -c console_main.cpp


.PHONY: clean
clean: 
	rm *.o
