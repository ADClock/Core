// -*- c++ -*-
//
// Copyright 2010 Ovidiu Predescu <ovidiu@gmail.com>
// Date: May 2010
//
// Updated: 08-JAN-2012 for Arduno IDE 1.0 by <Hardcore@hardcoreforensics.com>
// Updated: 29-MAR-2013 replacing strtoul with parseHexChar by <shin@marcsi.ch>
// Updated: 21-JUL-2019 get it workin again by Krüger
//
// HttpServer for Arduino. - Jetzt auch irgendwie für ESP
//

// 10 milliseconds read timeout
#define READ_TIMEOUT 10

#include "Arduino.h"

extern "C"
{

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
}

#include "HttpServer.h"

// Temporary buffer.
static char buffer[160];

void *malloc_check(size_t size)
{
  void *r = malloc(size);
  return r;
}

HttpServer::HttpServer(PathHandler handlers[],
                       const char **headers,
                       const int port)
    : handlers_(handlers),
      server_(WiFiServer(port)),
      path_(NULL),
      request_type_(UNKNOWN_REQUEST),
      client_(WiFiClient(255))
{
  if (headers)
  {
    int size = 0;
    for (int i = 0; headers[i]; i++)
    {
      size++;
    }
    headers_ = (HeaderValue *)malloc_check(sizeof(HeaderValue) * (size + 1));
    if (headers_)
    {
      for (int i = 0; i < size; i++)
      {
        headers_[i].header = headers[i];
        headers_[i].value = NULL;
      }
      headers_[size].header = NULL;
    }
  }
}

void HttpServer::begin()
{
  server_.begin();
}

// Process headers.
boolean HttpServer::process_headers()
{
  if (headers_)
  {
    // First clear the header values from the previous HTTP request.
    for (int i = 0; headers_[i].header; i++)
    {
      if (headers_[i].value)
      {
        free(headers_[i].value);
        // Ensure the pointer is cleared once the memory is freed.
        headers_[i].value = NULL;
      }
    }
  }

  enum State
  {
    ERROR,
    START_LINE,
    HEADER_NAME,
    HEADER_VALUE,
    HEADER_VALUE_SKIP_INITIAL_SPACES,
    HEADER_IGNORE_VALUE,
    END_HEADERS,
  };
  State state = START_LINE;

  char ch;
  int pos;
  const char *header;
  uint32_t start_time = millis();
  while (1)
  {
    if (should_stop_processing())
    {
      return false;
    }
    if (millis() - start_time > READ_TIMEOUT)
    {
      return false;
    }
    if (!read_next_char(client_, (uint8_t *)&ch))
    {
      continue;
    }
    start_time = millis();
    switch (state)
    {
    case START_LINE:
      if (ch == '\r')
      {
        break;
      }
      else if (ch == '\n')
      {
        state = END_HEADERS;
      }
      else if (isalnum(ch) || ch == '-')
      {
        pos = 0;
        buffer[pos++] = ch;
        state = HEADER_NAME;
      }
      else
      {
        state = ERROR;
      }
      break;

    case HEADER_NAME:
      if (pos + 1 >= sizeof(buffer))
      {
        state = ERROR;
        break;
      }
      if (ch == ':')
      {
        buffer[pos] = 0;
        header = buffer;
        if (is_requested_header(&header))
        {
          state = HEADER_VALUE_SKIP_INITIAL_SPACES;
        }
        else
        {
          state = HEADER_IGNORE_VALUE;
        }
        pos = 0;
      }
      else if (isalnum(ch) || ch == '-')
      {
        buffer[pos++] = ch;
      }
      else
      {
        state = ERROR;
        break;
      }
      break;

    case HEADER_VALUE_SKIP_INITIAL_SPACES:
      if (pos + 1 >= sizeof(buffer))
      {
        state = ERROR;
        break;
      }
      if (ch != ' ')
      {
        buffer[pos++] = ch;
        state = HEADER_VALUE;
      }
      break;

    case HEADER_VALUE:
      if (pos + 1 >= sizeof(buffer))
      {
        state = ERROR;
        break;
      }
      if (ch == '\n')
      {
        buffer[pos] = 0;
        if (!assign_header_value(header, buffer))
        {
          state = ERROR;
          break;
        }
        state = START_LINE;
      }
      else
      {
        if (ch != '\r')
        {
          buffer[pos++] = ch;
        }
      }
      break;

    case HEADER_IGNORE_VALUE:
      if (ch == '\n')
      {
        state = START_LINE;
      }
      break;

    default:
      break;
    }

    if (state == END_HEADERS)
    {
      break;
    }
    if (state == ERROR)
    {
      return false;
    }
  }
  return true;
}

