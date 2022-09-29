#ifndef GUARD_BASE_H
#define GUARD_BASE_H

#include <string>
#include <vector>
#include <iostream>
typedef std::vector<unsigned char> bytes;

template <typename T, unsigned char *CHARMAP, int BITWIDTH>
void _bit_mapped_encode(const bytes& in, T& out) {
  static const int mask = (1 << BITWIDTH) - 1;
  int val = 0;
  int bits = 0;
  for (auto c : in) {
    val = (val << 8) + c;
    bits += 8;
    while (bits >= BITWIDTH) {
      out.push_back(CHARMAP[(val>>(bits-5))&mask]);
      bits -= BITWIDTH;
    }
  }
  if (bits > 0)
    out.push_back(CHARMAP[(val<<(5-bits))&mask]);
}

template <typename T, int BITWIDTH>
void _bit_raw_encode (const bytes& in, T& out) {
  static const int mask = (1 << BITWIDTH) - 1;
  unsigned val = 0;
  int bits = 0;
  for (auto b : in) {
    val |= (b << bits);
    bits += 8;
    if (bits >= BITWIDTH) {
      out.push_back(val & mask);
      val >>= BITWIDTH;
      bits -= BITWIDTH;
    }
  }
  if (bits > 0)
    out.push_back(val & mask);
}

template <unsigned char * CHARMAP, int BITWIDTH>
std::vector<int> _bit_decode_digits() {
  std::vector<int> table(256, -1);
  for (int i=0; i<BITWIDTH; i++)
    table[CHARMAP[i]] = i;
  return table;
}

template <typename T, unsigned char * CHARMAP, int BITWIDTH>
void _bit_mapped_decode (const T& in, bytes& out, int& bits)
{
  static std::vector<int> dd = _bit_decode_digits<CHARMAP, 1 << BITWIDTH>();

  bits = 0;
  int val = 0;
  for (auto c : in) {
    if (dd[c] == -1)
      break;
    val = (val << BITWIDTH) + dd[c];
    bits += BITWIDTH;
    if (bits >= 8) {
      out.push_back( (unsigned char) ((val>>(bits-8))&0xFF));
      bits -= 8;
    }
  }
  if (bits > 0)
    out.push_back( (unsigned char) ((val<<(8-bits))&0xFF));
}

template <typename T, int BITWIDTH>
void _bit_raw_decode (const T& in, bytes& out, int& bits) {
  bits = 0;
  int val = 0;
  for (auto i : in) {
    val |= (i << bits);
    bits += BITWIDTH;
    while (bits >= 8) {
      out.push_back(val & 0xFF);
      val >>= 8;
      bits -= 8;
    }
  }
}

static unsigned char B64DIGITS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
static unsigned char B32DIGITS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

std::string b64_encode(const bytes& in, bool padded = false);
bytes b64_decode(const std::string& in);
std::string b32_encode(const bytes& in);
bytes b32_decode(const std::string& in);
std::pair<bytes,std::pair<unsigned char,int>> b32_decode_partial(const std::string& in);
std::vector<uint16_t> b2048_encode(const bytes& in);
bytes b2048_decode(const std::vector<uint16_t> &in);

#endif
