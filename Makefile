proj5.exe: CarClass.o EventClass.o random.o IntersectionSimulationClass.o project5.o
	g++ CarClass.o EventClass.o random.o IntersectionSimulationClass.o project5.o -o proj5.exe

CarClass.o: CarClass.cpp
	g++ -std=c++98 -Wall -c CarClass.cpp -o CarClass.o
EventClass.o: EventClass.cpp
	g++ -std=c++98 -Wall -c EventClass.cpp -o EventClass.o
random.o: random.cpp
	g++ -std=c++98 -Wall -c random.cpp -o random.o
IntersectionSimulationClass.o: IntersectionSimulationClass.cpp
	g++ -std=c++98 -Wall -c IntersectionSimulationClass.cpp -o IntersectionSimulationClass.o
project5.o: project5.cpp
	g++ -std=c++98 -Wall -c project5.cpp -o project5.o

clean:
	rm -rf CarClass.o EventClass.o random.o IntersectionSimulationClass.o project5.o proj5.exe