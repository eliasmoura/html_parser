
INC= %.h

%.o: %.c
	clang -g -std=c99 -I$(INC) %<
html_parser: %.o
	clang -g -std=c99 -I$(INC) %< -o html_parser


all: html_parser
