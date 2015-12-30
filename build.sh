bin="html_parser"
echo "Compiling project..."
clang -Wall -o $bin html_parser.c string.c -I. --debug -g

./$bin
