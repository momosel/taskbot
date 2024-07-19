all: run

build:
	@echo "Creating build directory..."
	mkdir -p build
	@echo "Running CMake..."
	cd build && cmake .. || { echo 'CMake failed'; exit 1; }
	@echo "Building project..."
	cd build && make || { echo 'Make failed'; exit 1; }

run: build
	@echo "Checking compiler and architecture..."
	g++ --version
	uname -m
	@echo "Running the bot..."
	cd build && ./TelegramBot || { echo 'Run failed'; exit 1; }

clean:
	@echo "Cleaning build directory..."
	rm -rf build
