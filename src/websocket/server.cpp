//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

//------------------------------------------------------------------------------

// Report a failure
void fail(boost::system::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

// Echoes back all received WebSocket messages
class session : public std::enable_shared_from_this<session> {
    websocket::stream<tcp::socket> ws;
    boost::asio::strand<boost::asio::io_context::executor_type> strand;
    boost::beast::multi_buffer buffer;

public:
    // Take ownership of the socket
    explicit session(tcp::socket socket)
        : ws(std::move(socket))
        , strand(ws.get_executor())
    { }

    // Start the asynchronous operation
    void run() {
        // Accept the websocket handshake
        ws.async_accept(
            boost::asio::bind_executor(
                strand,
                std::bind(
                    &session::on_accept,
                    shared_from_this(),
                    std::placeholders::_1
                )
            )
        );
    }

    void on_accept(boost::system::error_code ec) {
        if (ec) {
            fail(ec, "accept");
            return;
        }

        // Read a message
        do_read();
    }

    void do_read() {
        // Read a message into our buffer
        ws.async_read(
            buffer,
            boost::asio::bind_executor(
                strand,
                std::bind(
                    &session::on_read,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            )
        );
    }

    void on_read( boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        // This indicates that the session was closed
        if (ec == websocket::error::closed) {
            return;
        }

        if (ec) {
            fail(ec, "read");
        }

        // Echo the message
        ws.text(ws.got_text());
        ws.async_write(
            buffer.data(),
            boost::asio::bind_executor(
                strand,
                std::bind(
                    &session::on_write,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            )
        );
    }

    void on_write( boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if (ec) {
            fail(ec, "write");
            return;
        }

        // Clear the buffer
        buffer.consume(buffer.size());

        // Do another read
        do_read();
    }
};

//------------------------------------------------------------------------------

// Accepts incoming connections and launches the sessions
class listener : public std::enable_shared_from_this<listener> {
    tcp::acceptor acceptor;
    tcp::socket sock;

public:
    listener(
        boost::asio::io_context& ioc,
        tcp::endpoint endpoint)
        : acceptor(ioc)
        , sock(ioc)
    {
        boost::system::error_code ec;

        // Open the acceptor
        acceptor.open(endpoint.protocol(), ec);
        if (ec) {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
        if (ec) {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor.bind(endpoint, ec);
        if (ec) {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
        if (ec) {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void run() {
        if (! acceptor.is_open()) {
            return;
        }
        do_accept();
    }

    void do_accept() {
        acceptor.async_accept(
            sock,
            std::bind(
                &listener::on_accept,
                shared_from_this(),
                std::placeholders::_1
            )
        );
    }

    void on_accept(boost::system::error_code ec) {
        if (ec) {
            fail(ec, "accept");
        } else {
            // Create the session and run it
            std::make_shared<session>(std::move(sock))->run();
        }

        // Accept another connection
        do_accept();
    }
};

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 4) {
        std::cerr <<
            "Usage: websocket-server-async <address> <port> <threads>\n"
            "Example:\n"
            "    websocket-server-async 0.0.0.0 8080 1\n";
        return EXIT_FAILURE;
    }
    auto const address = boost::asio::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));

    // The io_context is required for all I/O
    boost::asio::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<listener>(ioc, tcp::endpoint{address, port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
        v.emplace_back([&ioc]{ ioc.run(); });
    }
    ioc.run();

    return EXIT_SUCCESS;
}

