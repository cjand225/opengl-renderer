# Declare variables
DEST = build
PROJECT = OpenGLRenderer

# Default command when typing 'make'
default: build

# Create Build folder and compile source
build: clean
	mkdir -p $(DEST) && cd $(DEST) && cmake .. && make

# Start the binary
run: build
	./$(DEST)/$(PROJECT)

# Remove build folder
clean: 
	rm -rf $(DEST)

.PHONY: default build run clean
