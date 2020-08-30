/*
 * File: ServerAddRequestNet.h
 * 
 * Summary: This file handles clients connecting to add a custom server
 * to the device.
 */

namespace SmartMailBox
{
  namespace ServerNetwork
  {
    class ServerAddRequestNet
    {
    public:

      void setup()
      {
        WiFiServer server_ptr(80);
        m_server_ptr = &server_ptr;

        server_ptr.begin();
      }

      void checkClient()
      {
        WiFiClient client = server.available();
        if (client)
        {
          while (client.connected()) {            // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
              handle(client);
              
            }
          }
        }
      }
    
      void handle(WiFiClient& wifi)
      {
          
      }

    private:
      WiFiServer *m_server_ptr;
    }
  }
}
