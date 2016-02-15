#include <stdio.h>
#include <IOKit/IOKitLib.h>
#include "lib/smbios_parse.h"

void print_smbios_entry(struct SMBEntryPoint* e);
void print_smbios_data(byte_t* data);

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
	} else print_smbios_entry((struct SMBEntryPoint *) CFDataGetBytePtr(dataRef));


	// Retrieve and Parse SMBIOS Data

	dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
		CFSTR("SMBIOS"), kCFAllocatorDefault, kNilOptions);

	if (!dataRef) {
		printf("Unable to retrieve SMBIOS data!\n");
		return -1;
	} else print_smbios_data((byte_t*) CFDataGetBytePtr(dataRef));

	return 0;
}

void print_smbios_entry(struct SMBEntryPoint* e){
	printf("Address of SMBIOS Entry Point: %p.\n", e);
}

void print_smbios_data(byte_t* data){
	printf("Address of SMBIOS Data: %p.\n", data);
}