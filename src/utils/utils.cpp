#include "utils.h"

uint32_t calculate_hash(const std::string &identifier) {
    const uint32_t hash_module = 511; // or 211: the prime number
    uint32_t g; // for calculating hash
    const uint32_t hash_mask = 0xF0000000;
    uint32_t hash_value = 0;

    for (size_t i = 0; i < identifier.length(); i++) {
        // Calculating hash: see Dragon Book, Fig. 7.35
        hash_value = (hash_value << 4) + static_cast<uint8_t>(identifier[i]);
        if ((g = hash_value & hash_mask) != 0) {
            hash_value = hash_value ^ (hash_value >> 24);
            hash_value ^= g;
        }
    }
    // final hash value for identifier
    return hash_value % hash_module;
}