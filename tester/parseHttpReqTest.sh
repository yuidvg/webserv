#!/bin/bash

SERVER="localhost"
PORT="8080"

function telnet_test()
{
    {
        echo "GET / HTTP/1.1"
        echo "Host: $SERVER"
        echo ""  # Additional empty line to end the request
        sleep 2  # Give some time to receive the response
    } | telnet $SERVER $PORT
}

echo "Running Telnet test..."
telnet_test
