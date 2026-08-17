NAME    = malloc

SRC_DIR = src
OBJ_DIR = obj

SRCS    = main.c $(wildcard $(SRC_DIR)/*.c)

OBJS    = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC      = cc
CFLAGS  = -Wall -Werror -Wextra

VAL     = valgrind --leak-check=full --track-origins=yes

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

debug: CFLAGS += -g3 -DDEBUG
debug: re

val: debug
	$(VAL) ./$(NAME)

.PHONY: all clean fclean re run debug val