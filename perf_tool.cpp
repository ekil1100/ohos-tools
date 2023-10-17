#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <elf_file>\n";
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        std::perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) < 0)
    {
        std::perror("fstat");
        close(fd);
        return 1;
    }

    void *mem = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
    {
        std::perror("mmap");
        close(fd);
        return 1;
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)mem;
    Elf64_Shdr *shdrs = (Elf64_Shdr *)((char *)mem + ehdr->e_shoff);
    const char *strtab = (const char *)mem + shdrs[ehdr->e_shstrndx].sh_offset;

    for (int i = 0; i < ehdr->e_shnum; ++i)
    {
        if (shdrs[i].sh_type == SHT_SYMTAB)
        {
            // ark .strtab index is 2;
            shdrs[i].sh_link = 2;
            const char *symtab_str = (const char *)mem + shdrs[shdrs[i].sh_link].sh_offset;
            Elf64_Sym *syms = (Elf64_Sym *)((char *)mem + shdrs[i].sh_offset);
            int first_global = -1;
            for (size_t j = 0; j < shdrs[i].sh_size / sizeof(Elf64_Sym); ++j)
            {
                if (ELF64_ST_BIND(syms[j].st_info) == STB_GLOBAL &&
                    first_global == -1)
                {
                    first_global = j;
                }
                if (ELF64_ST_TYPE(syms[j].st_info) == STT_FUNC)
                {
                    // ark text start offset is 0x1000;
                    syms[j].st_value = syms[j].st_value + 0x1000;
                    // section 1 is ark aot .text
                    syms[j].st_shndx = 1;
                }
                // ark aot file max section number is 6.
                if (syms[j].st_shndx > 6)
                    syms[j].st_shndx = 0;
                /* std::cout << symtab_str + syms[j].st_name << "\t value:" << syms[j].st_value << " \t shn:" << syms[j].st_shndx << std::endl; */
            }
            shdrs[i].sh_info = first_global;
            /* std::cout << ".symtab's sh_info is " << shdrs[i].sh_info << std::endl; */
        }
    }

    munmap(mem, sb.st_size);
    close(fd);
    return 0;
}
