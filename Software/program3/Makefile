clean:
	echo cleaning
build: clean
	gcc program3Tester.c program.c ../lib/util.c -Wall -Wextra -o program.exe 
run: build
	./program.exe | tee output.txt
