#include "main.h"

/*
 * e_ident[] - IDENTIFICATION INDEXES
 *
 * Description: The initial bytes mark the file as an object file
 *              and provide machine-independent data.
 * The initial bytes of an ELF header (and an object file)
 *  correspond to the e_ident member.
*/

/**
 * check_if_elf - checks if file is an ELF file by checking if it
 *                contains the magic numbers/file signature of an ELF file
 *
 * @e_ident: a pointer to an array of bytes specifies
 *           how to interpret the file
 *
 * Return: nothing
*/
void check_if_elf(unsigned char *e_ident)
{
	int index = 0;

	while (index < 4)
	{
		if (e_ident[index] != 127 &&
		    e_ident[index] != 'E' &&
		    e_ident[index] != 'L' &&
		    e_ident[index] != 'F') /*0x7F is 127 in ASCII*/
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}

		index++;
	}
}

/**
 * print_magic - prints the magic numbers of the ELF file
 *
 * @e_ident: a pointer to an array of bytes specifies
 *           how to interpret the file
 *
 * Return: nothing
*/
void print_magic(unsigned char *e_ident)
{
	int index = 0;

	printf("  Magic    ");

	while (index < EI_NIDENT) /*EI_NIDENT == sizeof(e_ident[]) = 16*/
	{
		printf("%02x", e_ident[index]);

		if (index == EI_NIDENT - 1)
			printf("\n");
		else
			printf(" "); /*space each number*/

		index++;
	}
}

/**
 * print_class - prints the class of the ELF file
 *
 * @e_ident: a pointer to an array of bytes specifies
 *           how to interpret the file
 *
 * Return: nothing
*/
void print_class(unsigned char *e_ident)
 * print_type - prints the type of the ELF file
 *
 * @e_type: the object file type
 * @e_ident: a pointer to an array of bytes specifies
 *           how to interpret the file
 *
 * Return: nothing
*/
void print_type(unsigned int e_type, unsigned char *e_ident)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		e_type >>= 8;

	printf("  Type:                              ");

	switch (e_type)
	{
		case ET_NONE:
			printf("NONE (Unknown type)\n");
			break;
		case ET_REL:
			printf("REL (Relocatable file)\n");
			break;
		case ET_EXEC:
			printf("EXEC (Exexutable file)\n");
			break;
		case ET_DYN:
			printf("DYN (Shared object file)\n");
			break;
		case ET_CORE:
			printf("CORE (Core file)\n");
			break;
		default:
			printf("<unknown: %x\n", e_type);
	}
}

/**
 * print_entry - prints the entry point of an ELF file
 *
 * @e_entry: the virtual memory address from where the
 *           process starts executing
 *
 * @e_ident: a pointer to an array of bytes specifies
 *           how to interpret the file
 *
 * Return: nothing
*/
void print_entry(unsigned long int e_entry, unsigned char *e_ident)
{
	printf("  Entry point address:               ");

	if (e_ident[EI_DATA] == ELFDATA2MSB)
	{
		e_entry = ((e_entry << 8) & 0xFF00FF00) |
			  ((e_entry >> 8) & 0xFF00FF);
		e_entry = (e_entry << 16) | (e_entry >> 16);
	}

	if (e_ident[EI_CLASS] == ELFCLASS32)
		printf("%#x\n", (unsigned int)e_entry);
	else
		printf("%#lx\n", e_entry);
}

/**
 * close_file - closes the ELF file
 *
 * @elf_file: the file descriptor for the ELF file
 *
 * Return: nothing
*/
void close_file(int elf_file)
{
	if (close(elf_file) == -1)
	{
		dprintf(STDERR_FILENO,
			"Error: Can't close fd %d\n", elf_file);
		exit(98);
	}
}

/**
 * main - Entry point
 *
 * Description: elf_header_parser file
 *
 * @argc: argument counter
 * @argv: argument vector
 *
 * Return: Always 0 (success)
*/
int main(int __attribute__((__unused__)) argc, char *argv[])
{
	Elf64_Ehdr *elf;
	int elf_file, read_file;

	elf_file = open(argv[1], O_RDONLY);
	if (elf_file == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	elf = malloc(sizeof(Elf64_Ehdr));
	if (elf == NULL)
	{
		close_file(elf_file);
		free(elf);
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	read_file = read(elf_file, elf, sizeof(Elf64_Ehdr));
	if (read_file == -1)
	{
		free(elf);
		close_file(elf_file);
		dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", argv[1]);
		exit(98);
	}
	check_if_elf(elf->e_ident);
	printf("ELF Header:\n");
	print_magic(elf->e_ident);
	print_class(elf->e_ident);
	print_data(elf->e_ident);
	print_version(elf->e_ident);
	print_osabi(elf->e_ident);
	print_abi(elf->e_ident);
	print_type(elf->e_type, elf->e_ident);
	print_entry(elf->e_entry, elf->e_ident);
	free(elf);
	close_file(elf_file);
	return (0);
}
