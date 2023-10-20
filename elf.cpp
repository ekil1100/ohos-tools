#include <fcntl.h>
#include <gelf.h>
#include <iostream>
#include <libelf.h>
#include <unistd.h>

int main()
{
    // 打开 ELF 文件
    int fd = open("t.an", O_RDONLY, 0);
    if (fd < 0)
    {
        std::cerr << "无法打开 ELF 文件" << std::endl;
        return 1;
    }

    // 初始化 libelf
    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        std::cerr << "初始化 libelf 失败" << std::endl;
        close(fd);
        return 1;
    }

    // 打开 ELF 对象
    Elf *elf = elf_begin(fd, ELF_C_READ, nullptr);
    if (!elf)
    {
        std::cerr << "无法打开 ELF 对象" << std::endl;
        close(fd);
        return 1;
    }

    // 获取 ELF 类型
    Elf_Kind ek = elf_kind(elf);
    std::cout << "ELF 类型: " << ek << std::endl;
    if (ek != ELF_K_ELF)
    {
        std::cerr << "不是一个有效的 ELF 文件" << std::endl;
        elf_end(elf);
        close(fd);
        return 1;
    }

    // 获取 ELF 符号表节
    Elf_Scn *scn = nullptr;
    Elf_Data *data = nullptr;
    GElf_Shdr shdr;

    while ((scn = elf_nextscn(elf, scn)) != nullptr)
    {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB)
        {
            data = elf_getdata(scn, data);
            break;
        }
    }

    if (!data)
    {
        std::cerr << "未找到符号表" << std::endl;
        elf_end(elf);
        close(fd);
        return 1;
    }

    // 读取符号表
    GElf_Sym sym;
    size_t nsyms = shdr.sh_size / shdr.sh_entsize;

    for (size_t i = 0; i < nsyms; ++i)
    {
        gelf_getsym(data, i, &sym);

        // 获取符号名称
        const char *sym_name = elf_strptr(elf, shdr.sh_link, sym.st_name);

        // 获取符号地址
        Elf64_Addr sym_addr = sym.st_value;

        // 获取符号大小
        GElf_Xword sym_size = sym.st_size;

        // 获取符号绑定属性（如局部、全局等）
        unsigned char sym_bind = GELF_ST_BIND(sym.st_info);

        // 获取符号类型（如函数、对象、未定义等）
        unsigned char sym_type = GELF_ST_TYPE(sym.st_info);

        // 打印符号信息
        std::cout << "Symbol #" << i << std::endl;
        std::cout << "Symbol Name: " << sym_name << std::endl;
        std::cout << "Symbol Address: " << std::hex << sym_addr << std::dec
                  << std::endl;
        std::cout << "Symbol Size: " << sym_size << std::endl;
        std::cout << "Symbol Binding: " << int(sym_bind) << std::endl;
        std::cout << "Symbol Type: " << int(sym_type) << std::endl;

        // 可以进一步根据需要处理其他符号信息
    }

    // 清理资源
    elf_end(elf);
    close(fd);

    return 0;
}
