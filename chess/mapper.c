int main() {
    const off_t file_size = 500 * 1024 * 1024;   // 500 Mo

    // Utilise mmap pour allouer un bloc de mémoire sans fichier préexistant
    void *mapped_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped_data == MAP_FAILED) {
        perror("Erreur lors de l'allocation de mémoire");
        return EXIT_FAILURE;
    }

    // Accès aux données allouées (par exemple, les 100 premiers octets)
    printf("Contenu des 100 premiers octets de la mémoire allouée :\n%.100s\n", (char *)mapped_data);

    // Utilisez maintenant l'espace alloué comme vous le souhaitez...

    // Libère la mémoire allouée
    if (munmap(mapped_data, file_size) == -1) {
        perror("Erreur lors de la libération de la mémoire allouée");
    }

    return EXIT_SUCCESS;
}
