build:
    @mkdir -p build
    @cmake -S . -B build
    @cd build && \
        make -j my_sqlite

run: build
    @./build/my_sqlite

unit-test:
    @mkdir -p build
    @cmake -S . -B build
    @cd build &&    \
        make -j test_sqlite  && \
        ./tests/unit/test_sqlite

integration-test: build
    @python -m pytest

clean:
    @rm -rf build