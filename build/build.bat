g++ -Wall -Wextra -Werror -Wpedantic -c ../src/Tombs.cpp -I../include/ ^
	-I../../SDL3/SDL3-3.2.22/x86_64-w64-mingw32/include ^
	-I../../SDL3_image/SDL3_image-3.2.4/x86_64-w64-mingw32/include ^
	-I../../SDL3_ttf/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include 
g++ -Wall -Wextra -Werror -Wpedantic -c ../src/Game.cpp -I../include/ ^
	-I../../SDL3/SDL3-3.2.22/x86_64-w64-mingw32/include ^
	-I../../SDL3_image/SDL3_image-3.2.4/x86_64-w64-mingw32/include ^
	-I../../SDL3_ttf/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include 
g++ -Wall -Wextra -Werror -Wpedantic -c ../src/main.cpp -I../include/ ^
	-I../../SDL3/SDL3-3.2.22/x86_64-w64-mingw32/include ^
	-I../../SDL3_image/SDL3_image-3.2.4/x86_64-w64-mingw32/include ^
	-I../../SDL3_ttf/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include 
g++  main.o Game.o Tombs.o libvuk.a -o under.exe ^
	-L../../SDL3/SDL3-3.2.22/x86_64-w64-mingw32/lib ^
	-L../../SDL3_image/SDL3_image-3.2.4/x86_64-w64-mingw32/lib ^
	-L../../SDL3_ttf/SDL3_ttf-3.1.0/x86_64-w64-mingw32/lib ^
	-lSDL3_image -lSDL3_ttf -lSDL3 