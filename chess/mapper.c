#include "standards.h"
#include "board.h"
#include "compare.h"

void *create_map(const off_t size)
{
	void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (map == MAP_FAILED) {
		perror("Erreur lors de l'allocation de mémoire");
		return NULL;
	}

	return map; 
}

void *open_map(const char *path, const off_t size)
{
	struct stat file_stat;
	void *map = NULL;
	int fd;

	// Ouvre le fichier en lecture seule
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		perror("Erreur lors de l'ouverture du fichier");
		return NULL;
	}

	// Obtient la taille du fichier
	if (fstat(fd, &file_stat) == -1) {
		perror("Erreur lors de la récupération de la taille du fichier");
		close(fd);
		return NULL;
	}

	// Vérifie que la taille du fichier est suffisante
	if (file_stat.st_size < size) {
		fprintf(stderr, "La taille du fichier est inférieure à %d Mo\n", (int)(size / (1024 * 1024)));
		close(fd);
		return NULL;
	}

	// Utilise mmap pour mapper le fichier en mémoire
	map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

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

int flush_map(const char *path, void *map, const off_t size)
{
	// Écriture des données dans un fichier
	int output_fd = open(path, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

	if (output_fd == -1) {
		perror("Erreur lors de l'ouverture du fichier de sortie");
		munmap(map, size);
		return EXIT_FAILURE;
	}

	// Écriture des données dans le fichier
	if (write(output_fd, map, size) == -1)
		perror("Erreur lors de l'écriture dans le fichier de sortie");

	// Fermeture du fichier de sortie
	close(output_fd);

	// Libère la mémoire allouée
	if (munmap(map, size) == -1)
		perror("Erreur lors de la libération de la mémoire allouée");

	return EXIT_SUCCESS;
}
