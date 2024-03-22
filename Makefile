
EXECUTABLE  = elevator
TEST_EXECUTABLE = unit_tests

C_INCLUDES = \
-Icore/inc \
-Icore/driver

C_INCLUDES_TEST = \
-Itests/unity \
-Itests/unit \
$(C_INCLUDES)

COMPILER    = clang
CFLAGS      = $(C_INCLUDES) -Wall -g -std=gnu11 -fsanitize=address
CFLAGS_TEST = $(C_INCLUDES_TEST) -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS     = -fsanitize=address
EXCLUDE     = '*test*'

SOURCEDIR = core
TESTDIR  = tests
BUILDDIR = build
TESTBUILDDIR = test_build

SOURCES := $(patsubst $(SOURCEDIR)/%, %, $(shell find $(SOURCEDIR) -name '*.c'  ! -name $(EXCLUDE)  ! -path $(EXCLUDE)))
OBJECTS  = $(addprefix $(BUILDDIR)/, $(SOURCES:.c=.o))

$(info Sources: $(SOURCES))
$(info Objects: $(OBJECTS))

TEST_SOURCES := $(patsubst $(TESTDIR)/%, %, $(shell find $(TESTDIR) -name '*.c')) #$(patsubst $(SOURCEDIR)/%, %, $(shell find $(SOURCEDIR) -name '*.c'  ! -name $(EXCLUDE)  ! -path $(EXCLUDE)))
TEST_OBJECTS  = $(addprefix $(TESTBUILDDIR)/, $(TEST_SOURCES:.c=.o)) #$(addprefix $(TESTBUILDDIR)/, $(SOURCES:.c=.o))

$(info Test sources: $(TEST_SOURCES))
$(info Test objects: $(TEST_OBJECTS))

# Declare which targets are not files, but rather commands for Make
.PHONY: default rebuild clean docs test

# Rules to build `default`
default: $(EXECUTABLE) # Technically, only default because it is the first rule in the file

# Rules to build `rebuild`
rebuild: clean default

# Rules to build `clean`
clean:
	rm -f $(EXECUTABLE)
	rm -rf $(BUILDDIR)
	rm -rf $(TESTBUILDDIR)

# Rules to build `docs`
docs: | doxconfig # The pipe symbol ensures that the doxconfig exists before making the docs
	doxygen doxconfig
#	@echo "Opening docs"
#   @open html/index.html

test: $(TEST_EXECUTABLE) 

## executable depends on object files: link them
$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## object files depend on source files: compile them
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS) $<

## test executable depends on object files: link them
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(filter-out %main.o,$(OBJECTS))
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## test object files depend on source files: compile them
$(TESTBUILDDIR)/%.o: $(TESTDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS_TEST) $<