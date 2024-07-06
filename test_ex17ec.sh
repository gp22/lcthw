#!/bin/bash
set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

ERROR="${RED}test failed${NC}"
SUCCESS="${GREEN}test passed${NC}"

if test -f ex17ec.dat; then
    rm ex17ec.dat
fi

if test -f ex17ec; then
    rm ex17ec
fi

echo "Running make as... make ex17ec"
make ex17ec

echo ""

echo "Test case 1 running as... ./ex17ec ex17ec.dat c 5 2"
./ex17ec ex17ec.dat c 5 2

echo ""

echo "Test case 2 running as... ./ex17ec ex17ec.dat s 0 gp22 gp22"
./ex17ec ex17ec.dat s 0 gp22 gp22

echo ""

echo "Test case 3 running as... ./ex17ec ex17ec.dat s 1 gp22 gp22"
./ex17ec ex17ec.dat s 1 gp22 gp22

echo ""

echo "Test case 4 running as... ./ex17ec ex17ec.dat l"
./ex17ec ex17ec.dat l

echo ""

echo "Test case 5 running as... ./ex17ec ex17ec.dat g 0"
./ex17ec ex17ec.dat g 0

echo ""

echo "Test case 6 running as... ./ex17ec ex17ec.dat g 1"
./ex17ec ex17ec.dat g 1

echo ""

echo "Test case 7 running as... ./ex17ec ex17ec.dat d 0"
./ex17ec ex17ec.dat d 0

echo ""

echo "Test case 8 running as... ./ex17ec ex17ec.dat d 1"
./ex17ec ex17ec.dat d 1

echo ""

echo "Test case 9 running as... ./ex17ec ex17ec.dat l"
./ex17ec ex17ec.dat l

echo ""

rm ex17ec.dat
rm ex17ec
