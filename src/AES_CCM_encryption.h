
#include <iostream>
#include <string>
#include "../cryptopp-CRYPTOPP_7_0_0/hex.h"
#include "../cryptopp-CRYPTOPP_7_0_0/cryptlib.h"
#include "../cryptopp-CRYPTOPP_7_0_0/filters.h"
#include "../cryptopp-CRYPTOPP_7_0_0/aes.h"
#include "../cryptopp-CRYPTOPP_7_0_0/ccm.h"

namespace AES_CCM_Encryption
{
using CryptoPP::byte;

constexpr byte DEFAULT_IV[] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16};
constexpr size_t DEFAULT_IV_LEN = sizeof(DEFAULT_IV);

/**
 * encrypt()
 * @return cipher string
 * @param iv Initialization Vector
 * @param adata authentication data
 * @param pdata plain data
 */
std::string encrypt(const std::string &key, const std::string &iv, const std::string &adata, const std::string &pdata);

/**
 * decypt()
 * @param iv Initialization Vector
 * @param cipher cipher string
 * @param adata authentiocation data
 * @param pdata_size
 * @param rpdata retrived plain data
 */
void decrypt(const std::string &key, const std::string &iv, const std::string &cipher, const std::string &adata, size_t pdata_size, std::string &rpdata);

}