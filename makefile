# Compilateur et options
CC = g++
CFLAGS = -I. -Wall -Wextra -Werror -std=c++11
LDFLAGS = -lgflw -lGL -lGLU 
TARGET = pingpong

# Fichiers sources
CLASSES_SRC = classes.cpp
FORCES_SRC = forces.cpp
MAIN_SRC = main.cpp

# Fichiers objets
CLASSES_OBJ = $(CLASSES_SRC:.cpp=.o)
FORCES_OBJ = $(FORCES_SRC:.cpp=.o)
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# Tous les fichiers objets
OBJS = $(MAIN_OBJ) $(CLASSES_OBJ) $(FORCES_OBJ)

# Règle principale : compile et exécute
all: run

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) 

# Compilation des fichiers sources en objets
%.o: %.cpp fonctions.hpp
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

# Recompilation complète
re: fclean all

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make ou make all     -> Compile et exécute le projet"
	@echo "  make clean           -> Supprime les fichiers objets"
	@echo "  make fclean          -> Supprime les exécutables et les objets"
	@echo "  make re              -> Nettoie et recompile entièrement le projet"
	@echo "  make help            -> Affiche cette aide"
