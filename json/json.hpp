#ifndef __JSON_H__
#define __JSON_H__

#include <csignal>
#include <cstring>
#include <exception>
#include <map>
#include <string.h>
#include <string>

#define JSONMap HashedSymList<JSON>

namespace JSON {
    template<class V>
    class HashedSymList {
        constexpr static const size_t MIN_NUM_ENTRIES = 10;
		static size_t Hash(const char *s) {
            if (s == NULL) {
                return 0;
            }
            size_t h = 0;
            for (size_t i=0; s[i]; i++) {
                h = h*129 ^ ~s[i];
            }
            return h;
		}
        public:
        class Symbol {
            public:
            const char *key;
            size_t hash;
            V value;
            Symbol(uint32_t hash, char *key) {
                this->key = key;
                this->hash = hash;
            }
            Symbol(char *key) : Symbol(Hash(key), key) {};
            Symbol() : Symbol(0, nullptr) {};

            Symbol& operator=(const Symbol& sym) {
                hash = sym.hash;
                key = sym.key;
                value = sym.value;
            }
            V& operator=(const V& val) {
                value = val;
            }

            operator V() const {return value;}
        };

        private:
        Symbol *entries = nullptr;
        size_t max_entries;
        size_t length = 0;

        public:

        HashedSymList() : HashedSymList(MIN_NUM_ENTRIES) {}
        HashedSymList(size_t max) {
            max_entries = max;
            if (max > 0) {
                entries = new Symbol[max];
            } else {
                entries = nullptr;
            }
        }
        HashedSymList(HashedSymList *v) : HashedSymList(v->max_entries) {
            length = v->length;
            for (size_t i = 0; i < max_entries; i++) {
                entries[i] = v->entries[i];
            }
        }

        void Resize(size_t size) {
            if (size < max_entries) {
                length = -1;
            }
            Symbol *newmembers = new Symbol[size];
            size_t j = 0;
            for (size_t i = 0; i < max_entries; i++) {
                if (entries[i].key != nullptr) {
                    newmembers[j++] = entries[i];
                    if (j >= size) {
                        break;
                    }
                }
            }
            while (j < size) {
                entries[j++].key = nullptr;
            }
            max_entries = size;
            delete [] entries;
            entries = newmembers;
            Length();
        }

        V *Add(const char *key, V value) {
            for (size_t i = 0; i < max_entries; i++) {
                if (entries[i].key == nullptr) {
                    entries[i].hash = Hash(key);
                    entries[i].key = key;
                    entries[i].value = value;
                    length++;
                    return &entries[i].value;
                }
            }
            Resize(max_entries + MIN_NUM_ENTRIES);
            size_t j = FindFirstEmptyIndex();
            entries[j].hash = Hash(key);
            entries[j].key = key;
            entries[j].value = value;
            length++;
            return &entries[j].value;
        }

        void Remove(const char *key) {
            Symbol& sym = FindSym(key);
            sym.name = nullptr;
            length--;
        }

        long long FindSymIndex(const char *key) {
            uint32_t hash = Hash(key);
            for (size_t i = 0; i < max_entries; i++) {
                if (entries[i].key != nullptr) {
                    if (entries[i].hash == hash) {
                        if (!strcmp(entries[i].key, key)) {
                            return i;
                        }
                    }
                }
            }
            return -1;
        }

        bool Contains(const char *key) {
            return (FindSymIndex(key) != -1);
        }

        size_t Length() {
            size_t l = 0;
            for (size_t i = 0; i < max_entries; i++) {
                if (entries[i].key != nullptr) {
                    l++;
                }
            }
            length = l;
            return l;
        }

        size_t FindFirstEmptyIndex() {
            for (size_t i = 0; i < max_entries; i++) {
                if (entries[i].key == nullptr) {
                    return i;
                }
            }
            size_t n = max_entries;
            Resize(max_entries + MIN_NUM_ENTRIES);
            return n;
        }

        Symbol& Get(size_t i) {
            for (size_t j = 0; j < max_entries; j++) {
                if (entries[j].key != nullptr) {
                    if (i == 0) {
                        return entries[j];
                    }
                    i--;
                }
            }
            return entries[FindFirstEmptyIndex()];
        }

        Symbol& FindSym(const char *key) {
            long long i = FindSymIndex(key);
            if (i == -1) {
                return entries[FindFirstEmptyIndex()];
            }
            return entries[i];
        }

