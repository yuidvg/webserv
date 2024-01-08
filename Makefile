#OMIT BEFORE SUBMITTING
DEBUG_FLAGS = -g

CXX = c++
CXXFLAGS = $(DEBUG_FLAGS) -Wall -Werror -Wextra -std=c++98 -MMD -I src
RM = rm -rf
AR = ar rcs

NAME = webserv
SRCS_DIR = src/
SRCS = $(wildcard $(SRCS_DIR)*/*.cpp $(SRCS_DIR)*.cpp)
OBJS_DIR = obj/
OBJS = $(patsubst $(SRCS_DIR)%,$(OBJS_DIR)%,$(SRCS:.cpp=.o))
DEPS = $(SRCS:.cpp=.d)

#CLIENT
CLIENT_NAME = client_app
CLIENT_SRCS = client/client.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

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
	$(RM) $(NAME) $(DEBUG_NAME) $(DEBUG_CLIENT_NAME)

re: fclean all

client: $(CLIENT_NAME)

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: all clean fclean re debug

-include $(DEPS)