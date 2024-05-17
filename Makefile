#OMIT BEFORE SUBMITTING
# DEBUG_FLAGS = -g #-O0

CXX = c++
CXXFLAGS = -MMD -Wall -Werror -Wextra -std=c++98 -I src
RM = rm -rf
AR = ar rcs

NAME = webserv
SRCS_DIR = src/
SRCS = $(wildcard $(SRCS_DIR)*/*.cpp $(SRCS_DIR)*.cpp)
OBJS_DIR = obj/
OBJS = $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(SRCS:.cpp=.o))
DEPS = $(SRCS:.cpp=.d)

DEBUG_NAME = webserv_debug
DEBUG_OBJS_DIR = debug_obj/
DEBUG_OBJS = $(patsubst $(SRCS_DIR)%,$(DEBUG_OBJS_DIR)%,$(SRCS:.cpp=.o))
DEBUG_DEPS = $(SRCS:.cpp=.d)
DEBUG_FLAGS = -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR) $(DEBUG_OBJS_DIR)

fclean: clean
	$(RM) $(NAME) $(CLIENT_NAME) $(DEBUG_NAME)

re: fclean all

debug: $(DEBUG_NAME)

$(DEBUG_NAME): $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $^

$(DEBUG_OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

test:
	@ASAN_OPTIONS=detect_leaks=1
	@echo "テストを開始します..."
	@./tester/tester.sh
	@echo "テストが完了しました。"

.PHONY: all clean fclean re test debug

-include $(DEPS)