        const char *Keys(size_t i) {
            return Get(i).key;
        }

        V& Values(size_t i) {
            return Get(i).value;
        }
		
		V& Values(const char *key) {
			return FindSym(key).value;
		}

        Symbol& NextSym(const char *key) {
            long long i = FindSymIndex(key);
            if (i == -1) {
                return -1;
            }
            return NextSym(i);
        }

        Symbol& NextSym(size_t i) {
            if (i+1 >= max_entries) {
                return new Symbol;
            }
            return entries[i+1];
        }

        V& operator[](const char *key) {
            return Values(key);
        }

        V& operator[](size_t i) {
            return Values(i);
        }
    };
    enum Type {
        Empty = 0,
        Object,
        Array,
        Null,
        Boolean,
        Integer,
        Float,
        String,
    };
    class JSON {
        public:
        class JSONArray {
            static const size_t MIN_ALLOC = 10;
            public:
            size_t length;
            size_t allocated;
            JSON *members;
            JSONArray() : JSONArray(MIN_ALLOC) {}
            JSONArray(size_t size) {
                allocated = size;
                length = 0;
                members = new JSON[size]();
            }
            JSONArray(JSON *members, size_t size) {
                this->allocated = size;
                this->length = size;
                this->members = members;
            }
            JSONArray(JSONArray& a) : JSONArray(a.length) {
                for (size_t i=0; i<a.length; i++) {
                    members[i] = a.members[i];
                }
            }
            ~JSONArray() {
                if (members != NULL && allocated > 0) {
                    delete members;
                }
            }
            size_t trim() {
                resize(length);
                return length;
            }
            void resize(size_t size) {
                if (size < length) {
                    size = length;
                }
                JSON *newmembers = new JSON[size]();
                for (size_t i=0; i<length; i++) {
                    newmembers[i] = members[i];
                }
                delete [] members;
                members = newmembers;
                allocated = size;
            }
            JSON *append(JSON object) {
                if (length + 1 >= allocated) {
                    resize(allocated + MIN_ALLOC);
                }
                members[length] = object;
				return &members[length++];
            }
            void remove(size_t i) {
                if (i < length) {
                    if (i == length - 1) {
                        members[length] = JSON();
                    } else for (; i<length-1; i++)
                        members[i] = members[i+1];
                    length--;
                }
            }
            JSON *get(size_t i) {
                if (i < length) {
                    return &members[i];
                }
                return NULL;
            }
            JSON *operator[](size_t i) {
                return get(i);
            }
            JSON *operator+=(JSON object) {
                return append(object);
            }
        };
        private:
        Type type = Type::Empty;
        union {
            const char *s;
            double d;
            long long i;
            JSONArray *a;
            JSONMap *o;
        } value;
        public:
        JSON() {}
        JSON(JSONMap *o) {
            this->type = Type::Object;
            this->value.o = o;
        }
        JSON(JSONArray *a) {
            this->type = Type::Array;
            this->value.a = a;
        }
        JSON(long long i) {
            this->type = Type::Integer;
            this->value.i = i;
        }
        JSON(double d) {
            this->type = Type::Float;
            this->value.d = d;
        }
        JSON(char *s) {
            this->type = Type::String;
            this->value.s = s;
        }
        JSON(std::string s) {
            this->type = Type::String;
            this->value.s = (char*) s.c_str();
        }

