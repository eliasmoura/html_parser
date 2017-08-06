DEBUG_ARGS=-g
# -fsanitize=undefined -fsanitize=address
FLAGS=-std=c99 -Wall -Wextra -Werror -Wconversion -Wstrict-prototypes -pedantic -Wformat-security -Wno-missing-field-initializers
ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:allocator_may_return_null=1
# LIBS=-pthread
INC=-I.
CC=clang

all: html_parser.so

test: print_tree html_parser.so

%.o: %.c
	${CC} ${FLAGS} ${DEBUG_ARGS} -fPIC -c $<

html_parser.so: html_parser.o string.o
	${CC} ${FLAGS} $^ -shared -o $@

print_tree: print_tree.o html_parser.so string.o
	${CC} ${FLAGS} ${DEBUG_ARGS} $^ -o test/$@

debug: html_parser
	gdb -nh -x /home/kotto/local/cfg/gdb/init $^

run: html_parser sample/test.html
	./html_parser ./sample/test.html 1>&2

clean: html_parser.o print_tree.o string.o html_parser.so print_tree
	-rm *.o
	-rm print_tree *.so
