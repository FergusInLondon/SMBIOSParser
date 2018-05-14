#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "smbios.h"

// Linked List Element
typedef struct SMBValue {
	struct SMBValue*	next;
	SMBStructHeader*	structure;
} SMBValue;

// Internals (make static after refactor.)
SMBByte		*smbios_dup_data;
SMBValue 	*smbios_first;
SMBValue 	*smbios_current_entry;
size_t		smbios_raw_size;
uint8_t		smbios_counter;


//
// Parsing functions; parses the SMBIOS data and generates
//  a linked list containing header & structure pointers.
//
// Status:
//  The parser code is.. ermm.. interesting.

void smbios_parse(const void *raw_smbios, size_t size);
void smbios_fastforward(SMBByte **cursor);
void smbios_destroy();


//
// Search function, returns a pointer to the header entry
//  and modifies *ptr to point directly to the entry.
//
// Status:
//  Works by iterating through all of the entries, this could
//   be avoided by using some form of look-up table.

SMBValue* smbios_search(SMBByte type);


//
// Iterator Methods
//  Parser exposes an iterator API for accessing all parsed
//   entries.

bool				smbios_iterate();
void				smbios_iterate_reset();
SMBByte				smbios_current_type();
SMBStructHeader*	smbios_current_structure();
