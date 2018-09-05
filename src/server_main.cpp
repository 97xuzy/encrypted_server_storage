#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "AES_CCM_encryption.h"
using CryptoPP::byte;
#include "password_hash.h"
#include "http_server.h"

#include "assert.h"

int main(int argc, char* argv[])
{
    cout << password_hash_hex("Hfllo World") << '\n';
    try
    {
        HTTPServer server(8182);
        server.run();
    }
    catch (const std::runtime_error &e)
    {
        cout << e.what() << '\n';
    }

    return 0;
}

