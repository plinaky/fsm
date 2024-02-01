#include "standards.h"
#include "board.h"
#include "compare.h"

static int fd;

void *create_map(const off_t size)
{
	void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (map == MAP_FAILED) {
		perror("Erreur lors de l'allocation de mémoire");
		return NULL;
	}

	memset(map, 0, size);

	return map; 
}

void *open_map(const char *path, const off_t size)
{
	struct stat file_stat;
	void *map = NULL;

	// Ouvre le fichier en lecture seule
	fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("Erreur lors de l'ouverture du fichier");
		return NULL;
	}

	// Redimensionne le fichier à la taille nécessaire
	if (ftruncate(fd, size) == -1) {
		perror("Erreur lors du redimensionnement du fichier");
		close(fd);
		exit(EXIT_FAILURE);
	}

	// Utilise mmap pour mapper le fichier en mémoire
	map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (map == MAP_FAILED) {
		perror("Erreur lors du mappage du fichier en mémoire");
		close(fd);
		return NULL;
	}

	return map; 
}

int delete_map(void *map, const off_t size)
{
	// Libère la mémoire allouée
	if (munmap(map, size) == -1)
		perror("Erreur lors de la libération de la mémoire allouée");

	return EXIT_SUCCESS;
}

int flush_map(void *map, const off_t size)
{

	// Redimensionne le fichier à la taille nécessaire
	if (ftruncate(fd, size) == -1) {
		perror("Erreur lors du redimensionnement du fichier");
		close(fd);
		exit(EXIT_FAILURE);
	}

	// Libère la mémoire allouée
	if (munmap(map, size) == -1)
		perror("Erreur lors de la libération de la mémoire allouée");

	// Fermeture du fichier de sortie
	close(fd);

	return EXIT_SUCCESS;
}