        Type getType() {
            return type;
        }
        JSON *operator[](size_t i) {
            if (type == Type::Array) {
                return value.a->get(i);
            }
            value.a->append(JSON());
            return value.a->get(value.a->length);
        }
        JSON *operator[](std::string key) {
            const char *s = key.c_str();
            if (type == Type::Object) {
                if (value.o->Contains(s)) {
                    return &value.o->FindSym(s).value;
                }
                return value.o->Add(key.c_str(), JSON());
            }
            printf("Cannot index non-object with string\n");
            std::exception();
        }
        JSON *operator[](char *key) {
            if (type == Type::Object) {
                if (value.o->Contains(key)) {
                    return &value.o->FindSym(key).value;
                }
                return value.o->Add(key, JSON());
            }
            printf("Cannot index non-object with string\n");
            std::exception();
        }
        bool contains(std::string key) {
            if (type == Type::Object) {
                return value.o->Contains(key.c_str());
            }
            return false;
        }
        bool contains(const char *key) {
            if (type == Type::Object) {
                return value.o->Contains(key);
            }
            return false;
        }
        const char* getCString() {
            if (type != Type::String) {
                return NULL;
            }
            return value.s;
        }
        std::string getString() {
            if (type != Type::String) {
                return "";
            }
            return std::string(value.s);
        }
        double getFloat() {
            if (type != Type::Float) {
                return 0.0f;
            }
            return value.d;
        }
        long long getInteger() {
            if (type != Type::Integer) {
                return 0;
            }
            return value.i;
        }
        size_t getUnsigned() {
            if (type != Type::Integer) {
                return 0;
            }
            return value.i;
        }
        bool getBoolean() {
            if (type != Type::Boolean) {
                return false;
            }
            return value.i > 0;
        }
        bool isNull() {
            return type == Type::Null;
        }
        size_t getArrayLength() {
            if (type != Type::Array) {
                return 0;
            }
            return value.a->length;
        }
        size_t getObjectLength() {
            if (type != Type::Object) {
                return 0;
            }
            return value.o->Length();
        }
        JSONMap *getObject() {
            if (type != Type::Object) {
                return NULL;
            }
            return value.o;
        }
        JSONArray *getArray() {
            if (type != Type::Array) {
                return NULL;
            }
            return value.a;
        }


        JSONMap *setObject(JSONMap& o) {
            this->type = Type::Object;
            this->value.o = new JSONMap(o);
			return this->value.o;
        }
        JSONMap *setObject(JSONMap *o) {
            this->type = Type::Object;
            this->value.o = o;
			return this->value.o;
        }
        JSONArray *setArray(JSONArray& a) {
            this->type = Type::Array;
            this->value.a = new JSONArray(a);
			return this->value.a;
        }
        JSONArray *setArray(JSONArray *a) {
            this->type = Type::Array;
            this->value.a = a;
			return this->value.a;
        }
        double setFloat(double d) {
            this->type = Type::Float;
            this->value.d = d;
			return d;
        }
        long long setInteger(long long i) {
            this->type = Type::Integer;
            this->value.i = i;
			return i;
        }
        const char *setString(const char *s) {
            this->type = Type::String;
            this->value.s = s;
			return s;
        }
        std::string setString(std::string s) {
            this->type = Type::String;
            this->value.s = (char*)s.c_str();
			return s;
        }
        bool setBoolean(bool v) {
            this->type = Type::Boolean;
            this->value.i = v ? 1 : 0;
			return v;
        }
        void setNull() {
            this->type = Type::Null;
            this->value.i = 0;
        }

        const char *serialize() {
            std::string o = std::string();
            switch (type) {
                case Type::Empty:
                    break;
                case Type::Null:
                    o = "null";
                    break;
                case Type::Boolean:
                    o = value.i?"true":"false";
                    break;
                case Type::Integer:
                    o = std::to_string(value.i);
                    break;
                case Type::Float:
                    o = std::to_string(value.d);
                    break;
                case Type::String:
                    if (value.s != NULL) {
                        o.reserve(strlen(value.s));
                        o.append("\"");
                        o.append(value.s);
                        o.append("\"");
                    } else {
                        o.append("\"\"");
                    }
                    break;
                case Type::Array:
                    o.reserve(value.a->length*4);
                    o.append("[");
                    for (size_t i=0; i<value.a->length; i++) {
                        o.append(value.a->get(i)->serialize());
                        if (i+1<value.a->length) {
                            o.append(",\n");
                        }
                    }
                    o.append("]");
                    break;
                case Type::Object:
                    size_t len = value.o->Length();
                    o.reserve(len*6);
                    o.append("{");
                    for (size_t i=0; i<len; i++) {
                        o.append("\"");
                        o.append(value.o->Keys(i));
                        o.append("\": ");
                        o.append(value.o->Values(i).serialize());
                        if (i+1 < len) {
                            o.append(",\n");
                        }
                    }
                    o.append("}");
                    break;
            }
            o.shrink_to_fit();
            return dupcstr(o.c_str());
        }

        static JSON deserialize(const char *data) {
            size_t i = 0;
            return deserialize(data, i);
        }

        private:
        static char nibble(char c) {
            if (c >= '0' && c <= '9') {
                return c - '0';
            } else if (c >= 'A' && c <= 'F') {
                return c + 10 - 'A';
            } else if (c >= 'a' && c <= 'f') {
                return c + 10 - 'a';
            } else {
                return 0;
            }
        }
        static void skipspace(const char *data, size_t &i) {
            while (data[i] == ' ' || data[i] == '\t' || data[i] == '\n') {
                i++;
            }
        }
        static char nextchar(const char *data, size_t &i) {
            skipspace(data, i);
            char c = data[i++];
            skipspace(data, i);
            return c;
        }

