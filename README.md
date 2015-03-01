## A Pure C SMBIOS Parser

This is a simple parser for SMBIOS written in C. It's built upon the work done in C++ by [@ZipFile](https://github.com/ZipFile) available at this [repo](https://github.com/ZipFile/smbios); I've removed the C++ specific features (i.e Vectors, struct inheritence..) and simplified it somewhat, as well as bolting on a bit of a simple iterator.

**Still very much a work in progress; i.e it compiles but how well it works is anyones guess; I need to do some further work on this.**


### Functions

    void smbios_parse(const void *raw_smbios, size_t size);

Accepts a pointer to an SMBIOS structure, and the size of the structure.

This method parses the structure, constructing an internal linked list containing the different sections which are available.

    struct header* smbios_extract_values(byte_t type, void *ptr);

Retrieves the values associated with a specific section.

Accepts a 'type' value and a pointer to a ```struct``` that will hold the values read from the SMBIOS.

Type values and the structs are declared in the header file.

    void smbios_clear();

Destroys the internal linked list and frees any memory that's been used.

    void smbios_skip(byte_t **x);

[**Internal**] This has no need to be called by any external code relying upon this parser.

Simply skips to the next SMBIOS section.


#### Iterator methods

The above methods give no mechanism of iterating through the available sections of the SMBIOS structure. 

For this reason, I've implemented a simple iterator to allow available sections to be accessed easily.

    int smbios_iterate();

Moves on to the next section of the SMBIOS, as found in the internal linked list.

To allow this function to be called in ```while()``` loops, ```0``` will be returned when there are no more sections available, and ```1``` will be returned when the next section has successfully been set.

    void smbios_iterate_reset();

This resets the iterator, and thus should be called after iterating through the SMBIOS sections.

It works by simply resetting the internal pointer to the initial node of our linked list.

    byte_t smbios_current_type();

Returns the ```type``` value associated with the current section.

    void* smbios_current_structure();

Returns a pointer to a structure containing the values associated with the current section.


##### Example

Here's an example of how this code could be used for SMBIOS parsing, for Mac OS X. 

The below example comes from a work in progress by [@perfaram](https://github.com/perfaram/) - with the exception of the calls to ```smbios_*``` functions.

If using Mac OS X and Objective-C, a nicer implementation could be done using an alternative data structure - an ```NSMutableDictionary``` - as opposed to the internal Linked List found in this parser.


    #include <stdint.h>
    #include <stddef.h>
    #include <stdlib.h>
    #include <string.h>
    #import <Foundation/Foundation.h>

    #import "smbios_header.h"

    int main(int argc, const char * argv[]) {

        mach_port_t 					myMasterPort;
        CFMutableDictionaryRef        	myMatchingDictionary;
        kern_return_t					result;
        io_object_t                   	foundService;
        
        IOMasterPort(MACH_PORT_NULL, &myMasterPort);
        
        myMatchingDictionary = IOServiceMatching("AppleSMBIOS");
        foundService = IOServiceGetMatchingService( myMasterPort, myMatchingDictionary );
        
        IOMasterPort(MACH_PORT_NULL, &myMasterPort);
        
        myMatchingDictionary = IOServiceMatching("AppleSMBIOS");
        foundService = IOServiceGetMatchingService( myMasterPort, myMatchingDictionary );
        if (foundService == 0) {
            printf("No \"AppleSMBIOS\" IOService in IORegistry");
            return false;
        }
        
        CFMutableDictionaryRef    properties    = NULL;
        CFDataRef                 smbiosdata;
        
        result = IORegistryEntryCreateCFProperties( foundService,
                                                   &properties,
                                                   kCFAllocatorDefault,
                                                   kNilOptions );
        if (result != kIOReturnSuccess) {
            printf("No data in \"AppleSMBIOS\" IOService");
            return false;
        }
        
        result = CFDictionaryGetValueIfPresent( properties,
                                               CFSTR("SMBIOS"),
                                               (const void **)&smbiosdata );
        if (result != true) {
            printf("No \"SMBIOS\" property in \"AppleSMBIOS\" IOService");
            return false;
        }
        
        /*
        ** BEGIN SMBIOS PARSING; Parser functionality ->
        */
        
        
        // Parse the SMBIOS values
        smbios_parse((const char *)smbiosdata, (size_t)CFDataGetLength(smbiosdata));

        // Iterate through all the retrieved values
        while( smbios_iterate() ){
            printf("Type: %i\t Address: %p\n", smbios_current_type(), smbios_current_structure());
            
            // Check the current type; if it's a mem device...
            if( smbios_current_type() == smbios_type_memory_device ){
                // Then retrieve that region of memory in to a struct
                struct mem_device *values = smbios_current_structure();
                printf("Memory Device Serial Number: %hhu", values->serial_number);
            }
        }
        smbios_iterate_reset();

        
        // Alternatively, without the iterator, we can grab specific values:
        struct proc_info *pInfo;
        smbios_extract_values(smbios_type_processor_info, pInfo);
        
        // Although, always check any returned pointers - don't assume they're GOOD
        if( pInfo != NULL){
            printf("Socket Description: %hhu\n", pInfo->socket_designation);
        }
        
        // Clear up and free that mem.
        smbios_clear();

        
        /*
        ** END SMBIOS PARSING; End of Parser functionality
        */
        
        return 0;
    }