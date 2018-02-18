CC = clang
WARNINGS = -Wall -Werror -Wfloat-equal -Wformat -g
CFLAGS = -std=c99
LDLIBS = -lm -lncurses -lcurses

blddir = build
srcs = $(shell ls src | grep "\.c")
objs = $(srcs:.c=.o)
exe = fiddle

.PHONY: all clean

all: $(blddir) $(exe)

$(blddir):
	mkdir -p $@

%.o: src/%.c
	@echo "Compiling    \x1B[92m$@\x1B[0m \x1B[2m<- $<\x1B[0m"
	@$(CC) $(WARNINGS) $(CFLAGS) -c $< -o $(blddir)/$@

$(exe): $(objs)
	@echo "Linking      \x1B[93m$@\x1B[0m \x1B[2m<- $(foreach i,$^,$(blddir)/$(i))\x1B[0m "
	@$(CC) $(WARNINGS) $(LDLIBS) -o $@ $(foreach i,$^,$(blddir)/$(i) )

clean:
	$(Q)rm -rf $(blddir)
