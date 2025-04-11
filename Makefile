CC = gcc -g -Wall -Wextra -Werror
RD = -lreadline

HED = inc/
SRC = src/main.c src/initialization/get_env.c utils/strutils.c utils/free_minishell.c
OBJ = $(SRC:.c=.o)

NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -I$(HED) $(OBJ) -o $(NAME) $(RD)

%.o: %.c $(HED)
	$(CC) -c $< -o $@ $(RD)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean