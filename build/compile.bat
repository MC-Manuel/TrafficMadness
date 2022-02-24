call g++ -Wall -c ../src/*.cpp -I../include
call g++ -Wall -c ../src/cpp/gameObjects/*.cpp -I../include
call g++ -Wall -c ../src/cpp/global/*.cpp -I../include
call g++ *.o -o TrafficMadness.exe -L../lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system 
