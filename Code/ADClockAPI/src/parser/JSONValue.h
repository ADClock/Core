/**
* @author Samuel Mokrani
*
* @section LICENSE
*
* Copyright (c) 2011 mbed
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* @section DESCRIPTION
*    Types Abstraction. Minimalist JSON serializer and parser (inspired by picojson). Is used by MbedJSONRpc.
*
*/

#ifndef _Mbed_RPC_VALUE_H_
#define _Mbed_RPC_VALUE_H_

#define NB_TOKEN 20
/*!< Number maximum of JSONValue in an array or an object */
#include "Arduino.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** JSONValue class
 *
 * Example:
 *    - creation of an JSONValue of type TypeObject containing two others JSONValue: 
 *         -one array of one string and one integer identified by "my_array"
 *         -a boolean identified by "my_boolean"
 *    - serialization in JSON format of this object
 * @code
 * #include "mbed.h"
 * #include "JSONValue.h"
 * #include <string>
 *
 * int main() {          
 *
 *   JSONValue demo;
 *   std::string s;
 *
 *   //fill the object
 *   demo["my_array"][0] = "demo_string";
 *   demo["my_array"][1] = 10;
 *   demo["my_boolean"] = false;
 *
 *   //serialize it into a JSON string
 *   s = demo.serialize();
 *   printf("json: %s\r\n", s.c_str());
 * }
 *  
 * @endcode
 *
 * Example:
 *     - creation of an JSONValue from a JSON string
 *     - extraction of different values from this existing JSONValue
 * @code
 * #include "mbed.h"
 * #include "JSONValue.h"
 * #include <string>
 *
 * int main() {     
 *    JSONValue demo;
 *
 *   const  char * json = "{\"my_array\": [\"demo_string\", 10], \"my_boolean\": true}";
 *
 *   //parse the previous string and fill the object demo
 *   parse(demo, json);
 *
 *   std::string my_str;
 *   int my_int;
 *   bool my_bool;
 *
 *   my_str = demo["my_array"][0].get<std::string>();
 *   my_int = demo["my_array"][1].get<int>();
 *   my_bool = demo["my_boolean"].get<bool>();
 *   
 *    printf("my_str: %s\r\n", my_str.c_str());
 *    printf("my_int: %d\r\n", my_int);
 *    printf("my_bool: %s\r\n", my_bool ? "true" : "false");
 * }
 * @endcode
 */
class JSONValue
{
public:
  /**
    * \enum Type
    * \brief All types which can be used
    */
  enum Type
  {
    TypeNull,    /*!< Null type */
    TypeBoolean, /*!< Boolean */
    TypeInt,     /*!< Integer */
    TypeDouble,  /*!< Double */
    TypeString,  /*!< String */
    TypeArray,   /*!< Array (contains JSONValue) */
    TypeObject   /*!< Object (contains JSONValue identified by a name)*/
  };

  /**
    * JSONValue constructor of type TypeNull
    */
  JSONValue() : _type(TypeNull), index_array(0), index_token(0) {}

  /**
    * JSONValue constructor of type TypeBoolean
    *
    * @param value the object created will be initialized with this boolean
    */
  JSONValue(bool value) : _type(TypeBoolean), index_array(0), index_token(0)
  {
    _value.asBool = value;
  }

  /**
    * JSONValue constructor of type TypeInt
    *
    * @param value the object created will be initialized with this integer
    */
  JSONValue(int value) : _type(TypeInt), index_array(0), index_token(0)
  {
    _value.asInt = value;
  }

  /**
    * JSONValue constructor of type TypeDouble
    *
    * @param value the object created will be initialized with this double
    */
  JSONValue(double value) : _type(TypeDouble), index_array(0), index_token(0)
  {
    _value.asDouble = value;
  }

  /**
    * JSONValue constructor of type TypeString
    *
    * @param value the object created will be initialized with this string
    */
  JSONValue(std::string const &value) : _type(TypeString), index_array(0), index_token(0)
  {
    _value.asString = new std::string(value);
  }

  /**
    * JSONValue constructor of type TypeString
    *
    * @param value the object created will be initialized with this string
    */
  JSONValue(const char *value) : _type(TypeString), index_array(0), index_token(0)
  {
    _value.asString = new std::string(value);
  }

  /**
    * Copy constructor
    *
    * @param rhs object which will be copied
    */
  JSONValue(JSONValue const &rhs) : _type(TypeNull) { *this = rhs; }

  /**
    * Destructor
    */
  ~JSONValue() { clean(); }

  /**
    * = Operator overloading for an JSONValue from an JSONValue
    *
    * @param rhs object
    * @return a reference on the JSONValue affected
    */
  JSONValue &operator=(JSONValue const &rhs);

  /**
    * = Operator overloading for an JSONValue from an int
    *
    * @param rhs integer
    * @return a reference on the JSONValue affected
    */
  JSONValue &operator=(int const &rhs) { return operator=(JSONValue(rhs)); }

  /**
    * = Operator overloading for an JSONValue from a boolean
    *
    * @param rhs boolean
    * @return a reference on the JSONValue affected
    */
  JSONValue &operator=(bool const &rhs) { return operator=(JSONValue(rhs)); }

