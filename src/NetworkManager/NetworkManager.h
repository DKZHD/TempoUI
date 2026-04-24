#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <queue>
#include <vector>

#ifdef _WIN32
using Socket = unsigned long long;
#elif defined(__unix__) || defined(__APPLE__)
using Socket = int;
#else
#error "Unsupported platform!"
#endif

class NetworkManager
{
public:
    NetworkManager() = default;
    ~NetworkManager() { stop(); }

    // Server-Side
    void start_server(const std::string& port);
    void stop();

    // Client-Side
    void connect_to_server(const std::string& ip, const std::string& port);
    void send_message(const std::string& message);
    void disconnect_from_server();

    void process_messages(const std::function<void(const std::string&)>& message_recieved_func);

private:
    std::atomic<bool> is_active = false;
    std::mutex thread_lock;
    std::thread network_thread;
    std::queue<std::string> unprocessed_messages;

    Socket server_connection = 0;
    void client_loop(const std::string& ip, const std::string& port);

    void server_loop(const std::string& port);
    std::vector<Socket> active_clients;
};