void HttpServer::process()
{
  client_ = server_.available();
  if (!client_.connected() || !client_.available())
  {
    return;
  }

  boolean is_complete = get_line(buffer, sizeof(buffer));
  if (!buffer[0])
  {
    return;
  }
  if (!is_complete)
  {
    // The requested path is too long.
    send_error_code(414);
    client_.stop();
    return;
  }

  char *request_type_str = get_field(buffer, 0);
  request_type_ = UNKNOWN_REQUEST;
  if (!strcmp("GET", request_type_str))
  {
    request_type_ = GET;
  }
  else if (!strcmp("POST", request_type_str))
  {
    request_type_ = POST;
  }
  else if (!strcmp("PUT", request_type_str))
  {
    request_type_ = PUT;
  }
  else if (!strcmp("DELETE", request_type_str))
  {
    request_type_ = DELETE;
  }

  path_ = get_field(buffer, 1);

  // Process the headers.
  if (!process_headers())
  {
    // Malformed header line.
    send_error_code(417);
    client_.stop();
  }
  // Header processing finished. Identify the handler to call.

  boolean should_close = true;
  boolean found = false;
  for (int i = 0; handlers_[i].path; i++)
  {
    int len = strlen(handlers_[i].path);
    boolean exact_match = !strcmp(path_, handlers_[i].path);
    boolean regex_match = false;
    if (handlers_[i].path[len - 1] == '*')
    {
      regex_match = !strncmp(path_, handlers_[i].path, len - 1);
    }
    if ((exact_match || regex_match) && (handlers_[i].type == ANY || handlers_[i].type == request_type_))
    {
      found = true;
      should_close = (handlers_[i].handler)(*this);
      break;
    }
  }

  if (!found)
  {
    send_error_code(404);
  }
  if (should_close)
  {
    client_.stop();
  }

  free(path_);
  free(request_type_str);
}

boolean HttpServer::is_requested_header(const char **header)
{
  if (!headers_)
  {
    return false;
  }
  for (int i = 0; headers_[i].header; i++)
  {
    if (!strcmp(*header, headers_[i].header))
    {
      *header = headers_[i].header;
      return true;
    }
  }
  return false;
}

boolean HttpServer::assign_header_value(const char *header, char *value)
{
  if (!headers_)
  {
    return false;
  }
  boolean found = false;
  for (int i = 0; headers_[i].header; i++)
  {
    // Use pointer equality, since `header' must be the pointer
    // inside headers_.
    if (header == headers_[i].header)
    {
      headers_[i].value = (char *)malloc_check(strlen(value) + 1);
      if (!headers_[i].value)
      {
        return false;
      }
      strcpy(headers_[i].value, value);
      found = true;
      break;
    }
  }
  return found;
}

void HttpServer::send_error_code(Client &client, int code)
{
  client.print(F("HTTP/1.1 "));
  client.print(code, DEC);
  client.print(F(" OK\r\n"));
  if (code != 200)
  {
    end_headers(client);
  }
}

void HttpServer::send_content_type(const char *content_type)
{
  client_.print("Content-Type: ");
  client_.println(content_type);
}

const char *HttpServer::get_path() { return path_; }

