#ifndef GUARD_MNEMONIC_H
#define GUARD_MNEMONIC_H

#include <string>
#include <vector>

typedef std::vector<unsigned char> bytes;

std::string mnemonic_from_seed(bytes seed);
bytes seed_from_mnemonic(std::string);

bytes sha512_256(bytes input);
#endif
