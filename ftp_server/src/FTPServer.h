#if !defined FTPServer_H
#define FTPServer_H

#include <list>


#include "ClientConnection.h"

int define_socket_TCP(int port);

class FTPServer {
public:
  FTPServer(int port = 21);
  void run();
  void stop();

private:
  int port;
  int msock;
  std::list<ClientConnection*> connection_list;
};

#endif
