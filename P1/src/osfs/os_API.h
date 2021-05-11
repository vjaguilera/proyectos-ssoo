#include "../structs/osFile.h"

// GENERALES

void os_mount(char* diskname, int partition);

void os_bitmap(unsigned num);

int os_exists(char* filename);

void os_ls();

// Master Boot Table

void os_mbt();

void os_create_partition(int id, int size);

void os_delete_partition(int id);

void os_reset_mbt();

// Manejo archivos

osFile* os_open(char* filename, char mode);

int os_read(osFile* file_desc, void* buffer, int nbytes);

int os_write(osFile* file_desc, void* buffer, int nbytes);

int os_close(osFile* file_desc);

int os_rm(char* filename);

