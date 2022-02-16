g++ -Wall -c ../src/*.cpp -I../include
::g++ -Wall -c ../src/external/*.cpp
g++ -Wall -c ../src/cpp/gameObjects/*.cpp -I../include
g++ -Wall -c ../src/cpp/global/*.cpp -I../include

g++ *.o -Wl,-subsystem,windows -o TrafficMadness.exe -L../lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system 


