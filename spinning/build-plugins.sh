#!/bin/bash
g++ -I$1/include box-plugin-classic.cpp -shared -fPIC -o libbox-plugin-classic.so
g++ -I$1/include box-plugin-adaptive.cpp -shared -fPIC -o libbox-plugin-adaptive.so
