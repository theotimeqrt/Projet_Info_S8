# Variables
CC = g++
CFLAGS = -I.
TARGETS = test
CLASSES_SRC = classes.cpp
MAIN_SRC = main.cpp

CLASSES_OBJ = $(CLASSES_SRC:.cpp=.o)
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# Règle principale
all: run

run: $(TARGETS)
	@echo "🚀 Lancement du programme..."
	./test

test: $(MAIN_OBJ) $(CLASSES_OBJ)
	@echo "🔨 Création de l'exécutable..."
	$(CC) $(CFLAGS) -o test $(MAIN_OBJ) $(CLASSES_OBJ)
	@echo "✅ Exécutable créé avec succès."

classes: $(CLASSES_OBJ)
	@echo "🔨 Compilation des classes en fichiers objets..."
	@echo "✅ Classes compilées avec succès."

main: $(MAIN_OBJ)
	@echo "🔨 Compilation de main en fichier objet..."
	@echo "✅ Main compilé avec succès."

# Compilation des fichiers sources en objets
%.o: %.cpp
	@echo "🔨 Compilation de $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 🧹 Nettoyage des fichiers objets et exécutables
clean:
	@echo "🧹 Nettoyage des fichiers objets..."
	rm -f $(CLASSES_OBJ) $(MAIN_OBJ)
	@echo "🧹 Nettoyage terminé."

fclean: clean
	@echo "🧹 Suppression des exécutables..."
	rm -f $(TARGETS)
	@echo "🧹 Suppression terminée."

# Recompilation complète
re: fclean all

# Aide
help:
	@echo "📝 Commandes disponibles :"
	@echo "  make ou make all → Compile main.cpp et classes.cpp"
	@echo "  make classes     → Compile uniquement classes.cpp en objet"
	@echo "  make main        → Compile uniquement main.cpp en objet"
	@echo "  make clean       → Supprime les fichiers objets (.o)"
	@echo "  make fclean      → Supprime les exécutables et les objets"
	@echo "  make re          → Nettoie tout (fclean) et recompile entièrement le projet (all)"
	@echo "  make help        → Affiche les commandes disponibles"