  /**
    * = Operator overloading for an JSONValue from a double
    *
    * @param rhs double
    * @return a reference on the JSONValue affected
    */
  JSONValue &operator=(double const &rhs) { return operator=(JSONValue(rhs)); }

  /**
    * = Operator overloading for an JSONValue from a string
    *
    * @param rhs string
    * @return a reference on the JSONValue affected
    */
  JSONValue &operator=(const char *rhs) { return operator=(JSONValue(std::string(rhs))); }

  /**
    * [] Operator overloading for an JSONValue.
    * Each TypeObject object can contain an array of NB_TOKEN JSONValue. 
    * This operator is useful to create an array or to retrieve an JSONValue of an existing array.
    *
    * @param i index of the array
    * @return a reference on the JSONValue created or retrieved
    */
  JSONValue &operator[](int i);

  /**
    * [] Operator overloading for an JSONValue.
    * Each TypeObject JSONValue can contain NB_TOKEN JSONValue IDENTIFIED BY A NAME 
    * This operator is useful to create a TypeObject JSONValue or to retrieve an JSONValue of an existing TypeObject.
    *
    *
    * @param str identifier of the sub JSONValue
    * @return a reference on the JSONValue created or retrieved
    */
  JSONValue &operator[](std::string str);

  /**
    * Retrieve the value of an JSONValue object.
    *
    * Let's suppose that we have an JSONValue of type TypeString.
    * To retrieve this string, you have to do:
    *   my_obj.get<std::string>();
    *
    * @return A contant reference on the value of the object
    */
  template <typename T>
  const T &get() const;

  /**
    * Retrieve the value of an JSONValue object.
    *
    * Let's suppose that we have an JSONValue of type TypeInt.
    * To retrieve this integer, you have to do:
    *   my_obj.get<int>();
    *
    * @return A reference on the value of the object
    */
  template <typename T>
  T &get();

  /**
    * Return the type of the JSONValue object
    *
    * @return type of the JSONValue object
    */
  Type const &getType() const
  {
    return _type;
  }

  /**
    * Return the size of an JSONValue object (works for TypeString, TypeArray or TypeObject) 
    *
    * @return size
    */
  int size() const;

  /**
    * Check for the existence in a TypeObject object of member identified by name
    *
    * @param name Identifier
    * @return true if the object is of type TypeObject AND contains a member named "name", false otherwise
    */
  bool hasMember(char *name);

  /**
    * Convert an JSONValue in a JSON frame
    *
    * @return JSON string
    */
  std::string serialize();

protected:
  // object type
  Type _type;

  //indexes of TypeObject and TypeArray
  int index_array;
  int index_token;

  //an object can contain NB_TOKEN tokens. Each token have a name
  JSONValue *token[NB_TOKEN];
  std::string *token_name[NB_TOKEN];

  //an object can contain an array of NB_TOKEN elements
  JSONValue *array[NB_TOKEN];

  // Clean up
  void clean();

  union {
    bool asBool;
    int asInt;
    double asDouble;
    std::string *asString;
  } _value;

  JSONValue &operator[](int i) const { return *(array[i]); }
  JSONValue &operator[](std::string k) const;

  std::string to_str();
  void serialize(std::back_insert_iterator<std::string> os);
};

#define GET(ctype, var)                             \
  template <>                                       \
  inline const ctype &JSONValue::get<ctype>() const \
  {                                                 \
    return var;                                     \
  }                                                 \
  template <>                                       \
  inline ctype &JSONValue::get<ctype>()             \
  {                                                 \
    return var;                                     \
  }
GET(bool, _value.asBool)
GET(double, _value.asDouble)
GET(int, _value.asInt)
GET(std::string, *_value.asString)
#undef GET

//Input class for JSON parser
class input
{
protected:
  const char *cur_;
  const char *end_;
  int last_ch_;
  bool ungot_;
  int line_;

public:
  input(const char *first, const char *last) : cur_(first), end_(last), last_ch_(-1), ungot_(false), line_(1){};

  int getc_()
  {
    if (ungot_)
    {
      ungot_ = false;
      return last_ch_;
    }
    if (cur_ == end_)
    {
      last_ch_ = -1;
      return -1;
    }
    if (last_ch_ == '\n')
    {
      line_++;
    }
    last_ch_ = *cur_++ & 0xff;
    return last_ch_;
  }

  void ungetc()
  {
    if (last_ch_ != -1)
    {
      ungot_ = true;
    }
  }

