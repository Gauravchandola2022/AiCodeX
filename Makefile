CC = gcc
CFLAGS = -Wall -Wextra

SRCS = compiler.c parser.c semantic.c utils/lex.yy.c components/symbol_table.c components/tokens.c components/memory.c components/ast_json_exporter.c components/ast_visualizer.c components/parsers/parser.c components/parsers/expressions.c components/parsers/statements.c components/parsers/conditionals.c components/parsers/functions.c components/parsers/loops.c components/generator/codegen.c
OBJS = $(SRCS:.c=.o)
TARGET = cmmx

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
