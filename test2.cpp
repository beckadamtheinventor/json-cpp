#include "json/json.hpp"
int main(int argc, char **argv) {
	const char *data = "{\"hello\": \"world\"}";
	// deserialize the C string "data" into a JSON object
	JSON::JSON obj = JSON::JSON::deserialize(data);
	// create a JSON Array starting with 8 members, and assign to the "messages" key of JSON object "obj"
	JSONArray *arr = new JSONArray(8);
	for (int i=0; i<8; i++) {
		arr->get(i).setString("Hello World");
	}
	obj["messages"] = arr;
	printf("%s\n", obj.serialize());
	return 0;
}
