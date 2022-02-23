all: philosophers

philosophers: philosophers.c 
	gcc $< -Wall -Wextra -Werror -lpthread -o $@

test: philosophers
	./philosophers

clean:
	rm -f philosophers
