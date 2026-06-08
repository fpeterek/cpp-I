#include <print>

#include <SFML/Network.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>


int main() {
    sf::TcpSocket      socket;
    sf::Socket::Status status = socket.connect({127, 0, 0, 1}, 8080);

    if (status != sf::Socket::Status::Done) {
        std::println(stderr, "Failed to connect");
    }

    std::string_view msg = "Message to server";

    if (socket.send(msg.data(), msg.size()) != sf::Socket::Status::Done) {
        std::println(stderr, "Failed to send");
    }

    char resp[1024] = "";
    size_t rec = 0;

    if (socket.receive(resp, 1024, rec) != sf::Socket::Status::Done) {
        std::println(stderr, "Failed to receive");
    }

    std::println("Response: {}", resp);
}
