# Beck's JSON library

Licensed GPLv3. See `LICENSE.txt`.

Basic usage example:
```
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
```

## Classes

`JSON::JSON`

Any JSON Value, including Objects and Arrays.

Can be initialized with any supported type.

`JSON::JSONObject`

A JSON Object.

`JSON::JSONArray`

A JSON Array.


## JSON Static Methods

`JSON::JSON::deserialize`

De-serialize a JSON string into a JSON Value. (`JSON::JSON`)


## JSON Class Operator Methods

`JSON *operator[](size_t i)`

Return a pointer to the JSON value at the specified index of this JSON Array. Returns NULL if not a JSON Array.

`JSON *operator[](std::string key)`

Return a pointer to the JSON value at the specified key of this JSON Object. Returns NULL if not a JSON Object.

`JSON *operator[](char *key)`

Return a pointer to the JSON value at the specified key of this JSON Object. Returns NULL if not a JSON Object.


## JSON Class Methods

`const char *serialize();`

Serialize a JSON value into a JSON string.

`Type getType();`

Return the type associated with this JSON value.

`bool contains(const char *key);` 

Returns true if this is a string-indexed JSON object and contains key `key`.

`const char *getCString();`

Return the C string associated with this object. Returns NULL if this object is not a string.

`const char *getString();`

Return the `std::string` associated with this object. Returns NULL if this object is not a string.

`double getFloat();`

Return the floating point value associated with this object. Returns NULL if this object is not a float.

`long long getInteger();`

Return the integer value associated with this object. Returns NULL if this object is not an integer.

`size_t getUnsigned();`

Return the unsigned integer value associated with this object. Returns NULL if this object is not an integer.

`bool getBoolean();`

Return the boolean value associated with this object. Returns NULL if this object is not a boolean.

`bool isNull();`

Returns true if this object is null.

`JSONArray *getArray()`

Return the JSON Array value associated with this object. Returns NULL if this object is not a JSON Array.

`JSONObject *getObject()`

Return the JSON Object value associated with this object. Returns NULL if this object is not a JSON Object.

`size_t getArrayLength()`

Return the number of entries in this array. Returns 0 if this object is not a JSON Array.

`size_t getObjectLength()`

Return the number of entries in this object. Returns 0 if this object is not a JSON Object.

`JSONMap *setObject(JSONMap& o)`

`JSONMap *setObject(JSONMap *o)`

Set this JSON Value to a JSON Object.

`JSONArray *setArray(JSONArray& a)`

`JSONArray *setArray(JSONArray *o)`

Set this JSON Value to a JSON Array.

`double setFloat(double d)`

Set this JSON Value to a floating point value.

`long long setInteger(long long i)`

Set this JSON Value to an integer value.

`const char *setString(const char *s)`

`std::string setString(std::string s)`

Set this JSON Value to a string value.

`bool setBoolean(bool v)`

Set this JSON Value to a boolean value.

`void setNull(void)`

Set this JSON Value to a null value.

