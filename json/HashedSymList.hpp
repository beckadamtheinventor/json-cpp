#ifndef __HASHED_SYM_LIST_HPP__
#define __HASHED_SYM_LIST_HPP__

#include <stdint.h>
#include <string.h>

namespace HashedSymList {
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
}

#endif