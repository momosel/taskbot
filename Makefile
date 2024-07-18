all: build

build:
	mkdir -p build
	cd build && cmake ..
	cd build && make

run: build
	./build/TelegramBot

clean:
	rm -rf build
