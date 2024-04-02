
all: test.cpp
	g++ test.cpp -o test

ifeq ($(OS),Windows_NT)
clean:
	del test.exe

run: all
	test.exe test.json

else
clean:
	rm test

run: all
	./test test.json

endif

