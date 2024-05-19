#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

char* append_char(const char* str, char c) {
    size_t len = strlen(str);
    char* new_str = (char*)malloc(len + 2);

    if (new_str == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(new_str, str);
    new_str[len] = c;
    new_str[len + 1] = '\0';

    return new_str;
}

char* slice_string(const char* str, int start, int end) {
    // Ensure the start and end indices are within the bounds of the string
    int strLength = strlen(str);
    if (start < 0 || end <= start || start >= strLength || end > strLength) {
        return NULL;
    }

    // Calculate the length of the slice
    int sliceLength = end - start;

    // Allocate memory for the new sliced string (+1 for the null terminator)
    char* slice = (char*)malloc((sliceLength + 1) * sizeof(char));
    if (slice == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Copy the elements from the original string to the new string
    strncpy(slice, str + start, sliceLength);

    // Add the null terminator
    slice[sliceLength] = '\0';

    return slice;
}

char* parse_string(const char* bencoded_value) {
    int length = atoi(bencoded_value);
    const char* colon_index = strchr(bencoded_value, ':');

    if (!(colon_index != NULL)) {
        fprintf(stderr, "Invalid encoded value: %s\n", bencoded_value);
        exit(EXIT_FAILURE);
    }

    const char* start = colon_index + 1;
    char* decoded_str = (char*)malloc(length + 1);
    strncpy(decoded_str, start, length);
    decoded_str[length] = '\0';
    return decoded_str;
}

char* parse_int(const char* bencoded_value) {
    char* decoded_str = "";
    int i = 1;
    while (true) {
	    if (bencoded_value[i] == 'e') {
	        break;
	    }
	    decoded_str = append_char(decoded_str, bencoded_value[i]);
	    i++;
    }
    return decoded_str;
}

char* decode_basic(const char* bencoded_value) {
    char* decoded_str;
    if (is_digit(bencoded_value[0])) {
        decoded_str = parse_string(bencoded_value);
    }
    else if (bencoded_value[0] == 'i') {
        decoded_str = parse_int(bencoded_value);
    }
    else {
        fprintf(stderr, "Unsupported data\n");
        exit(EXIT_FAILURE);
    }
    return decoded_str;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: RogRent <command> <args>\n");
        return EXIT_FAILURE;
    }

    const char* command = argv[1];

    if (strcmp(command, "decode") == 0) {
        const char* encoded_str = argv[2];
        char* decoded_str = decode_basic(encoded_str);
        printf("\"%s\"\n", decoded_str);
        free(decoded_str);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

