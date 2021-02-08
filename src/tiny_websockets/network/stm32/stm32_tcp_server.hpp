#pragma once

#if defined(__STM32F1__) || defined(__STM32F4__) 

#include <tiny_websockets/internals/ws_common.hpp>
#include <tiny_websockets/network/tcp_client.hpp>
#include <tiny_websockets/network/tcp_server.hpp>
#include <tiny_websockets/network/stm32/stm32_tcp_client.hpp>


namespace websockets { namespace network {
  class STM32TcpServer : public TcpServer {
  public:
    STM32TcpServer() : EthernetServer(uint16_t port) : _port(port) {}

    bool poll() override {
      yield();
      return server.available();
    }

    bool listen(const uint16_t port) override {
      yield();
      //server = EthernetServer(port);
      server.begin(port);
      return available();
    }
    
    TcpClient* accept() override {
      EthernetClient client = server.accept();
      return new STM32TcpClient(client);
    }

    bool available() override {
      yield();
      return static_cast<bool>(server);
    }
    
    void close() override {
      // Not Implemented
    }

    virtual ~STM32TcpServer() {
      // Not Implemented
    }

  protected:
    int getSocket() const override {
      return -1; // Not Implemented
    }

  private:
    uint16_t _port;
    EthernetServer server;
  };
}} // websockets::network

#endif // #ifdef STM32
