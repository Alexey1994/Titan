#include "make_PE.h"
#include <stdio.h>
#include <stdlib.h>

char dos_header[]={
                   'M', 'Z', 0x90,0x00,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0xff,0xff,0x00,0x00,
                   0xb8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,
                   0x0e,0x1f,0xba,0x0e,0x00,0xb4,0x09,0xcd,0x21,0xb8,0x01,0x4c,0xcd,0x21,'T', 'h',
                   'i', 's', ' ', 'p', 'r', 'o', 'g', 'r', 'a', 'm', ' ', 'c', 'a', 'n', 'n', 'o',
                   't', ' ', 'b', 'e', ' ', 'r', 'u', 'n', ' ', 'i', 'n', ' ', 'D', 'O', 'S', ' ',
                   'm', 'o', 'd', 'e', 0x2e,0x0d,0x0d,0x0a,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                  };

char windows_header[]="PE\0";
char machine_i386[]={0x4c,0x01};

void make_PE(char *name)
{
    FILE *f=fopen(name, "wb");
//File header
    short number_of_sections=5;
    int time_date_stamp=0x5236E84E;
    int symbols_pointer=0;
    int number_of_symbols=0;
    short size_of_optional_header=0x00e0;
    short flags=0x030f;

//Optional header
    short magic=0x010b;
    short linker_version=0x0256;
    int size_of_code=0x00000a00;
    int size_of_initialized_data=0x00001200;
    int size_of_uninitialized_data=0x00000200;
    int address_of_entry_point=0x00001280;
    int base_of_code=0x00001000;
    int base_of_data=0x00002000;
//
    int image_base=0x00400000;
    int section_alignment=0x00001000;
    int file_alignment=0x00000200;
    int operating_system_version=0x04;
    int image_version=0x01;
    int subsystem_version=0x04;
    int win32_version_value=0;
    int size_of_image=0x00006000;
    int size_of_headers=0x00000400;
    int check_sum=0x0000d18a;
    short subsystem=0x0003;
    short dll_characteristics=0;
    int size_of_stack_reserve=0x00200000;
    int size_of_stack_commit=0x00001000;
    int size_of_heap_reserve=0x00100000;
    int size_of_heap_commit=0x00001000;
    int loader_flags=0;
    int number_of_data_directory=0x00000010;

    int export_table=0;
    int size_of_export_table=0;
    int import_table=0x00005000;
    int size_of_import_table=0x000002e8;
    int resource_table=0;
    int size_of_resource_table=0;
    int exception_table=0;
    int size_of_exception_table=0;
    int certificate_table=0;
    int size_of_sertificate_table=0;
    int base_relocation_table=0;
    int size_of_base_relocation_table=0;
    int debug=0;
    int size_of_debug=0;
    int architecture_specific_data=0;
    int size_of_architecture_specific_data=0;
    int global_ptr=0;
    int size_of_global_ptr=0;
    int TLS_table=0;
    int size_of_TLS_table=0;
    int load_config_table=0;
    int size_of_load_config_table=0;
    int bound_import=0;
    int size_of_bound_import=0;
    int address_table=0;
    int size_of_address_table=0;
    int delay_import_descriptor=0;
    int size_of_delay_import_descriptor=0;
    int COM_descriptor=0;
    int size_of_COM_descriptor=0;
    int null=0;

    char section_text[]=".text\0\0";
    char section_data[]=".data\0\0";
    char section_rdata[]=".rdata\0";
    char section_bss[]=".bss\0\0\0";
    char section_idata[]=".idata\0";

    int virtual_size=0x00000844;
    int virtual_address=0x00001000;
    int size_of_raw_data=0x00000A00;
    int pointer_to_raw_data=0x00000400;
    int pointer_to_relocation=0;
    int relocations=0;
    int number_of_relocation_relocations=0;
    int characteristics=0x60500060;

    fwrite(dos_header, sizeof(dos_header), 1, f);

    fwrite(windows_header, sizeof(windows_header), 1, f);
    fwrite(machine_i386, sizeof(machine_i386), 1, f);
    fwrite(&number_of_sections, 2, 1, f);
    fwrite(&time_date_stamp, 4, 1, f);
    fwrite(&symbols_pointer, 4, 1, f);
    fwrite(&number_of_symbols, 4, 1, f);
    fwrite(&size_of_optional_header, 2, 1, f);
    fwrite(&flags, 2, 1, f);

    fwrite(&magic, 2, 1, f);
    fwrite(&linker_version, 2, 1, f);
    fwrite(&size_of_code, 4, 1, f);
    fwrite(&size_of_initialized_data, 4, 1, f);
    fwrite(&size_of_uninitialized_data, 4, 1, f);
    fwrite(&address_of_entry_point, 4, 1, f);
    fwrite(&base_of_code, 4, 1, f);
    fwrite(&base_of_data, 4, 1, f);

    fwrite(&image_base, 4, 1, f);
    fwrite(&section_alignment, 4, 1, f);
    fwrite(&file_alignment, 4, 1, f);
    fwrite(&operating_system_version, 4, 1, f);
    fwrite(&image_version, 4, 1, f);
    fwrite(&subsystem_version, 4, 1, f);
    fwrite(&win32_version_value, 4, 1, f);
    fwrite(&size_of_image, 4, 1, f);
    fwrite(&size_of_headers, 4, 1, f);
    fwrite(&check_sum, 4, 1, f);
    fwrite(&subsystem, 2, 1, f);
    fwrite(&dll_characteristics, 2, 1, f);
    fwrite(&size_of_stack_reserve, 4, 1, f);
    fwrite(&size_of_stack_commit, 4, 1, f);
    fwrite(&size_of_heap_reserve, 4, 1, f);
    fwrite(&size_of_heap_commit, 4, 1, f);
    fwrite(&loader_flags, 4, 1, f);
    fwrite(&number_of_data_directory, 4, 1, f);

    fwrite(&export_table, 4, 1, f);
    fwrite(&size_of_export_table, 4, 1, f);
    fwrite(&import_table, 4, 1, f);
    fwrite(&size_of_import_table, 4, 1, f);
    fwrite(&resource_table, 4, 1, f);
    fwrite(&size_of_resource_table, 4, 1, f);
    fwrite(&exception_table, 4, 1, f);
    fwrite(&size_of_exception_table, 4, 1, f);
    fwrite(&certificate_table, 4, 1, f);
    fwrite(&size_of_sertificate_table, 4, 1, f);
    fwrite(&base_relocation_table, 4, 1, f);
    fwrite(&size_of_base_relocation_table, 4, 1, f);
    fwrite(&debug, 4, 1, f);
    fwrite(&size_of_debug, 4, 1, f);
    fwrite(&architecture_specific_data, 4, 1, f);
    fwrite(&size_of_architecture_specific_data, 4, 1, f);
    fwrite(&global_ptr, 4, 1, f);
    fwrite(&size_of_global_ptr, 4, 1, f);
    fwrite(&TLS_table, 4, 1, f);
    fwrite(&size_of_TLS_table, 4, 1, f);
    fwrite(&load_config_table, 4, 1, f);
    fwrite(&size_of_load_config_table, 4, 1, f);
    fwrite(&bound_import, 4, 1, f);
    fwrite(&size_of_bound_import, 4, 1, f);
    fwrite(&address_table, 4, 1, f);
    fwrite(&size_of_address_table, 4, 1, f);
    fwrite(&delay_import_descriptor, 4, 1, f);
    fwrite(&size_of_delay_import_descriptor, 4, 1, f);
    fwrite(&COM_descriptor, 4, 1, f);
    fwrite(&size_of_COM_descriptor, 4, 1, f);
    fwrite(&null, 4, 1, f);
    fwrite(&null, 4, 1, f);

//
    fwrite(section_text, sizeof(section_text), 1, f);
    fwrite(&virtual_size, 4, 1, f);
    fwrite(&virtual_address, 4, 1, f);
    fwrite(&size_of_raw_data, 4, 1, f);
    fwrite(&pointer_to_raw_data, 4, 1, f);
    fwrite(&pointer_to_relocation, 4, 1, f);
    fwrite(&relocations, 4, 1, f);
    fwrite(&number_of_relocation_relocations, 4, 1, f);
    fwrite(&characteristics, 4, 1, f);
//
    virtual_size=0x00000010;
    virtual_address=0x00002000;
    size_of_raw_data=0x00000200;
    pointer_to_raw_data=0x00000E00;
    characteristics=0xC0300040;

    fwrite(section_data, sizeof(section_data), 1, f);
    fwrite(&virtual_size, 4, 1, f);
    fwrite(&virtual_address, 4, 1, f);
    fwrite(&size_of_raw_data, 4, 1, f);
    fwrite(&pointer_to_raw_data, 4, 1, f);
    fwrite(&pointer_to_relocation, 4, 1, f);
    fwrite(&relocations, 4, 1, f);
    fwrite(&number_of_relocation_relocations, 4, 1, f);
    fwrite(&characteristics, 4, 1, f);
//
    virtual_size=0x00000180;
    virtual_address=0x00003000;
    size_of_raw_data=0x00000200;
    pointer_to_raw_data=0x00001000;
    characteristics=0x40600040;

    fwrite(section_rdata, sizeof(section_rdata), 1, f);
    fwrite(&virtual_size, 4, 1, f);
    fwrite(&virtual_address, 4, 1, f);
    fwrite(&size_of_raw_data, 4, 1, f);
    fwrite(&pointer_to_raw_data, 4, 1, f);
    fwrite(&pointer_to_relocation, 4, 1, f);
    fwrite(&relocations, 4, 1, f);
    fwrite(&number_of_relocation_relocations, 4, 1, f);
    fwrite(&characteristics, 4, 1, f);
//
    virtual_size=0x00000054;
    virtual_address=0x00004000;
    size_of_raw_data=0;
    pointer_to_raw_data=0;
    characteristics=0xC0400080;

    fwrite(section_bss, sizeof(section_bss), 1, f);
    fwrite(&virtual_size, 4, 1, f);
    fwrite(&virtual_address, 4, 1, f);
    fwrite(&size_of_raw_data, 4, 1, f);
    fwrite(&pointer_to_raw_data, 4, 1, f);
    fwrite(&pointer_to_relocation, 4, 1, f);
    fwrite(&relocations, 4, 1, f);
    fwrite(&number_of_relocation_relocations, 4, 1, f);
    fwrite(&characteristics, 4, 1, f);
//
    virtual_size=0x000002E8;
    virtual_address=0x00005000;
    size_of_raw_data=0x00000400;
    pointer_to_raw_data=0x00001200;
    characteristics=0x40300040;

    fwrite(section_idata, sizeof(section_idata), 1, f);
    fwrite(&virtual_size, 4, 1, f);
    fwrite(&virtual_address, 4, 1, f);
    fwrite(&size_of_raw_data, 4, 1, f);
    fwrite(&pointer_to_raw_data, 4, 1, f);
    fwrite(&pointer_to_relocation, 4, 1, f);
    fwrite(&relocations, 4, 1, f);
    fwrite(&number_of_relocation_relocations, 4, 1, f);
    fwrite(&characteristics, 4, 1, f);

    int i;
    for(i=0; i<0x000015f0-0x0000230; i++)
        fputc(0x01, f);

    fclose(f);
}
