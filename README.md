# PolyLib

## Install

sudo mkdir polylib
sudo cp include/*.hpp /usr/include/polylib/
g++ -shared -o build/libpolylib.so -std=c++14 src/*.cpp -fPIC -lyandex -lvk
sudo cp build/libpolylib.so /usr/lib/

## Usage

cd usage/

g++ synh-drive.cpp -lyandex -lpolylib -lpq -lpqxx -o release/synch-drive
g++ main.cpp -lyandex -lvk -lpolylib -lpq -lpqxx -o release/bot

cd release/

./synch-drive
./bot
