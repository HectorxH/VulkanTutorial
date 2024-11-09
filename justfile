default:
    just --list

build:
    cmake -B build -S .
    make -C build

run app:
    ./build/apps/{{app}}

clean:
    make -C clean

format-all:
    find . -iname '*.hpp' -o -iname '*.cpp' | tee /dev/tty | xargs clang-format -i


analyze:
