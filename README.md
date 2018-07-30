# PolyLib

## Dependencies

1. <a href="https://github.com/alexmustdie/yandex-cpp">yandex-cpp</a> (my own)<br>
1. <a href="https://github.com/alexmustdie/vk-cpp">vk-cpp</a> (my own)<br>
1. <a href="https://github.com/jtv/libpqxx">pqxx</a>

## Install

sudo mkdir polylib<br>
sudo cp include/\*.hpp /usr/include/polylib/<br>
g++ -shared -o build/libpolylib.so -std=c++14 src/\*.cpp -fPIC -lyandex -lvk<br>
sudo cp build/libpolylib.so /usr/lib/

## Usage

cd usage/

g++ synh-drive.cpp -lyandex -lpolylib -lpq -lpqxx -o release/synch-drive<br>
g++ main.cpp -lyandex -lvk -lpolylib -lpq -lpqxx -o release/bot

cd release/

./synch-drive<br>
./bot
