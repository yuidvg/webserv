#!/bin/bash

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
NORMAL="\033[0m"
SERVER_ADDRESS="${1:-localhost:8080}"

echo "############# Normal tests #############"
./tester/normalTest.sh
echo

echo "############# CGI tests #############"
./tester/cgiTester.sh
echo

# その他のコマンド
other_commands=(
    "ruby ./tester/chunkRequest.rb"
)
function run_other_command() {
    local cmd=$1

    echo "Running: ${cmd}"
    eval ${cmd}
    if [[ $? -eq 0 ]]; then
        echo -en "${GREEN}OK"
    else
        echo -en "${RED}KO"
    fi
    echo -e "${NORMAL} -> ${cmd}"
}

echo "############# chunk test #############"
for cmd in "${other_commands[@]}"; do
    run_other_command "$cmd"
done

./tester/siegeTest.sh