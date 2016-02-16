#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "smbios_parse.h"

void smbios_skip(SMBByte **x) {
	// Offset length of header
	*x += ((SMBStructHeader *) x)->length;
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


SMBValue* smbios_search(SMBByte type) {
	SMBValue *entry = smbios_first;
	while(entry->next != NULL){
		if (entry->header->type == type) {
			return entry;
		}
	}

	return (SMBValue *)NULL;
}


void smbios_parse(const void *raw_smbios, size_t size) {
	SMBByte *x;
	
	smbios_raw_size = size;
	smbios_raw_data = malloc( smbios_raw_size );
	memcpy(smbios_raw_data, raw_smbios, smbios_raw_size);
	
	// Potential mem leak here if smbios_first has already been init.
	smbios_first = malloc( sizeof(SMBValue) );
	smbios_current_entry = smbios_first;

	x = smbios_raw_data;
	SMBValue *prev = NULL;
	while( (size_t)(x - smbios_raw_data) < smbios_raw_size ) {
		if( prev == NULL ){
			// First Run
			smbios_first->header = (SMBStructHeader *)x;
			prev = smbios_first;
			continue;
		}
		
		// Ahoy. Potential Memory Leak Here. Please see
		//  clear()!
		SMBValue *e = malloc( sizeof(SMBValue) );
		e->header = (SMBStructHeader *)x;
		prev->next = e;
		
		prev = e;
		smbios_skip(&x);
	}
}


void smbios_clear() {
	// Delete all Entry instances
	SMBValue *entry = smbios_first;
	while( entry->next != NULL ){
		entry = entry->next;
		free(entry);
	}
	
	// Reset raw data
	free(smbios_raw_data);
	smbios_raw_size = 0;
};


SMBByte smbios_current_type(){
	return smbios_current_entry->header->type;
}


void* smbios_current_structure(){
	return ((SMBByte *) smbios_current_entry->header) + smbios_current_entry->header->length;
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