build:
    @mkdir -p build
    @cmake -S . -B build
    @cd build && \
        make my_sqlite

run: build
    @./build/my_sqlite

test:
    @mkdir -p build
    @cmake -S . -B build
    @cd build &&    \
        make test_sqlite  &&\
        ./tests/test_sqlite

clean:
    @rm -rf build