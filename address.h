#ifndef GUARD_ADDRESS_H
#define GUARD_ADDRESS_H
#include <string>
#include <vector>
#include "base.h"
class Address {
public:
  Address();                    // Constructs the ZERO address
  Address(std::string b32form);
  Address(bytes public_key);
  std::string as_string;
  bytes public_key;
  bool is_zero() const;
private:
  Address(std::string s, bytes with_csum);
  Address(bytes public_key, bytes with_csum);
};
std::ostream& operator<<(std::ostream& os, const Address& addr);

#endif
