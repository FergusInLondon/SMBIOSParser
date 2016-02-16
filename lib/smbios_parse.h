#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "smbios.h"

// Linked List Element
typedef struct SMBValue {
	struct SMBValue*	next;
	SMBStructHeader*	header;
	void*				structure;
} SMBValue;

// Internals (make static after refactor.)
SMBByte		*smbios_raw_data;
size_t		smbios_raw_size;
SMBValue 	*smbios_first;
SMBValue 	*smbios_current_entry;


//
// Parsing functions; parses the SMBIOS data and generates
//  a linked list containing header & structure pointers.
//
// Status:
//  The parser code is.. ermm.. interesting.

void smbios_parse(const void *raw_smbios, size_t size);
void smbios_skip(SMBByte **x);
void smbios_clear();


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
// Status:
//  Needs to be cleaned, but as it simply traverses a linked
//   list, I can't see it being too hairy.

SMBByte smbios_current_type();
void	*smbios_current_structure();
int		smbios_iterate();
void	smbios_iterate_reset();








