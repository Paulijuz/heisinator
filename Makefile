
EXECUTABLE  = elevator

C_INCLUDES = \
-Icore/inc \
-Icore/driver

COMPILER    = clang
CFLAGS      = $(C_INCLUDES) -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS     = -fsanitize=address
EXCLUDE     = '*test*'

SOURCEDIR = core
OBJECTDIR = build

SOURCES := $(patsubst $(SOURCEDIR)/%, %, $(shell find $(SOURCEDIR) -name '*.c'  ! -name $(EXCLUDE)  ! -path $(EXCLUDE)))
OBJECTS = $(addprefix $(OBJECTDIR)/, $(SOURCES:.c=.o))

# Declare which targets are not files, but rather commands for Make
.PHONY: default rebuild clean docs

# Rules to build `default`
default: $(EXECUTABLE) # Technically, only default because it is the first rule in the file

# Rules to build `rebuild`
rebuild: clean default

# Rules to build `clean`
clean:
	rm -f $(EXECUTABLE)
	rm -rf $(OBJECTDIR)

# Rules to build `docs`
docs: | doxconfig # The pipe symbol ensures that the doxconfig exists before making the docs
	doxygen doxconfig
#	@echo "Opening docs"
#   @open html/index.html

## executable depends on object files: link them
$(EXECUTABLE): $(OBJECTS) 
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## object files depend on source files: compile them
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS) $<
