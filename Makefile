CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 --pedantic-errors
RM = rm -rf
AR = ar rcs
NAME = webserv

INCLUDES = -I includes/
SRCS_DIR = src/
OBJS_DIR = obj/

SRCS = $(wildcard $(SRCS_DIR)*.cpp)
OBJS = $(addprefix $(OBJS_DIR), $(notdir $(SRCS:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all


debug: CXXFLAGS += -g
debug: re

.PHONY: all mkdir clean fclean re debug
