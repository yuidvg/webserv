SERVER_NAME = server
CLIENT_NAME = client
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SERVER_SOURCE = server.cpp
CLIENT_SOURCE = client.cpp

all : $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME) : $(SERVER_SOURCE)
	$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_SOURCE)

$(CLIENT_NAME) : $(CLIENT_SOURCE)
	$(CC) $(CFLAGS) -o $(CLIENT_NAME) $(CLIENT_SOURCE)

clean:
	rm -rf $(SERVER_NAME) $(CLIENT_NAME)

re: clean all