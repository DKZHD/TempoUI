#include "NetworkManager.h"
#include <string>

void NetworkManager::start_server(const std::string& port)
{
    if (is_active)
    {
        return;
    }
    is_active = true;
    network_thread = std::thread(&NetworkManager::server_loop, this, port);
}

void NetworkManager::connect_to_server(const std::string& ip, const std::string& port)
{
    if (is_active)
    {
        return;
    }
    is_active = true;
    network_thread = std::thread(&NetworkManager::client_loop, this, ip, port);
}

void NetworkManager::stop()
{
    is_active = false;
    if (network_thread.joinable())
    {
        network_thread.join();
    }
}

void NetworkManager::process_messages(const std::function<void(const std::string&)>& message_recieved_func)
{
    std::lock_guard<std::mutex> lock(thread_lock);
    while (!unprocessed_messages.empty())
    {
        message_recieved_func(unprocessed_messages.front());
        unprocessed_messages.pop();
    }
}

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#define FD_SETSIZE 1024 // NOLINT

#include <winsock2.h>
#include <ws2tcpip.h>

#endif

void NetworkManager::send_message(const std::string& message)
{
    if (server_connection != 0 && server_connection != ~0ULL)
    {
        send(server_connection, message.c_str(), (int)message.size(), 0);
    }
    else
    {
        for (Socket client : active_clients)
        {
            send(client, message.c_str(), (int)message.size(), 0);
        }
    }
}

void NetworkManager::disconnect_from_server()
{
    is_active.store(false);
    closesocket(server_connection);
    server_connection = 0;
}

void NetworkManager::client_loop(const std::string& ip, const std::string& port)
{
    WSADATA wsa_data;
    int i_result = 0;

    i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0)
    {
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: WSAStartup Failed: " + std::to_string(i_result));
        return;
    }

    addrinfo* result = NULL;
    addrinfo hints{ 0, AF_INET, SOCK_STREAM, IPPROTO_TCP };

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &result) != 0)
    {
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: IP not found");
        return;
    }

    server_connection = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_connection == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    if (connect(server_connection, result->ai_addr, (int)result->ai_addrlen))
    {
        closesocket(server_connection);
        server_connection = INVALID_SOCKET;
        freeaddrinfo(result);
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.emplace("ERROR: Unable to connect to server");
        return;
    }
    freeaddrinfo(result);

    while (is_active)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_connection, &readfds);

        timeval timeout{ 0, 100000 };

        int i_result = select(0, &readfds, nullptr, nullptr, &timeout);
        if (i_result > 0 && FD_ISSET(server_connection, &readfds))
        {
            char recvbuf[1024]; // NOLINT
            int recieved_bytes = recv(server_connection, recvbuf, 1023, 0);
            if (recieved_bytes > 0)
            {
                recvbuf[i_result] = '\0';
                std::lock_guard<std::mutex> lock(thread_lock);
                unprocessed_messages.push(std::string(recvbuf));
            }
            else
            {
                recvbuf[i_result] = '\0';
                std::lock_guard<std::mutex> lock(thread_lock);
                unprocessed_messages.push("Connection Lost!");
                break;
            }
        }
    }
    closesocket(server_connection);
    WSACleanup();
}

void NetworkManager::server_loop(const std::string& port)
{
    WSADATA wsa_data;
    int i_result = 0;

    i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0)
    {
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: WSAStartup Failed: " + std::to_string(i_result));
        return;
    }

    addrinfo* result = NULL;
    addrinfo hints{ AI_PASSIVE, AF_INET, SOCK_STREAM, IPPROTO_TCP };

    if (i_result = getaddrinfo(NULL, port.c_str(), &hints, &result); i_result != 0)
    {
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: getaddrinfo failed: " + std::to_string(i_result));
        return;
    }

    Socket listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_sock == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: listen socket creation failed!");
        return;
    }

    i_result = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
    if (i_result == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        closesocket(listen_sock);
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: Couldn't bind to port " + port);
        return;
    }

    freeaddrinfo(result);

    i_result = listen(listen_sock, SOMAXCONN);
    if (i_result == SOCKET_ERROR)
    {
        closesocket(listen_sock);
        WSACleanup();
        std::lock_guard<std::mutex> lock(thread_lock);
        unprocessed_messages.push("ERROR: Couldn't listen on port " + port);
        return;
    }

    while (is_active)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(listen_sock, &readfds);

        for (Socket client_sock : active_clients)
        {
            FD_SET(client_sock, &readfds);
        }

        timeval timeout{ 0, 100000 };

        int activity = select(0, &readfds, nullptr, nullptr, &timeout);
        if (activity == SOCKET_ERROR)
        {
            std::lock_guard<std::mutex> lock(thread_lock);
            unprocessed_messages.push("ERROR: select() failed!");
            break;
        }
        if (activity == 0)
            continue;

        if (FD_ISSET(listen_sock, &readfds))
        {
            SOCKET client = accept(listen_sock, nullptr, nullptr);
            if (client != INVALID_SOCKET)
            {
                active_clients.push_back(client);

                std::lock_guard<std::mutex> lock(thread_lock);
                unprocessed_messages.push("SYSTEM: A client connected.");
            }
        }

        for (auto it = active_clients.begin(); it != active_clients.end();)
        {
            Socket client = *it;

            if (FD_ISSET(client, &readfds))
            {
                char recvbuf[1024];
                int recvbuflen = 1024;

                i_result = recv(client, recvbuf, recvbuflen - 1, 0);

                if (i_result > 0)
                {
                    recvbuf[i_result] = '\0';
                    std::lock_guard<std::mutex> lock(thread_lock);
                    unprocessed_messages.push(std::string(recvbuf));
                    ++it;
                }
                else
                {
                    std::lock_guard<std::mutex> lock(thread_lock);
                    unprocessed_messages.push("SYSTEM: A client disconnected.\n");

                    closesocket(client);
                    it = active_clients.erase(it);
                }
            }
            else
            {
                ++it;
            }
        }
    }
    closesocket(listen_sock);
    WSACleanup();
}
#else
void NetworkManager::send_message(const std::string& message) {}
void NetworkManager::disconnect_from_server() {}
void NetworkManager::client_loop(const std::string& ip, const std::string& port) {}
void NetworkManager::server_loop(const std::string& port) {}
#endif