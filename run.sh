#!/bin/bash
HOST="127.0.0.1"
PORT="6666"

./server.exe $PORT
echo
echo "Test 1"
./client.exe $HOST $PORT < test1.txt
echo
echo "Test 2"
./client.exe $HOST $PORT < test2.txt
echo
echo "Test 3"
./client.exe $HOST $PORT < test3.txt
echo
echo "Test 4"
./client.exe $HOST $PORT < test4.txt
echo
