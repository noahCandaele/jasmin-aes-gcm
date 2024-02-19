#define BASE_16 16

#define NB_BYTES_64_BITS 8
#define NB_BYTES_128_BITS 16
#define NB_BYTES_192_BITS 24
#define NB_BYTES_256_BITS 32

#define RED_TEXT "\x1b[31m"
#define YELLOW_TEXT "\x1b[33m"
#define GREEN_TEXT "\x1b[32m"
#define BLUE_TEXT "\x1b[34m"
#define RESET_COLOR "\x1b[0m"

#define CODE_SUCCESS 0
#define CODE_FAIL 1
#define CODE_WARNING 2
#define CODE_INFO 3

void print_test_return_status(int return_code);

void print_uint8_array_as_binary(uint8_t* arr, size_t size, bool with_spaces);
void print_uint8_array_as_hex(uint8_t *arr, size_t size, bool with_spaces);
void print_uint8_array_as_ascii(uint8_t* arr, size_t size, bool with_spaces);

size_t nb_bytes_hex_string(char* hex_string);
bool compare_uint8_arrays(const uint8_t* array1, const uint8_t* array2, size_t size);

void convert_hex_string_to_uint8_array(char* hex_string, uint8_t* uint8_array, size_t uint8_array_size);