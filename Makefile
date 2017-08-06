DEBUG_ARGS=-g
# -fsanitize=undefined -fsanitize=address
FLAGS=-std=c99 -Wall -Wextra -Werror -Wconversion -Wstrict-prototypes -pedantic -Wformat-security -Wno-missing-field-initializers
ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:allocator_may_return_null=1
# LIBS=-pthread
INC=-I.
CC=clang

all: libhtml_parser.so

test: print_tree libhtml_parser.so

%.o: %.c
	${CC} ${FLAGS} ${DEBUG_ARGS} -fPIC -c $<

libhtml_parser.so: html_parser.o string.o
	${CC} ${FLAGS} $^ -shared -o $@

print_tree: test/print_tree.c libhtml_parser.so string.o
	${CC} ${FLAGS} ${DEBUG_ARGS} $^ -o test/$@

search_nodes: test/search_nodes.c libhtml_parser.so string.o
	${CC} ${FLAGS} ${DEBUG_ARGS} $^ -o test/$@

clean: html_parser.o print_tree.o string.o libhtml_parser.so print_tree
	-rm *.o
	-rm print_tree *.so
