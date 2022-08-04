NAME = GameOfLife
all:
	cat README.md
	c++ -Wall -Wextra -Werror main.cpp -o $(NAME)

$(NAME): all

clean:
	rm -f $(NAME)

re: clean all

.PHONY: $(NAME) all clean re
