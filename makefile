
ifeq ($(OS),Windows_NT)

all: test.cpp test2.cpp
	g++ test.cpp -o test.exe
	g++ test2.cpp -o test2.exe

clean:
	del test.exe test2.exe

run: all
	test2.exe
	test.exe test.json

else

all: test.cpp test2.cpp
	g++ test.cpp -o test.out
	g++ test2.cpp -o test2.out

clean:
	rm test.out test2.out

run: all
	./test2
	./test test.json

endif

