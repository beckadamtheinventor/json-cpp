
#include "json/json.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        JSON::JSON obj = JSON::JSON::deserialize("{\"hello\": \"world\"}");
        printf("%s\n", obj.serialize());
    } else {
        FILE *fd = fopen(argv[1], "r");
        if (fd == NULL) {
            printf("File \"%s\" not found.\n");
            return -1;
        }
        fseek(fd, 0, SEEK_END);
        size_t len = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        char *data = (char*) malloc(len);
        if (data != NULL)
            fread(data, len, 1, fd);
        fclose(fd);
        if (data == NULL) {
            printf("Failed to malloc space (%llu bytes) for file\n", len);
            return 1;
        }
        JSON::JSON obj = JSON::JSON::deserialize(data);
        free(data);
        printf("%s\n", obj.serialize());
    }
    return 0;
}
