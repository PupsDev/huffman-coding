CC = gcc 
FLAGS = -O



#huf
huf.o : huf.c
	@echo "\033[1;30;47mhuf.c en cours de compilation...\033[0m"
	@$(CC) -c $^ $(FLAGS)
	@echo "huf.c compilée"

huf : huf.o
	@echo "\033[1;30;47mPréparation de l'exécutable huf ...\033[0m"
	@$(CC) -o $@ $^ $(FLAGS)
	@echo "excecutable huf prêt."

#dehuf
dehuf.o : dehuf.c
	@echo "\033[1;30;47mdehuf.c en cours de compilation...\033[0m"
	@$(CC) -c $^ $(FLAGS)
	@echo "dehuf.c compilée"

dehuf : dehuf.o
	@echo "\033[1;30;47mPréparation de l'exécutable dehuf ...\033[0m"
	@$(CC) -o $@ $^ $(FLAGS)
	@echo "excecutable dehuf prêt."

clean :
	@rm -rf *.o
	@echo "Suppression des fichiers objet terminée."

cleanAll :
	@rm -rf *.o *.gch huf dehuf *.huf *.dehuf
	@echo "Nettoyage terminé."