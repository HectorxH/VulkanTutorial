default:
    just --list

build:
    cmake -B build -S .
    make -C build

run app:
    cd ./build/apps/ && ./{{app}}

clean:
    make -C clean

format *files:
    clang-format --style=file -i {{files}}

format-all:
    find . -iname '*.hpp' -o -iname '*.cpp' | tee /dev/tty | xargs clang-format --style=file -i
