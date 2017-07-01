DEBUG_ARGS=-g -fsanitize=undefined -fsanitize=address
FLAGS=-std=c99 -Wall -Wextra -Werror -Wconversion -Wstrict-prototypes -pedantic -Wformat-security
ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:allocator_may_return_null=1
# LIBS=-pthread
INC=-I.
jC=clang

html_parser: html_parser.o string.o
	${CC} ${FLAGS} ${LIBS} ${DEBUG_ARGS} ${INC} $^ -o $@

%.o: %.c
	${CC} ${FLAGS} ${LIBS} ${DEBUG_ARGS} -c $^

debug: html_parser
	gdb -nh -x /home/kotto/local/cfg/gdb/init $^

run: html_parser test.html
	./html_parser test.html 1>&2

clean:
	-rm string.o html_parser.o
	-rm html_parser

all: html_parser
