7wbin:
	gcc -ggdb -o 7w *.c *.h -lncurses -lm
clean:
	rm -f 7w
