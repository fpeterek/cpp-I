#include <cstdint>
#include <cstdio>
#include <cstring>
#include <print>
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main() {

    int sd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr = {
            .s_addr = htonl(INADDR_LOOPBACK)
        },
        .sin_zero = { 0  }
    };

    std::println("Connecting...");
    int res = connect(sd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    std::println("Connected");

    if (res != 0) {
        std::println(stderr, "Failed to connect");
    }

    FILE* file = fdopen(sd, "r+");

    std::print("Message to server: ");
    std::fflush(stdout);
    std::string msg;
    std::getline(std::cin, msg);

    uint32_t msg_size = static_cast<uint32_t>(msg.size() + 1);
    uint32_t nsize = htonl(msg_size);

    fwrite(static_cast<void*>(&nsize), sizeof(nsize), 1, file);
    std::println(file, "{}", msg);

    uint8_t resp_size_buff[sizeof(uint32_t) + 1] = { 0 };
    fread(resp_size_buff, 1, sizeof(uint32_t) + 1, file);

    uint32_t resp_size = 0;
    memcpy(&resp_size, resp_size_buff, sizeof(uint32_t));
    resp_size = ntohl(resp_size);

    char resp[1024] = "";
    size_t bytes_read = fread(resp, 1, resp_size, file);
    fclose(file);

    std::println("Server responded: {} ({} bytes, read {})", resp, strlen(resp), bytes_read);

}
