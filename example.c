#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include "lib/smbios_parse.h"

void print_smbios_entry(SMBEntryPoint* e);
void print_smbios_data(SMBByte* data, size_t len);

int main(int argc, const char * argv[]) {

	CFDataRef		dataRef;
	io_service_t	service = MACH_PORT_NULL;
	mach_port_t		myMasterPort;	

	IOMasterPort(MACH_PORT_NULL, &myMasterPort);
	service = IOServiceGetMatchingService(myMasterPort, IOServiceMatching("AppleSMBIOS"));
	if (!service) {
		printf("No \"AppleSMBIOS\" IOService in IORegistry.\n");
		return -1;
	}


	// Retrieve and Parse SMBIOS Entry Point data

	dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
		CFSTR("SMBIOS-EPS"), kCFAllocatorDefault, kNilOptions);

	if (!dataRef) {
		printf("Unable to retrieve SMBIOS entry point!\n");
		return -1;
	} else print_smbios_entry((SMBEntryPoint *) CFDataGetBytePtr(dataRef));


	// Retrieve and Parse SMBIOS Data

	dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
		CFSTR("SMBIOS"), kCFAllocatorDefault, kNilOptions);

	if (!dataRef) {
		printf("Unable to retrieve SMBIOS data!\n");
		return -1;
	} else print_smbios_data((SMBByte*) CFDataGetBytePtr(dataRef), (size_t)CFDataGetLength(dataRef));

	return 0;
}

void print_smbios_entry(struct SMBEntryPoint* e){
	printf("Address of SMBIOS Entry Point: %p.\n", e);
	printf("Version information:\n");
	printf("\t- Major Version: %d\n", e->majorVersion);
	printf("\t- Minor Version: %d\n", e->minorVersion);
}

void print_smbios_data(SMBByte* data, size_t len){
	printf("Address of SMBIOS Data: %p.\n", data);

	smbios_parse(data, len);

	// Although, always check any returned pointers - don't assume they're GOOD
	SMBValue *val = smbios_search(kSMBTypeProcessorInformation);	
	if (val != NULL) {
		SMBProcessorInformation *procInfo = (SMBProcessorInformation *)val->structure;
		printf("Socket Description: %hhu\n", procInfo->socketDesignation);
	}
	
	// Clear up and free that mem.
	smbios_clear();
}