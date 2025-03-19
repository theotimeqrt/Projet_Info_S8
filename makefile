# Compilateur et options
CC = g++
CXXFLAGS = -Wall -std=c++17

# Nom de l'exécutable
TARGET = pingpong

# Fichiers sources
SRC = main.cpp classes.cpp forces.cpp
OBJ = $(SRC:.cpp=.o)

# Règle principale : compilation complète après nettoyage
all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compilation des fichiers sources en objets
%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

# Recompilation complète
re: fclean all

# Exécution
run: all
	./$(TARGET)

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make ou make all     -> Compile le projet"
	@echo "  make run             -> Compile et exécute le projet"
	@echo "  make clean           -> Supprime les fichiers objets"
	@echo "  make fclean          -> Supprime les exécutables et les objets"
	@echo "  make re              -> Nettoie et recompile entièrement le projet"
	@echo "  make help            -> Affiche cette aide"