        static char *dupcstr(const char *s) {
            if (s == NULL) {
                return NULL;
            }
            char *r = (char*) malloc(strlen(s)+1);
            if (r != NULL) {
                strcpy(r, s);
            }
            return r;
        }

        static JSON deserialize(const char *data, size_t& i) {
            JSON o;
            char c = data[i];
            if (c == ',') {
                c = data[++i];
            }
            skipspace(data, i);
            c = data[i];
            if (c >= 'a' && c <= 'z') {
                if (!strcmp(&data[i], "null")) {
                    o.setNull();
                } else if (!strcmp(&data[i], "false")) {
                    o.setBoolean(false);
                } else if (!strcmp(&data[i], "true")) {
                    o.setBoolean(false);
                } else {
                    o.setNull();
                }
            } else if (c >= '0' && c <= '9') {
                size_t j;
                bool flt = false;
                bool neg = false;
                bool hex = false;
                if (data[i] == '-') {
                    neg = true;
                    i++;
                    c = data[i];
                }
                if (c == '0' && data[i+1] == 'x') {
                    hex = true;
                    i+=2;
                }
                j = i;
                while (c != ',' && c != ']' && c != '}') {
                    c = data[j++];
                    if (c == '.') {
                        flt = true;
                    }
                }
                if (hex) {
                    long long num = 0;
                    for (; i<j; j++) {
                        num = (num<<4) + nibble(data[i]);
                    }
                    if (neg) {
                        num = -num;
                    }
                    o.setInteger(num);
                } else if (flt) {
                    double num = 0;
                    double f = 0.1;
                    flt = false;
                    for (; i<j; i++) {
                        if (data[i] >= '0' && data[i] <= '9') {
                            if (flt) {
                                num += f*(data[i] - '0');
                                f *= 0.1;
                            } else {
                                num = num * 10 + data[i] - '0';
                            }
                        } else if (data[i] == '.') {
                            flt = true;
                        }
                    }
                    if (neg) {
                        num = -num;
                    }
                    o.setFloat(num);
                } else {
                    long long num = 0;
                    for (; i<j; i++) {
                        if (data[i] >= '0' && data[i] <= '9') {
                            num = num * 10 + data[i] - '0';
                        }
                    }
                    if (neg) {
                        num = -num;
                    }
                    o.setInteger(num);
                }
                i--;
            } else if (c == '"') {
                std::string s;
                i++;
                do {
                    c = data[i++];
                    if (c == '\\') {
                        if (data[i] == 'n') {
                            s += '\n';
                        } else if (data[i] == 't') {
                            s += '\t';
                        } else if (data[i] == 'r') {
                            s += '\r';
                        } else if (data[i] == '0') {
                            s += '\0';
                        } else if (data[i] == 'x') {
                            i++;
                            s += (nibble(data[i]) << 4) | nibble(data[i+1]);
                            i += 2;
                        } else {
                            s += c;
                            s += data[i++];
                        }
                    } else if (c != '"') {
                        s += c;
                    }
                } while (c != '"');
                o.setString(dupcstr(s.c_str()));
            } else if (c == '[') {
                JSONArray a;
                c = data[++i];
                skipspace(data, i);
                while (c != ']') {
                    JSON j = deserialize(data, i);
                    a.append(j);
                    c = nextchar(data, i);
                }
                o.setArray(a);
            } else if (c == '{') {
                JSONMap a;
                c = data[++i];
                skipspace(data, i);
                while (c != '}') {
                    const char *key = deserialize(data, i).getCString();
                    if (data[i] == ':') {
                        i++;
                    }
                    skipspace(data, i);
                    if (data[i] == ':') {
                        i++;
                    }
                    a.Add(dupcstr(key), deserialize(data, i));
                    c = nextchar(data, i);
                }
                o.setObject(a);
            }
            return o;
        }

    };
	typedef JSON::JSON::JSONArray JSONArray;
	typedef HashedSymList<JSON> JSONObject;

    static JSON deserialize(const char *s) {
        return JSON::deserialize(s);
    }

    static JSON deserialize(std::string s) {
        return JSON::deserialize(s.c_str());
    }

}


#endif