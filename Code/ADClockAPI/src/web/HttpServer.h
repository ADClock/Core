#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__
#include "WiFi.h"
#include <Print.h>

class HttpServer : public Print
{
public:
  // An HTTP path handler. The handler function takes the path it
  // registered for as argument, and the Client object to handle the
  // response.
  //
  // The function should return true if it finished handling the request
  // and the connection should be closed.
  typedef boolean (*WebHandlerFn)(HttpServer &web_server);

  enum HttpRequestType
  {
    UNKNOWN_REQUEST,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    ANY,
  };

  typedef struct
  {
    const char *path;
    HttpRequestType type;
    WebHandlerFn handler;
  } PathHandler;

  // Initialize the web server using a NULL terminated array of path
  // handlers, and a NULL terminated array of headers the handlers are
  // interested in.
  //
  // NOTE: Make sure the header names are all lowercase.
  HttpServer(PathHandler handlers[], const char **headers,
             const int port = 80);

  // Call this method to start the HTTP server
  void begin();

  // Handles a possible HTTP request. It will return immediately if no
  // client has connected. Otherwise the request is handled
  // synchronously.
  //
  // Call this method from the main loop() function to have the Web
  // server handle incoming requests.
  void process();

  // Sends the HTTP status code to the connect HTTP client.
  void send_error_code(int code)
  {
    send_error_code(client_, code);
  }
  static void send_error_code(Client &client, int code);

  void send_content_type(const char *content_type);

  // Call this method to indicate the end of the headers.
  inline void end_headers() { client_.println(); }
  static inline void end_headers(Client &client) { client.println(); }

  const char *get_path();
  const HttpRequestType get_type();
  const char *get_header_value(const char *header);
  WiFiClient &get_client() { return client_; }

  // Returns the Body of the Clients Request
  char *get_body();

  // Processes the HTTP headers and assigns values to the requested
  // ones in headers_. Returns true when successful, false in case of
  // errors.
  boolean process_headers();

  // Helper methods

  // Assumes `s' is an HTTP encoded URL, replaces all the escape
  // characters in it and returns the unencoded version. For example
  // for "/index%2Ehtm", this method returns "index.htm".
  //
  // The returned string must be free()d by the caller.
  static char *decode_url_encoded(const char *s);

  // These methods write directly in the response stream of the
  // connected client
  virtual size_t write(uint8_t c);
  virtual size_t write(const char *str);
  virtual size_t write(const uint8_t *buffer, size_t size);

  // Some methods used for testing purposes

  // Returns true if the HTTP request processing should be stopped.
  virtual boolean should_stop_processing() { return !client_.connected(); }

  // Reads a character from the request's input stream. Returns true
  // if the character could be read, false otherwise.
  virtual boolean read_next_char(Client &client, uint8_t *ch);

protected:
  // Returns the field number `which' from buffer. Fields are
  // separated by spaces. Should be a private method, but made public
  // so it can be tested.
  static char *get_field(const char *buffer, int which);

private:
  // The path handlers
  PathHandler *handlers_;

  typedef struct
  {
    const char *header;
    char *value;
  } HeaderValue;

  // The headers
  HeaderValue *headers_;

  // The TCP/IP server we use.
  WiFiServer server_;

  char *path_;
  HttpRequestType request_type_;
  WiFiClient client_;

  // Reads a line from the HTTP request sent by an HTTP client. The
  // line is put in `buffer' and up to `size' characters are written
  // in it.
  boolean get_line(char *buffer, int size);

  // Returns true if the header is marked as requested in the headers_
  // array. As a side effect, the pointer to the actual header is made
  // to point to the one in the headers_ array.
  boolean is_requested_header(const char **header);

  boolean assign_header_value(const char *header, char *value);
};

#endif