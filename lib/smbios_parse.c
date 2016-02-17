#include "smbios_parse.h"

SMBValue* smbios_search(SMBByte type) {
	SMBValue *entry = smbios_first;
	while (entry->next != NULL) {
		if (entry->structure->type == type) {
			return entry;
		}
	}

	return (SMBValue *)NULL;
}

void smbios_fastforward(SMBByte **cursor) {
	// If value at ptr is 0, then offset by 2 and return,
	//  else, get the length of the region in memory untl
	//  a null byte is encountered, add that to
	if (**cursor == 0) cursor += 2;
	else do {
		len  = strlen((const char *) cursor);
		*cursor += len + 1;
	} while(len > 0);
}

void smbios_parse(const void *raw_smbios, size_t size) {

	// Duplicate SMBIOS structure - potentially not required
	smbios_dup_data = malloc(size);
	memcpy(smbios_dup_data, raw_smbios, size);

	// Potential mem leak here if smbios_first has already been init.
	smbios_counter = 0;
	if (smbios_first != NULL) free(smbios_first);
	smbios_first = malloc(sizeof(SMBValue));
	smbios_current_entry = smbios_first;

	// Create first SMBValue entry
	SMBByte *cursor = smbios_dup_data;
	SMBValue *prev = malloc(sizeof(SMBValue));
	prev->structure = (SMBStructHeader *)smbios_dup_data;
	smbios_counter = 1;

	// Skip strings and null bytes
	cursor += (size_t)prev->structure->length;
	smbios_fastforward(&cursor);

	// Iterate and repeat through the rest of the table entries
	while (cursor < (smbios_dup_data + size)) {
		SMBStructHeader *head = (SMBStructHeader *)cursor;

		// Create new SMBValue element + initialise values
		SMBValue *e = malloc(sizeof(SMBValue));
		e->structure = head;
		prev->next = e;
		prev = e;

		cursor += (size_t)prev->structure->length;;

		smbios_fastforward(&cursor);
		smbios_counter++;
	}
}


void smbios_destroy() {
	// Delete all Entry instances
	SMBValue *entry = smbios_first;
	while (entry->next != NULL) {
		// How the hell was this ever meant to work?
		entry = entry->next;
		free(entry);
	}
	
	// Reset raw data
	free(smbios_dup_data);
	smbios_raw_size = 0;
};


SMBByte smbios_current_type(){
	return smbios_current_entry->structure->type;
}


void* smbios_current_structure(){
	return ((SMBByte *) smbios_current_entry->structure) + smbios_current_entry->structure->length;
}


int smbios_iterate(){
	int ret = 0;
	
	if (smbios_current_entry->next != NULL) {
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