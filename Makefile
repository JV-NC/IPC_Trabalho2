# Nome do executável
TARGET = main

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -O2

# Arquivos fonte e objeto
SRC = main.c utility.c
OBJ = $(SRC:.c=.o)

# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXT = .exe
else
	RM = rm -f
	EXT =
endif

# Regra padrão
all: $(TARGET)$(EXT)

# Como compilar o executável
$(TARGET)$(EXT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Como compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	$(RM) *.o $(TARGET)$(EXT)

# Força a recompilação de tudo
rebuild: clean all
