#!/bin/bash
g++ -std=c++11 client.cpp -lpthread -o client.exe
g++ -std=c++11 server.cpp -o server.exe
