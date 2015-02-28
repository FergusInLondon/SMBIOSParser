

/*
 * Type definitions
 */

typedef uint8_t   byte_t;
typedef uint16_t  word_t;
typedef uint32_t dword_t;

#ifdef _MSC_VER
typedef __int64 qword_t;
#else
#ifdef INT64_C
typedef uint64_t qword_t;
#else
typedef (unsigned long long int) qwordt_t;
#endif
#endif

typedef byte_t str_id;
typedef byte_t enum_t;


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



/*
 * SMBIOS Table Structures
 */


enum {
	smbios_type_bios_info                   =   0, // Required
	smbios_type_system_info                 =   1, // Required
	smbios_type_baseboard_info              =   2,
	smbios_type_module_info                 =   2,
	smbios_type_system_enclosure            =   3, // Required
	smbios_type_system_chassis              =   3, // Required
	smbios_type_processor_info              =   4, // Required
	smbios_type_memory_controller_info      =   5, // Obsolete
	smbios_type_memory_module_info          =   6, // Obsolete
	smbios_type_cache_info                  =   7, // Required
	smbios_type_port_connector_info         =   8,
	smbios_type_system_slots                =   9, // Required
	smbios_type_onboard_device_info         =  10, // Obsolete
	smbios_type_oem_strings                 =  11,
	smbios_type_system_config_options       =  12,
	smbios_type_language_info               =  13,
	smbios_type_group_associations          =  14,
	smbios_type_system_event_log            =  15,
	smbios_type_memory_array                =  16, // Required
	smbios_type_memory_device               =  17, // Required
	smbios_type_memory_error_info_32bit     =  18,
	smbios_type_memory_array_mapped_addr    =  19, // Required
	smbios_type_memory_device_mapped_addr   =  20,
	smbios_type_builtin_pointing_device     =  21,
	smbios_type_portable_battery            =  22,
	smbios_type_system_reset                =  23,
	smbios_type_hardware_security           =  24,
	smbios_type_system_power_controls       =  25,
	smbios_type_voltage_probe               =  26,
	smbios_type_cooling_device              =  27,
	smbios_type_temperature_probe           =  28,
	smbios_type_electrical_current_probe    =  29,
	smbios_type_out_of_band_remote_access   =  30,
	smbios_type_bis_entry_point             =  31, // Required
	smbios_type_system_boot_info            =  32, // Required
	smbios_type_memory_error_info_64bit     =  33,
	smbios_type_management_device           =  34,
	smbios_type_management_device_component =  35,
	smbios_type_management_device_threshold =  36,
	smbios_type_memory_channel              =  37,
	smbios_type_ipmi_device_info            =  38,
	smbios_type_system_power_supply         =  39,
	smbios_type_additional_info             =  40,
	smbios_type_onboard_device_extinfo      =  41,
	smbios_type_management_controller_host  =  42,
	smbios_type_inactive                    = 126,
	smbios_type_end_of_table                = 127, // Always last structure
};


#pragma pack(push, 1)
struct header {
	byte_t type;
	byte_t length;
	word_t handle;
};

struct string_list {
	byte_t count;
};

struct bios_info {
	// 2.0
	str_id  vendor;
	str_id  version;
	word_t  starting_segment;
	str_id  release_date;
	byte_t  rom_size;
	qword_t characteristics;
	// 2.4
	byte_t ext_char1;
	byte_t ext_char2;
	byte_t sb_major;
	byte_t sb_minor;
	byte_t ec_major;
	byte_t ec_minor;
};

struct system_info {
	// 2.0
	str_id manufacturer;
	str_id product_name;
	str_id version;
	str_id serial_number;
	// 2.1
	struct {
		dword_t time_low;
		word_t  time_mid;
		word_t  time_hi_and_version;
		byte_t  clock_seq_hi_and_reserved;
		byte_t  clock_seq_low;
		byte_t  node[6];
	} uuid;
	enum_t wakeup_type;
	// 2.4
	str_id sku;
	str_id family;
};

struct system_chassis {
	// 2.0
	str_id  manufacturer;
	byte_t  type;
	str_id  version;
	str_id  serial_number;
	str_id  assert_tag;
	// 2.1
	enum_t  bootup_state;
	enum_t  power_supply_state;
	enum_t  thermal_state;
	enum_t  security_status;
	// 2.3
	dword_t oem;
	byte_t  height;
	byte_t  cords;
};

struct proc_info {
	// 2.0
	str_id  socket_designation;
	enum_t  type;
	enum_t  family;
	str_id  manufacturer;
	qword_t id;
	str_id  version;
	byte_t  voltage;
	word_t  clock;
	word_t  speed_max;
	word_t  speed_cur;
	byte_t  status;
	enum_t  upgrade;
	// 2.1
	word_t  L1;
	word_t  L2;
	word_t  L3;
	// 2.3
	str_id  serial_number;
	str_id  assert_tag;
	str_id  part_number;
	// 2.5
	byte_t  cores;
	byte_t  cores_enabled;
	byte_t  threads;
	word_t  characteristics;
	enum_t  family2;
};

struct cache_info {
	// 2.0
	str_id socket_designation;
	word_t config;
	word_t size_max;
	word_t size_cur;
	word_t sram_supported;
	word_t sram_cur;
	// 2.1
	byte_t speed;
	enum_t error_correction_type;
	enum_t system_cache_type;
	enum_t associativity;
};

struct slot {
	// 2.0
	str_id slot_designation;
	enum_t type;
	enum_t data_bus_width;
	enum_t current_usage;
	enum_t length;
	word_t id;
	byte_t characteristics;
	// 2.1
	byte_t characteristics2;
	// 2.6
	word_t segment_group;
	byte_t bus;
	byte_t device;
};

typedef struct string_list oem_strings;
typedef struct string_list system_config_options;

struct lang_info {
	byte_t installed_langs;
	byte_t flags;
	byte_t reserved[15];
	str_id current_lang;
};

struct mem_arr {
	// 2.1
	enum_t  location;
	enum_t  use;
	enum_t  error_correction;
	dword_t capacity;
	word_t  error_info_handle;
	word_t  devices_number;
	// 2.7
	qword_t capacity_ext;
};

struct mem_device {
	// 2.1
	word_t  mem_arr_handle;
	word_t  mem_arr_error_info_handle;
	word_t  total_width;
	word_t  data_width;
	word_t  size;
	enum_t  form_factor;
	byte_t  device_set;
	str_id  device_locator;
	str_id  bank_locator;
	enum_t  type;
	word_t  type_detail;
	// 2.3
	word_t  speed;
	str_id  manufacturer;
	str_id  serial_number;
	str_id  assert_tag;
	str_id  part_number;
	// 2.6
	byte_t  attributes;
	// 2.7
	dword_t size_ext;
	word_t  clock_speed;
	// 2.8
	word_t  voltage_min;
	word_t  voltage_max;
	word_t  voltage;
};
#pragma pack(pop)