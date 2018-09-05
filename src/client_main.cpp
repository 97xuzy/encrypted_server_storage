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
#include "http_client.h"
#include "file_client.h"

#include "assert.h"

int main(int argc, char* argv[])
{
    cout << password_hash_hex("Hfllo World") << '\n';

    HTTPClient http_client(string("127.0.0.1"));
    FileClient file_client(http_client);
    file_client.request_file("file1.bin", FileClient::IN_MEMORY);
    //file_client.view_file();

    return 0;
}

