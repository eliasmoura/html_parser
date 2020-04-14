DEBUG_ARGS=-g
ASAN=-fsanitize=undefined -fsanitize=address
FLAGS=-std=c99 -Wall -Wextra -Werror -Wconversion -Wstrict-prototypes -pedantic -Wformat-security -Wno-missing-field-initializers
ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:allocator_may_return_null=1
# LIBS=-pthread
INC=-I.
CC=clang

all: ../libhtml_parser.so print_tree search_nodes

%.o: %.c
	${CC} ${FLAGS} ${DEBUG_ARGS} -fPIC -c $<

print_tree: print_tree.c ../libhtml_parser.so ../string.o
	${CC} ${FLAGS} ${DEBUG_ARGS} $^ -o $@

search_nodes: search_nodes.c ../libhtml_parser.so ../string.o
	${CC} ${FLAGS} ${DEBUG_ARGS} ${ASAN} $^ -o $@

clean: 
	-rm print_tree.o print_tree search_nodes.o search_nodes
	
