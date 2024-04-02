# Beck's JSON library

Licensed GPLv3. See `LICENSE.txt`.

Basic usage example:
```
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
```

## Classes

`JSON::JSON`

Any JSON Value, including Objects and Arrays.

Can be initialized with any supported type.

`JSON::JSONObject`

A JSON Object.

`JSON::JSONArray`

A JSON Array.


## Static Methods

`JSON::JSON JSON::deserialize(const char *s);`

`JSON::JSON JSON::deserialize(std::string s);`

De-serialize a JSON string into a JSON Value.


## JSONArray Class Operator Methods

`JSON *operator[](size_t i)`

Return a pointer to the JSON Value at the specified index of this JSON Array. Returns NULL if index is outside the bounds of the array.

`JSON *operator+=(JSON object)`

Append the JSON Value to this JSON Array and return a pointer to it.


## JSONArray Class Methods

`void remove(size_t i);`

Remove the JSON Value at the specified index of this JSON Array.


## JSON Class Operator Methods

`JSON *operator[](size_t i)`

Return a pointer to the JSON Value at the specified index of this JSON Array. Returns NULL if not a JSON Array.

`JSON *operator[](std::string key)`

Return a pointer to the JSON Value at the specified key of this JSON Object. Returns NULL if not a JSON Object.

`JSON *operator[](char *key)`

Return a pointer to the JSON Value at the specified key of this JSON Object. Returns NULL if not a JSON Object.


## JSON Class Methods

`const char *serialize();`

Serialize a JSON Value into a JSON string.

`Type getType();`

Return the type associated with this JSON Value.

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

