#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "smbios_parse.h"

void smbios_skip(byte_t **x) {
	// Offset length of header
	*x += ((struct header *) x)->length;
	size_t len = 0;
	
	// If value at ptr is 0, then offset by 2 and return,
	//  else, get the length of the region in memory untl
	//  a null byte is encountered, add that to
	if (**x == 0) x += 2;
	else do {
		len  = strlen((const char *) x);
		*x += len + 1;
	} while(len > 0);
}


struct header* smbios_extract_values(byte_t type, void *ptr) {
	ptr = NULL;
	
	// Find the header for the requested type;
	struct Entry *entry = smbios_first;
	while(entry->next != NULL){
		if(entry->header->type == type) break;
		entry = entry->next;
	}
	
	// If it wasn't found, dump out a NULL pointer;
	//  Leave checking to the user application!
	if(entry->header->type != type){
		return ptr;
	}
	
	// Offset pointer beyond the header, to the region of
	//  memory containing the values
	ptr = ((byte_t *) entry->header) + entry->header->length;
	return (struct header *) ptr;
}


void smbios_parse(const void *raw_smbios, size_t size) {
	byte_t *x;
	
	smbios_raw_size = size;
	smbios_raw_data = malloc( smbios_raw_size );
	memcpy(smbios_raw_data, raw_smbios, smbios_raw_size);
	
	// Potential mem leak here if smbios_first has already been init.
	smbios_first = malloc( sizeof(Entry) );
	smbios_current_entry = smbios_first;

	x = smbios_raw_data;
	Entry *prev = NULL;
	while( (size_t)(x - smbios_raw_data) < smbios_raw_size ) {
		if( prev == NULL ){
			// First Run
			smbios_first->header = (struct header *)x;
			prev = smbios_first;
			continue;
		}
		
		// Ahoy. Potential Memory Leak Here. Please see
		//  clear()!
		Entry *e = malloc( sizeof(Entry) );
		e->header = (struct header *)x;
		prev->next = e;
		
		prev = e;
		smbios_skip(&x);
	}
}


void smbios_clear() {
	// Delete all Entry instances
	Entry *entry = smbios_first;
	while( entry->next != NULL ){
		entry = entry->next;
		free(entry);
	}
	
	// Reset raw data
	free(smbios_raw_data);
	smbios_raw_size = 0;
};


byte_t smbios_current_type(){
	return smbios_current_entry->header->type;
}


void* smbios_current_structure(){
	return ((byte_t *) smbios_current_entry->header) + smbios_current_entry->header->length;
}


int smbios_iterate(){
	int ret = 0;
	
	if( smbios_current_entry->next != NULL ){
		smbios_current_entry = smbios_current_entry->next;
		ret = 1;
	} else {
		smbios_current_entry = smbios_first;	
	}
	
	return ret;
}


void smbios_iterate_reset() {
	smbios_current_entry = smbios_first;
}