#OMIT BEFORE SUBMITTING
DEBUG_FLAGS = -g #-O0

CXX = c++
CXXFLAGS = $(DEBUG_FLAGS) -MMD -Wall -Werror -Wextra -std=c++98 -I src
RM = rm -rf
AR = ar rcs

NAME = webserv
SRCS_DIR = src/
SRCS = $(wildcard $(SRCS_DIR)*/*.cpp $(SRCS_DIR)*.cpp)
OBJS_DIR = obj/
OBJS = $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(SRCS:.cpp=.o))
DEPS = $(SRCS:.cpp=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR) $(DEBUG_OBJS_DIR) $(DEBUG_CLIENT_OBJS)

fclean: clean
	$(RM) $(NAME) $(CLIENT_NAME)

re: fclean all

test:
	@echo "テストを開始します..."
	@./tester/tester.sh
	@echo "テストが完了しました。"

.PHONY: all clean fclean re test

-include $(DEPS)