/**
 * Socks5BalancerAsio : A Simple TCP Socket Balancer for balance Multi Socks5 Proxy Backend Powered by Boost.Asio
 * Copyright (C) <2020>  <Jeremie>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <exception>
#include "TcpRelayServer.h"
#include "UpstreamPool.h"
#include "StateMonitorServer.h"
#include "ConfigLoader.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    try {
        boost::asio::io_context ioc;
        boost::asio::executor ex = boost::asio::make_strand(ioc);

        auto configLoader = std::make_shared<ConfigLoader>();
        configLoader->load(R"(config-test.json)");
        configLoader->print();

        auto upstreamPool = std::make_shared<UpstreamPool>(ex);
        upstreamPool->setConfig(configLoader);
//        for (int i = 0; i != 100; ++i) {
//            auto s = upstreamPool->getServerBasedOnAddress();
//            std::cout << "[" << i << "] getServerBasedOnAddress:" << (s ? s->print() : "nullptr") << "\n";
//        }
//        std::cout << std::endl;

        auto tcpRelay = std::make_shared<TcpRelayServer>(ex, configLoader, upstreamPool);
        auto stateMonitor = std::make_shared<StateMonitorServer>(boost::asio::make_strand(ioc));

        tcpRelay->start();

        ioc.run();

    } catch (const std::exception e) {
        std::cerr << "catch std::exception: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "catch (...) exception: " << "\n";
    }

    return 0;
}
