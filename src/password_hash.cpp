#include "password_hash.h"

#include <string>
using std::string;
#include "../cryptopp-CRYPTOPP_7_0_0/sha.h"
using CryptoPP::SHA256;
#include "../cryptopp-CRYPTOPP_7_0_0/filters.h"
using CryptoPP::StringSource;
using CryptoPP::StringSink;
#include "../cryptopp-CRYPTOPP_7_0_0/hex.h"
using CryptoPP::HexEncoder;


string password_hash(const string &password)
{
    std::string digest;
    SHA256 hash;

    StringSource foo(password, true,
        new CryptoPP::HashFilter(hash, new StringSink(digest)) );
    return digest;
}

string password_hash_hex(const string &password)
{
    std::string digest;
    SHA256 hash;

    StringSource foo(password, true,
        new CryptoPP::HashFilter(hash, new HexEncoder( new StringSink(digest), true, 16, " " )) );
    return digest;
}

