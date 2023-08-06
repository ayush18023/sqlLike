namespace veno{

    inline uint32_t hash_str_uint32(const std::string& str) {

        uint32_t hash = 0x811c9dc5;
        uint32_t prime = 0x1000193;

        for(int i = 0; i < str.size(); ++i) {
            uint8_t value = str[i];
            hash = hash ^ value;
            hash *= prime;
        }

        return hash;

    }

    unsigned int jenkinsOneAtATimeHash(const char* str) {
        unsigned int hash = 0;
        while (*str) {
            hash += *str++;
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return hash;
    }
}