  const char *cur() const
  {
    return cur_;
  }
  int line() const
  {
    return line_;
  }
  void skip_ws()
  {
    while (1)
    {
      int ch = getc_();
      if (!(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'))
      {
        ungetc();
        break;
      }
    }
  }
  int expect(int expect)
  {
    skip_ws();
    if (getc_() != expect)
    {
      ungetc();
      return false;
    }
    return true;
  }
  bool match(const std::string &pattern)
  {
    for (std::string::const_iterator pi(pattern.begin());
         pi != pattern.end();
         ++pi)
    {
      if (getc_() != *pi)
      {
        ungetc();
        return false;
      }
    }
    return true;
  }
};

inline const char *parse(JSONValue &out, const char *first, const char *last, std::string *err);

/**
* JSON string parser and creation of an JSONValue
*
* @param out reference of an JSONValue which will be filled according to the JSON string
* @param str JSON string
* @return A non empty string if there is a parsing error
*
*/

inline std::string parse(JSONValue &out, const char *str);
inline bool _parse(JSONValue &out, input &in);
inline bool _parse_number(JSONValue &out, input &in);
inline bool _parse_string(JSONValue &out, input &in);
inline bool _parse_array(JSONValue &out, input &in);
inline bool _parse_object(JSONValue &out, input &in);

inline bool _parse_string(JSONValue &out, input &in)
{
#ifdef DEBUG
  printf("string detected\r\n");
#endif
  out = JSONValue(std::string(""));
  std::string &s = out.get<std::string>();
  while (1)
  {
    int ch = in.getc_();
    if (ch < ' ')
    {
      in.ungetc();
      return false;
    }
    else if (ch == '"')
    {
      return true;
    }
    else if (ch == '\\')
    {
      if ((ch = in.getc_()) == -1)
      {
        return false;
      }
      switch (ch)
      {
#define MAP(sym, val) \
  case sym:           \
    s.push_back(val); \
    break
        MAP('"', '\"');
        MAP('\\', '\\');
        MAP('/', '/');
        MAP('b', '\b');
        MAP('f', '\f');
        MAP('n', '\n');
        MAP('r', '\r');
        MAP('t', '\t');
#undef MAP
      default:
        return false;
      }
    }
    else
    {
      s.push_back(ch);
    }
  }
}

inline bool _parse_array(JSONValue &out, input &in)
{
#ifdef DEBUG
  printf("array detected\r\n");
#endif
  int i = 0;
  if (in.expect(']'))
  {
    return true;
  }
  do
  {
    if (!_parse(out[i], in))
    {
      return false;
    }
    i++;
  } while (in.expect(','));
  return in.expect(']');
}

inline bool _parse_object(JSONValue &out, input &in)
{
#ifdef DEBUG
  printf("object detected\r\n");
#endif
  if (in.expect('}'))
  {
    return true;
  }
  do
  {
    JSONValue key, val;
    if (in.expect('"') && _parse_string(key, in) && in.expect(':') && _parse(val, in))
    {
      out[key.get<std::string>().c_str()] = val;
#ifdef DEBUG
      printf("key: %s \r\n", key.get<std::string>().c_str());
#endif
    }
    else
    {
      return false;
    }
  } while (in.expect(','));
  return in.expect('}');
}

inline bool _parse_number(JSONValue &out, input &in)
{
#ifdef DEBUG
  printf("number detected\r\n");
#endif
  std::string num_str;
  while (1)
  {
    int ch = in.getc_();
    if (('0' <= ch && ch <= '9') || ch == '+' || ch == '-' || ch == '.' || ch == 'e' || ch == 'E')
    {
      num_str.push_back(ch);
    }
    else
    {
      in.ungetc();
      break;
    }
  }
  char *endp;
  if (strchr(num_str.c_str(), '.') != NULL || strchr(num_str.c_str(), 'e') != NULL || strchr(num_str.c_str(), '+') != NULL)
    out = JSONValue(strtod(num_str.c_str(), &endp));
  else
    out = JSONValue((int)strtod(num_str.c_str(), &endp));
  return endp == num_str.c_str() + num_str.size();
}

inline bool _parse(JSONValue &out, input &in)
{
  in.skip_ws();
  int ch = in.getc_();
  switch (ch)
  {
#define IS(ch, text, val) \
  case ch:                \
    if (in.match(text))   \
    {                     \
      out = val;          \
      return true;        \
    }                     \
    else                  \
    {                     \
      return false;       \
    }
    IS('n', "ull", JSONValue());
    IS('f', "alse", JSONValue(false));
    IS('t', "rue", JSONValue(true));
#undef IS
  case '"':
    return _parse_string(out, in);
  case '[':
    return _parse_array(out, in);
  case '{':
    return _parse_object(out, in);
  default:
    if (('0' <= ch && ch <= '9') || ch == '-')
    {
      in.ungetc();
      return _parse_number(out, in);
    }
    break;
  }
  in.ungetc();
  return false;
}

inline std::string parse(JSONValue &out, const char *pos)
{
  const char *last = pos + strlen(pos);
  std::string err;
  pos = parse(out, pos, last, &err);
  return err;
}

inline const char *parse(JSONValue &out, const char *first, const char *last, std::string *err)
{
  input in = input(first, last);
  if (!_parse(out, in) && err != NULL)
  {
    char buf[64];
    sprintf(buf, "syntax error at line %d near: ", in.line());
    *err = buf;
    while (1)
    {
      int ch = in.getc_();
      if (ch == -1 || ch == '\n')
      {
        break;
      }
      else if (ch >= ' ')
      {
        err->push_back(ch);
      }
    }
  }
  return in.cur();
}

#endif // _MbedJSONValue_H_