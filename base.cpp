#include "base.h"

// Surely I should write an implementation that avoids the almost
// cutnpaste job here.

std::string b32_encode(const bytes& in) {
  std::string out;
  out.reserve(8 + (8*in.size()/5));
  _bit_mapped_encode<std::string, B32DIGITS, 5>(in, out);
  return out;
}
std::string b64_encode(const bytes& in, bool padded) {
  std::string out;
  out.reserve(4 + (4*in.size()/3));
  _bit_mapped_encode<std::string, B64DIGITS, 6>(in, out);
  while (padded && out.size() % 4)
    out.push_back('=');
  return out;
}

bytes b32_decode(const std::string &in) {
  int bits = 0;
  bytes out;
  out.reserve(5*in.size()/8);
  _bit_mapped_decode<std::string, B32DIGITS, 5>(in, out, bits);
  return out;
}

// return pair of final byte and relevant bitmask with index.
std::pair<bytes,std::pair<unsigned char,int>> b32_decode_partial(const std::string &in) {
  int bits = 0;
  bytes out;
  out.reserve(5*in.size()/8);
  _bit_mapped_decode<std::string, B32DIGITS, 5>(in, out, bits);

  // use the leftover bit count to make a mask.
  unsigned char mask = (0xff << (8-bits)) & 0xFF;
  return {out, {mask, out.size() - 1}};
}

bytes b64_decode(const std::string &in) {
  int bits = 0;
  bytes out;
  out.reserve(3*in.size()/4);
  _bit_mapped_decode<std::string, B64DIGITS, 6>(in, out, bits);
  return out;
}

std::vector<uint16_t> b2048_encode(const bytes& in) {
  std::vector<uint16_t> out;
  out.reserve(1+8*in.size()/11);
  _bit_raw_encode<std::vector<uint16_t>, 11>(in, out);
  return out;
}

bytes b2048_decode(const std::vector<uint16_t> &in) {
  int bits = 0;
  bytes out;
  out.reserve(1+11*in.size()/8);
  _bit_raw_decode<std::vector<uint16_t>, 11> (in, out, bits);
  return out;
}
