#include <string>

/**
 * password_hash()
 * @return Hash of password in binary
 * @param password Input password in text form
 */
std::string password_hash(const std::string &password);

/**
 * password_hash_hex()
 * @return Hash of password in hex (text)
 * @param password Input password in text form
 */
std::string password_hash_hex(const std::string &password);
