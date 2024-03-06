
EXECUTABLE  = elevator

COMPILER    = clang
CFLAGS      = -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS     = -fsanitize=address
EXCLUDE     = '*test*'


SOURCEDIR = source
OBJECTDIR = build

SOURCES := $(patsubst $(SOURCEDIR)/%, %, $(shell find $(SOURCEDIR) -name '*.c'  ! -name $(EXCLUDE)  ! -path $(EXCLUDE)))
OBJECTS = $(addprefix $(OBJECTDIR)/, $(SOURCES:.c=.o))

## executable depends on object files: link them
$(EXECUTABLE): $(OBJECTS) 
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## object files depend on source files: compile them
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS) $<

# Declare which targets are not files, but rather commands for Make
.PHONY: all rebuild clean docs

# Build all
all:
	@echo "Building $(EXECUTABLE)"
	$(EXECUTABLE)
	@echo "Generating documentation"
	docs

# Rebuild all
rebuild: clean all

# Remove all generated files
clean:
	rm -f $(EXECUTABLE)
	rm -rf $(OBJECTDIR)

# Generate documentation
docs: | doxconfig # The pipe symbol ensures that the doxconfig exists before making the docs
	doxygen doxconfig
#	@echo "Opening docs"
#   @open html/index.html

