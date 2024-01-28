#include "tree.h"
#include "standards.h"

struct node *create_map(uint32_t pos_count)
{
	const off_t size = pos_count * sizeof(struct node);

	void *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (map == MAP_FAILED) {
		perror("Erreur lors de l'allocation de mémoire");
		return NULL;
	}
	create_root((struct node *)map, pos_count);

	return (struct node *)map; 
}

struct node *open_map(const char *path, uint32_t pos_count)
{
	const off_t size = pos_count * sizeof(struct node);
	struct stat file_stat;
	void *map= NULL;
	int fd;

	// Ouvre le fichier en lecture seule
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		perror("Erreur lors de l'ouverture du fichier");
		return map;
	}

	// Obtient la taille du fichier
	if (fstat(fd, &file_stat) == -1) {
		perror("Erreur lors de la récupération de la taille du fichier");
		close(fd);
		return map;
	}

	// Vérifie que la taille du fichier est suffisante
	if (file_stat.st_size < size) {
		fprintf(stderr, "La taille du fichier est inférieure à %d Mo\n", (int)(size / (1024 * 1024)));
		close(fd);
		return map;
	}

	// Utilise mmap pour mapper le fichier en mémoire
	map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED) {
		perror("Erreur lors du mappage du fichier en mémoire");
		close(fd);
		return map;
	}

	return (struct node *)map; 
}

int flush_map(char *path, struct node *map, uint32_t pos_count)
{
	const off_t size = pos_count * sizeof(struct node);

	// Écriture des données dans un fichier
	int output_fd = open(path, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

	if (output_fd == -1) {
		perror("Erreur lors de l'ouverture du fichier de sortie");
		munmap((void *)map, size);
		return EXIT_FAILURE;
	}

	// Écriture des données dans le fichier
	if (write(output_fd, (void *)map, size) == -1)
		perror("Erreur lors de l'écriture dans le fichier de sortie");

	// Fermeture du fichier de sortie
	close(output_fd);

	// Libère la mémoire allouée
	if (munmap((void *)map, size) == -1)
		perror("Erreur lors de la libération de la mémoire allouée");

	return EXIT_SUCCESS;
}

