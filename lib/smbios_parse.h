#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t   byte_t;
typedef uint16_t  word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

typedef byte_t str_id;
typedef byte_t enum_t;

#include "smbios_structures.h"

// We implement an internal Linked List after parsing
//  the SMBIOS table.
typedef struct Entry {
	struct Entry* next;
	struct header* header;
} Entry;




/*
 * Internal properties
 */
byte_t *smbios_raw_data;
size_t	smbios_raw_size;
Entry *smbios_first;
Entry *smbios_current_entry;



/*
 * Function Prototypes
 */


void smbios_skip(byte_t **x);

struct header* smbios_extract_values(byte_t type, void *ptr);

void smbios_parse(const void *raw_smbios, size_t size);

void smbios_clear();

byte_t smbios_current_type();

void* smbios_current_structure();

int smbios_iterate();

void smbios_iterate_reset();