#include <string_view>
#include <print>

#include <SFML/Network.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpListener.hpp>


int main() {
    sf::TcpListener listener;
    listener.setBlocking(true);
    if (listener.listen(8080) != sf::Socket::Status::Done) {
        std::println(stderr, "Failed to listen");
    }

    size_t connections = 0;

    while (true) {
        sf::TcpSocket socket;
        if (listener.accept(socket) != sf::Socket::Status::Done) {
            std::println(stderr, "Failed to accept");
            continue;
        }

        connections += 1;

        char in[1024] = "";
        size_t rec = 0;

        if (socket.receive(in, 1024, rec) != sf::Socket::Status::Done) {
            std::println(stderr, "Failed to receive");
            continue;
        }

        std::println("[{}]: {}", connections, in);

        std::string_view server_resp = "Response from server";

        if (socket.send(server_resp.data(), server_resp.size()) != sf::Socket::Status::Done) {
            std::println(stderr, "Failed to send");
        }
    }
}
