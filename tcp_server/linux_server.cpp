#include <cstdint>
#include <cstdio>
#include <cstring>
#include <print>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



int main() {
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd == -1) {
        std::println("Failed to create socket");
        return 1;
    }


    sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port =ntohs(8080),
        .sin_addr = {
            .s_addr = INADDR_ANY
        },
        .sin_zero = { 0  }
    };

    int res = bind(sd, reinterpret_cast<sockaddr*>(&address), sizeof(address));

    if (res != 0) {
        std::println(stderr, "Failed to bind");
        return 2;
    }

    res = listen(sd, 10);

    if (res != 0) {
        std::println(stderr, "Failed to listen");
    }


    sockaddr_in caller = {
        .sin_family = AF_INET,
        .sin_port = 0,
        .sin_addr = {
            .s_addr = 0x0
        },
        .sin_zero = { 0  }
    };

    socklen_t caller_len = sizeof(caller);

    size_t conn_count = 0;


    std::println("Listening...");
    

    while (true) {
        std::println("Accepting...");
        caller_len = sizeof(caller);
        int desc = accept(sd, reinterpret_cast<sockaddr*>(&caller), &caller_len);

        if (desc == -1) {
            std::println(stderr, "Failed to accept");
            continue;
        }
        std::println("Accepted...");

        conn_count += 1;


        FILE* file = fdopen(desc, "r+");

        uint8_t size_buff[sizeof(uint32_t)] = { 0  };

        fread(size_buff, sizeof(uint32_t), 1, file);
        // read(sd, size_buff, 4);

        uint32_t size = 0;
        std::memcpy(&size, size_buff, 4);
        size = ntohl(size);


        char in[1024] = "";
        std::println("Expecting {} bytes", size);

        fread(in, 1, size, file);
        // read(sd, in, 1024);

        *strchrnul(in, '\n') = 0;

        std::println("[{}]: {}(size={})", conn_count, in, strlen(in));

        std::string resp = std::format("Response #{}", conn_count);
        uint32_t resp_size = static_cast<uint32_t>(resp.size());
        resp_size = htonl(resp_size);

        uint8_t resp_buff[sizeof(resp_size) + 1] = { 0, 0, 0, 0, '\n' };
        memcpy(resp_buff, &resp_size, sizeof(resp_size));

        fwrite(resp_buff, sizeof(resp_buff), 1, file);
        std::println(file, "{}", resp);

        fclose(file);
    }
}

