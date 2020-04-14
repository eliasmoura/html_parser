DEBUG_ARGS=-g
ASAN=-fsanitize=undefined -fsanitize=address
FLAGS=-std=c99 -Wall -Wextra -Werror -Wconversion -Wstrict-prototypes -pedantic -Wformat-security -Wno-missing-field-initializers
ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:allocator_may_return_null=1
# LIBS=-pthread
INC=-I.
CC=gcc

all: libhtml_parser.so

test: test/search_nodes.c
	cd test
	make
	
%.o: %.c
	${CC} ${FLAGS} ${DEBUG_ARGS} -fPIC -c $<

libhtml_parser.so: html_parser.o string.o
	${CC} ${FLAGS} $^ -shared -o $@

clean:
	-rm *.o
	-rm libhtml_parser.so

.PHONY: clean
