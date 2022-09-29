#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "mnemonic.h"
#include "address.h"

inline bool operator==(const Address& lhs, const Address& rhs) {
  return lhs.as_string == rhs.as_string && lhs.public_key == rhs.public_key;
}
inline bool operator!=(const Address& lhs, const Address& rhs) {
  return !(lhs == rhs);
}
inline bool operator <(const Address& lhs, const Address& rhs ) {
  return lhs.as_string < rhs.as_string;
}

Address::Address() :
  Address("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY5HFKQ") {
}

bool
Address::is_zero() const {
  return public_key == bytes{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}

Address::Address(std::string address) :
  Address(address, b32_decode(address)) {
}

Address::Address(std::string address, bytes with_checksum) :
  as_string(address),
  public_key(bytes{with_checksum.begin(), with_checksum.begin()+32}) {
  assert(as_string.size() == 58);
  assert(public_key.size() == 32);
}

std::string to_hex(const std::string& in) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; in.size() > i; i++) {
    ss << std::setw(2) << (int)(unsigned char)in[i] << ':';
  }
  return ss.str();
}
std::string to_hex(const bytes& in) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; in.size() > i; i++) {
    ss << std::setw(2) << (int)(unsigned char)in[i] << ':';
  }
  return ss.str();
}

static bytes
checksummed(bytes public_key) {
  bytes copy(public_key);
  auto hash = sha512_256(public_key);
  copy.insert(copy.end(), hash.end()-4, hash.end());
  return copy;
}

Address::Address(bytes public_key) : Address(public_key,  checksummed(public_key)) { }

Address::Address(bytes public_key, bytes checksummed) :
  as_string(b32_encode(checksummed)),
  public_key(public_key) {
  assert(as_string.size() == 58);
  assert(public_key.size() == 32);
}

std::ostream&
operator<<(std::ostream& os, const Address& addr) {
  os << addr.as_string;
  return os;
}	

