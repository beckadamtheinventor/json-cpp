#include "json/json.hpp"
int main(int argc, char **argv) {
	const char *data = "{\"hello\": \"world\"}";
	// deserialize the C string "data" into a JSON object
	JSON::JSON obj = JSON::JSON::deserialize(data);
	// create a JSON Array starting with 8 members,
	JSON::JSONArray *arr = new JSON::JSONArray(8);
	for (int i=0; i<8; i++) {
		arr->append(JSON::JSON("Hello World"));
	}
	// assign to the "messages" key of JSON object "obj"
	obj["messages"]->setArray(arr);
	printf("%s\n\n", obj.serialize());
	return 0;
}