const HttpServer::HttpRequestType HttpServer::get_type()
{
  return request_type_;
}

const char *HttpServer::get_header_value(const char *name)
{
  if (!headers_)
  {
    return NULL;
  }
  for (int i = 0; headers_[i].header; i++)
  {
    if (!strcmp(headers_[i].header, name))
    {
      return headers_[i].value;
    }
  }
  return NULL;
}

int parseHexChar(char ch)
{
  if (isdigit(ch))
  {
    return ch - '0';
  }
  ch = tolower(ch);
  if (ch >= 'a' && ch <= 'e')
  {
    return ch - 'a' + 10;
  }
  return 0;
}

char *HttpServer::decode_url_encoded(const char *s)
{
  if (!s)
  {
    return NULL;
  }
  char *r = (char *)malloc_check(strlen(s) + 1);
  if (!r)
  {
    return NULL;
  }
  char *r2 = r;
  const char *p = s;
  while (*s && (p = strchr(s, '%')))
  {
    if (p - s)
    {
      memcpy(r2, s, p - s);
      r2 += (p - s);
    }
    // If the remaining number of characters is less than 3, we cannot
    // have a complete escape sequence. Break early.
    if (strlen(p) < 3)
    {
      // Move the new beginning to the value of p.
      s = p;
      break;
    }
    uint8_t r = parseHexChar(*(p + 1)) << 4 | parseHexChar(*(p + 2));
    *r2++ = r;
    p += 3;

    // Move the new beginning to the value of p.
    s = p;
  }
  // Copy whatever is left of the string in the result.
  int len = strlen(s);
  if (len > 0)
  {
    strncpy(r2, s, len);
  }
  // Append the 0 terminator.
  *(r2 + len) = 0;

  return r;
}

size_t HttpServer::write(uint8_t c)
{
  return client_.write(c);
}

size_t HttpServer::write(const char *str)
{
  return client_.write(str);
}

size_t HttpServer::write(const uint8_t *buffer, size_t size)
{
  return client_.write(buffer, size);
}

boolean HttpServer::read_next_char(Client &client, uint8_t *ch)
{
  if (!client.available())
  {
    return false;
  }
  else
  {
    *ch = client.read();
    return true;
  }
}

boolean HttpServer::get_line(char *buffer, int size)
{
  int i = 0;
  char ch;

  buffer[0] = 0;
  for (; i < size - 1; i++)
  {
    if (!read_next_char(client_, (uint8_t *)&ch))
    {
      continue;
    }
    if (ch == '\n')
    {
      break;
    }
    buffer[i] = ch;
  }
  buffer[i] = 0;
  return i < size - 1;
}

// Returns a newly allocated string containing the field number `which`.
// The first field's index is 0.
// The caller is responsible for freeing the returned value.
char *HttpServer::get_field(const char *buffer, int which)
{
  char *field = NULL;
  int i = 0;
  int field_no = 0;
  int size = strlen(buffer);

  // Locate the field we need. A field is defined as an area of
  // non-space characters delimited by one or more space characters.
  for (; field_no < which; field_no++)
  {
    // Skip over space characters
    while (i < size && isspace(buffer[i]))
    {
      i++;
    }
    // Skip over non-space characters.
    while (i < size && !isspace(buffer[i]))
    {
      i++;
    }
  }

  // Now we identify the end of the field that we want.
  // Skip over space characters.
  while (i < size && isspace(buffer[i]))
  {
    i++;
  }

  if (field_no == which && i < size)
  {
    // Now identify where the field ends.
    int j = i;
    while (j < size && !isspace(buffer[j]))
    {
      j++;
    }

    field = (char *)malloc_check(j - i + 1);
    if (!field)
    {
      return NULL;
    }
    memcpy(field, buffer + i, j - i);
    field[j - i] = 0;
  }
  return field;
}

char *HttpServer::get_body()
{
  char *buffer;
  client_.readBytes(buffer, 32000);
  return buffer;
}