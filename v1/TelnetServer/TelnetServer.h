/**@file
 * @brief     Simple Telnet Server (not finished)
 * @author    Igor Lesik 2014
 * @copyright 2014 Igor Lesik
 *
 */
#pragma once

#include <cstddef>
#include <functional>
#include <string>

namespace gem {

class TelnetServer
{
public:
    typedef unsigned int Port;
    typedef std::function<std::string (const std::string&)> MsgHandler;

    enum Error {ERR_OK=0, ERR_ACCEPT, ERR_RECV, ERR_DISCONNECT};

public:
    TelnetServer(MsgHandler handler=nullptr, Port port=4444);
    TelnetServer(const TelnetServer&) =delete;
    TelnetServer(const TelnetServer&&) =delete;
   ~TelnetServer();

    Error run(bool once=false, MsgHandler h=nullptr);

    void stop();

    Port getPort() const { return m_port; }
 
    bool create();

    bool create(Port port) {
        m_port = port;
        return create();
    }

    bool create(Port port, size_t attempts, size_t step=1);

protected:
    std::string handle(const char* message);
    Error runOnce();

private:
    Port m_port;
    int m_socket, m_client_sock;
    MsgHandler m_handler;
};

} // namespace gem
