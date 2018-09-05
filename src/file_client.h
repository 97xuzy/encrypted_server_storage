#pragma once

#include "http_client.h"
#include <string>
#include <map>
#include <vector>

/**
 * FileClient
 * Another layer of abstration over HTTPClient.
 * Manage the local copy of encrypted file.
 * File are referenced by filename.
 */
class FileClient
{
private:
    HTTPClient client;
    std::vector<std::string> on_disk_file;
    std::map<std::string, std::string> in_memory_file;
public:
    /**
     * option pass into request_file(), file requested are stored as physical file on disk
     */
    constexpr static int IN_FILE = 0b0001;

    /**
     * option pass into request_file(), file requested are stored in memory
     */
    constexpr static int IN_MEMORY = 0b0000;

    /**
     * Constructor
     */
    FileClient(const std::string &ip_addr);

    /**
     * Constructor
     */
    FileClient(const HTTPClient &client);

    /**
     * request_file()
     * Request a encrypted file from server
     * @param option IN_FILE or IN_MEMORY
     */
    void request_file(const std::string &filename, int option);

    /**
     * view_file()
     * Decrypt a local encrypted file.
     * Decrypted content are presented in a copy-on-write manner, changes made to
     * decrypted content will affect the encrypted file.
     * @return decrypted file content
     */
    std::string view_file(const std::string &filename, const std::string &key);

    /**
     * update_local_file()
     * Update the local copy of the encrypted file with given plain_content.
     * plain_content will be encrypted and replace the old content of the local copy.
     */
    void update_local_file(const std::string &filename, const std::string &key, const std::string &plain_content);

    /**
     * upload_file()
     * upload the local copy of the encrypted file to the server.
     */
    void upload_file(const std::string &filename);
};



