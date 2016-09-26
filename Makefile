deps:
	./scripts/deps.sh

clean:
	rm -rf build

base: deps
	mkdir -p build && cd build && cmake ../

create-runner: base
	cd build && make runner -j8

run: create-runner
	./build/runner

create-tests: base
	cd build && make test_runner -j8

test: create-tests
	./build/test_runner

.PHONY: run create-runner test create-tests
