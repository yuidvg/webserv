CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -MMD -I src
RM = rm -rf
AR = ar rcs

NAME = webserv
SRCS_DIR = src/
SRCS = $(wildcard $(SRCS_DIR)*/*.cpp $(SRCS_DIR)*.cpp)
OBJS_DIR = obj/
OBJS = $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(SRCS:.cpp=.o))
DEPS = $(SRCS:.cpp=.d)


#DEBUG
DEBUG_CXXFLAGS = -g
DEBUG_NAME = debug.out
DEBUG_OBJS_DIR = debug_obj/
DEBUG_OBJS = $(patsubst $(SRCS_DIR)%,$(DEBUG_OBJS_DIR)%,$(SRCS:.cpp=.o))
DEBUG_CLIENT_NAME = client
DEBUG_CLIENT_SRCS = client.cpp
DEBUG_CLIENT_OBJS = $(DEBUG_CLIENT_SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR) $(DEBUG_OBJS_DIR) $(DEBUG_CLIENT_OBJS)

fclean: clean
	$(RM) $(NAME) $(DEBUG_NAME) $(DEBUG_CLIENT_NAME)

re: fclean all


#DEBUG
debug: $(DEBUG_NAME) $(DEBUG_CLIENT_NAME)

$(DEBUG_NAME): $(DEBUG_OBJS) $(DEBUG_CLIENT_OBJS)
	$(CXX) $(DEBUG_CXXFLAGS) -o $@ $^

# オブジェクトファイルの生成ルール
$(DEBUG_OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEBUG_CLIENT_NAME): $(DEBUG_CLIENT_OBJS)
	$(CXX) -o $@ $^

.PHONY: all clean fclean re debug

-include $(DEPS)