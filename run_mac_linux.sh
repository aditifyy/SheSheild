#!/bin/sh
set -e
cd "$(dirname "$0")"

g++ -std=c++11 -O2 -pthread \
    backend/cpp/WebServer.cpp \
    backend/cpp/Graph.cpp \
    -o backend/cpp/sheshield_server

exec ./backend/cpp/sheshield_server
