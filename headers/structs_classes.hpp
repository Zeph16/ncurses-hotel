#ifndef SC_H
#define SC_H

#include "includes.hpp"

enum State {back, logged_out, logged_in, booked_in, booked_out};

class User {
public:
  int id = 0;
  std::string username = "dummydata";
  std::string password = "dummydata";
  std::string name = "dummydata";
  time_t creation_time = 0;
  time_t last_login = 0;
  bool logged_in = 0;
};

class Room {
public:
  int id;
  std::string name;
  std::string type;
  std::string bed;
  int userId = 0;
};

class Amenity {
public:
  int id;
  std::string name;
  double price;
};

class History {
public:
  int id;
  int userId;
  int roomId;
  int amenityIds[5];
  bool booked_out;
  time_t time_booked;
  time_t time_left;
};


template <class T>
struct Result {
  T value;
  int status;
};

template <class T>
struct l_node {
  T data;
  l_node<T>* next;
};

template <class T>
struct dll_node {
  T data;
  dll_node<T>* prev;
  dll_node<T>* next;
};

template <class T>
struct tree_node {
  T data;
  tree_node<T>* left;
  tree_node<T>* right;
};

#endif // !SC_H
