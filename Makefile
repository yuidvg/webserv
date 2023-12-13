CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
RM = rm -rf
AR = ar rcs
NAME = webserv
CLIENT_NAME = client

INCLUDES = -I includes/
SRCS_DIR = src/
OBJS_DIR = obj/

# srcディレクトリのソースファイル
SRCS = $(wildcard $(SRCS_DIR)*.cpp)
# srcディレクトリ外のclient.cpp
CLIENT_SRCS = client.cpp
# オブジェクトファイルのパス変換
OBJS = $(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_DIR)%.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

all: $(OBJS_DIR) $(NAME) $(CLIENT_NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

$(CLIENT_NAME): $(CLIENT_OBJS)
	$(CXX) -o $@ $^

# オブジェクトファイルの生成ルール
$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# client.cppのオブジェクトファイル生成
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# オブジェクトファイルを格納するディレクトリを作成
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	$(RM) $(OBJS_DIR) $(CLIENT_OBJS)

fclean: clean
	$(RM) $(NAME) $(CLIENT_NAME)

re: fclean all

debug: CXXFLAGS += -g
debug: re

.PHONY: all clean fclean re debug
