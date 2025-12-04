#ifndef PICOSHA2_HPP
#define PICOSHA2_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdint.h>

//importacao de biblioteca terceira
// essa e' um biblioteca de SHA-256.
// ela vai gerar um hash em hexadecimal a partir de um texto para senha e e salvar/validar

//usamos tipo assim: std::string hash = picosha2::hash256_hex_string("senha");
//depois ele so' compara o hash calculado com o hash salvo no arquivo

namespace picosha2 {

typedef uint32_t uint32;
typedef uint8_t uint8;


//fica no namespace detail pra separar o que é motor do algoritmo
//do que e' API que você chama no sistema
namespace detail {

//funcoes auxiliares do algoritmo SHA-256
inline uint32 ch(uint32 x, uint32 y, uint32 z) { return (x & y) ^ ((~x) & z); }
inline uint32 maj(uint32 x, uint32 y, uint32 z) { return (x & y) ^ (x & z) ^ (y & z); }
inline uint32 rotr(uint32 x, uint32 n) { return (x >> n) | (x << (32 - n)); }
inline uint32 bsig0(uint32 x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
inline uint32 bsig1(uint32 x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
inline uint32 ssig0(uint32 x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
inline uint32 ssig1(uint32 x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }
}

// Constantes fixas do SHA-256
static const uint32 k[64] = {
    0x428a2f98ul,0x71374491ul,0xb5c0fbcful,0xe9b5dba5ul,0x3956c25bul,0x59f111f1ul,0x923f82a4ul,0xab1c5ed5ul,
    0xd807aa98ul,0x12835b01ul,0x243185beul,0x550c7dc3ul,0x72be5d74ul,0x80deb1feul,0x9bdc06a7ul,0xc19bf174ul,
    0xe49b69c1ul,0xefbe4786ul,0x0fc19dc6ul,0x240ca1ccul,0x2de92c6ful,0x4a7484aaul,0x5cb0a9dcul,0x76f988daul,
    0x983e5152ul,0xa831c66dul,0xb00327c8ul,0xbf597fc7ul,0xc6e00bf3ul,0xd5a79147ul,0x06ca6351ul,0x14292967ul,
    0x27b70a85ul,0x2e1b2138ul,0x4d2c6dfcul,0x53380d13ul,0x650a7354ul,0x766a0abbul,0x81c2c92eul,0x92722c85ul,
    0xa2bfe8a1ul,0xa81a664bul,0xc24b8b70ul,0xc76c51a3ul,0xd192e819ul,0xd6990624ul,0xf40e3585ul,0x106aa070ul,
    0x19a4c116ul,0x1e376c08ul,0x2748774cul,0x34b0bcb5ul,0x391c0cb3ul,0x4ed8aa4aul,0x5b9cca4ful,0x682e6ff3ul,
    0x748f82eeul,0x78a5636ful,0x84c87814ul,0x8cc70208ul,0x90befffaul,0xa4506cebul,0xbef9a3f7ul,0xc67178f2ul
};

// Gera o SHA-256 em bytes (32 bytes) a partir de uma string
inline void hash256(const std::string& src, std::vector<uint8>& dst) {
    static const uint32 initial_hash[8] = {
        0x6a09e667ul,0xbb67ae85ul,0x3c6ef372ul,0xa54ff53aul,
        0x510e527ful,0x9b05688cul,0x1f83d9abul,0x5be0cd19ul};

    dst.resize(32);
    std::vector<uint8> msg(src.begin(), src.end());
    uint64_t bit_len = msg.size() * 8;
    msg.push_back(0x80);

    while ((msg.size() % 64) != 56) msg.push_back(0);

    for (int i = 7; i >= 0; --i)
        msg.push_back((bit_len >> (i * 8)) & 0xFF);

    uint32 h[8];
    for (int i = 0; i < 8; ++i) h[i] = initial_hash[i];

    for (size_t chunk = 0; chunk < msg.size(); chunk += 64) {
        uint32 w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (msg[chunk + 4*i] << 24) |
                   (msg[chunk + 4*i + 1] << 16) |
                   (msg[chunk + 4*i + 2] << 8) |
                    msg[chunk + 4*i + 3];
        }
        for (int i = 16; i < 64; ++i)
            w[i] = detail::ssig1(w[i - 2]) + w[i - 7] +
                   detail::ssig0(w[i - 15]) + w[i - 16];

        uint32 a=h[0],b=h[1],c=h[2],d=h[3],
               e=h[4],f=h[5],g=h[6],h0=h[7];

        for (int i = 0; i < 64; ++i) {
            uint32 temp1 = h0 + detail::bsig1(e) + detail::ch(e,f,g)
                          + k[i] + w[i];
            uint32 temp2 = detail::bsig0(a) + detail::maj(a,b,c);
            h0 = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0]+=a; h[1]+=b; h[2]+=c; h[3]+=d;
        h[4]+=e; h[5]+=f; h[6]+=g; h[7]+=h0;
    }

    for (int i = 0; i < 8; ++i) {
        dst[4*i]   = (h[i] >> 24) & 0xFF;
        dst[4*i+1] = (h[i] >> 16) & 0xFF;
        dst[4*i+2] = (h[i] >> 8) & 0xFF;
        dst[4*i+3] = h[i] & 0xFF;
    }
}

inline std::string bytes_to_hex_string(const std::vector<uint8>& bytes) {
    std::ostringstream oss;
    for (uint8 b : bytes)
        oss << std::hex << std::setw(2)
            << std::setfill('0') << (int)b;
    return oss.str();
}

inline std::string hash256_hex_string(const std::string& src) {
    std::vector<uint8> hash;
    hash256(src, hash);
    return bytes_to_hex_string(hash);
}

}

#endif
