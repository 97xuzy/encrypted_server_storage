#include "file_client.h"
#include "AES_CCM_encryption.h"

#include <string>
using std::string;

FileClient::FileClient(const string &ip_addr) : client(ip_addr)
{
}

FileClient::FileClient(const HTTPClient &http_client) : client(http_client)
{
}

void FileClient::request_file(const string &filename, int option)
{
    this->client.send_request(filename, "encrypted_server_storage");
    auto result = this->client.receive_response_body();

    if(std::get<0>(result) != 200) return;

    string body = std::get<1>(result);

    if((option & IN_MEMORY) == IN_MEMORY)
    {
        this->in_memory_file.emplace(std::make_pair(filename, body));
    }
    else
    {
        ;
    }
}

string FileClient::view_file(const string &filename, const string &key)
{
    string cipher;
    string plain_data;
    string adata("encrypted_server_storage");
    string iv((const char *)AES_CCM_Encryption::DEFAULT_IV, AES_CCM_Encryption::DEFAULT_IV_LEN);

    AES_CCM_Encryption::decrypt(key, iv, cipher, adata, plain_data.length(), plain_data);

    return plain_data;
}

void FileClient::update_local_file(const string &filename, const string &key, const string &plain_content)
{
    string adata("encrypted_server_storage");
    string iv((const char *)AES_CCM_Encryption::DEFAULT_IV, AES_CCM_Encryption::DEFAULT_IV_LEN);

    string cipher = AES_CCM_Encryption::encrypt(key, iv, adata, plain_content);
}

void FileClient::upload_file(const std::string &filename)
{
    ;
}
