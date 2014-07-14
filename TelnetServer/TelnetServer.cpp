/**@file
 * @brief     Simple Telnet Server (not finished)
 * @author    Igor Lesik 2014
 * @copyright 2014 Igor Lesik
 *
 */
#include "TelnetServer.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <stdexcept>

using namespace gem;

TelnetServer::TelnetServer(MsgHandler handler, Port port):
    m_port(port),
    m_socket(::socket(AF_INET, SOCK_STREAM, 0)),
    m_client_sock(-1),
    m_handler(handler)
{
    if (m_socket == -1) {
        throw std::runtime_error("can't create socket");
    }
}

TelnetServer::~TelnetServer()
{
    stop();
}

void
TelnetServer::stop()
{
    // closing socket interrupts blocking recv() and listen()
    ::close(m_socket);
    m_socket = -1;
    ::close(m_client_sock);
    m_client_sock = -1;
}

TelnetServer::Error
TelnetServer::run(bool once, MsgHandler new_handler)
{
    if (new_handler) m_handler = new_handler;

    Error result { ERR_OK };

    while (m_socket != -1) {
        result = runOnce();
        if (not once and result == ERR_DISCONNECT) continue;
    }

    return result;
}

TelnetServer::Error
TelnetServer::runOnce()
{
    ::listen(m_socket, /*backlog=*/1);

    struct sockaddr_in client; 
    socklen_t c = sizeof(client);
     
    m_client_sock = ::accept(m_socket, (struct sockaddr *)&client, &c);
    if (m_client_sock < 0)
    {
        return ERR_ACCEPT;
    }

    char message[1024*1];
    int read_size;

    //Receive a message from client
    while( (read_size = ::recv(m_client_sock, message, sizeof(message), 0)) > 0 )
    {
        message[read_size] = '\0';
        std::string reply { handle(message) };
        ::write(m_client_sock , reply.c_str(), reply.size());
    }

    ::close(m_client_sock);
    m_client_sock = -1;

    Error returnCode { ERR_OK };

    if (read_size == 0)
    {
        returnCode = ERR_DISCONNECT;
    }
    else if (read_size == -1)
    {
        returnCode = ERR_RECV;
    }

    return returnCode;
}

bool
TelnetServer::create(Port port, size_t attempts, size_t step)
{
    bool created { create(port) };
    if (not created) {
        for (size_t attempt = 0; attempt < attempts; ++attempt) {
            port += step;
            created = create(port);
            if (created) break;
        }
    }

    return created;
}

bool
TelnetServer::create()
{
    struct sockaddr_in server;

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( m_port );
     
    if (::bind(m_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        return false;
    }

    return true;
}

std::string
TelnetServer::handle(const char* message)
{
    if (m_handler) {
        return m_handler(message);
    }

    return "server can't handle request\n";
}

#if (0)

#include <iostream>
#include <signal.h>

static TelnetServer* g_serverPtr = nullptr;

static void signal_catcher(int signo) {
    g_serverPtr->stop();
}

std::string echo(const std::string& message)
{
    return std::string("echo: ") + message;
}

std::string echo2(const std::string& message)
{
    return std::string("echooo:) ") + message;
}

int main()
{
    if (signal(SIGINT, signal_catcher) == SIG_ERR) {
        std::cout << "An error occurred while setting a signal handler.\n";
        return EXIT_FAILURE;
    }

    TelnetServer server(echo);
    g_serverPtr = &server;

    bool created { server.create(4444, 3) };

    if (not created) {
        std::cout << "can't open port " << std::dec << server.getPort() << "\n";

        return 0;
    }

    std::cout << "server port is " << std::dec << server.getPort() << "\n";

    TelnetServer::Error result = server.run(false, echo2);

    std::cout << "server exited with code " << std::dec << result << "\n";

    return 0;
}

#endif
