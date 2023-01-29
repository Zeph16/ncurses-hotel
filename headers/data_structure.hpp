#ifndef DS_H
#define DS_H

#include "structs_classes.hpp"
template <class T>
class DSdemo {

private:
  dll_node<T> *head = NULL;
  dll_node<T> *tail = NULL;


public:
  dll_node<T>* nodeAtIndex(int i);
  int size();
  bool isEmpty();
  void trash();
  Result<T> remove();
  Result<T> index(int i);
  void addAtIndex(T value, int i);
  Result<T> removeAtIndex(int i);
  Result<T> removeById(int id);

  void enqueue_add(T value);
  Result<T> dequeue_pop();

  void push(T value);

  dll_node<T>* search(int id);

  Result<DSdemo<T>> sublist(int i, int j);

  Result<DSdemo<T>> duplicate();
};












template <class T>
int DSdemo<T>::size() {
  if (isEmpty()) {
    return 0;
  }
  int i;
  dll_node<T>* iter = head;
  for (i = 0; iter; i++) {
    iter = iter->next;
  }
  return i;
}

template <class T>
dll_node<T>* DSdemo<T>::nodeAtIndex(int i) {
  if (isEmpty()) {
    return NULL;
  }
  dll_node<T>* node = head;
  int n = size();
  if (i >= n) {
    return NULL;
  }
  for (int iter = 0; iter < i; iter++) {
    node = node->next;
  }
  return node;
}

template <class T>
bool DSdemo<T>::isEmpty() {
  if (head == NULL || tail == NULL) {
    return true;
  }
  return false;
}

template <class T>
Result<T> DSdemo<T>::remove() {
  Result<T> res;
  res.status = 0;
  if (isEmpty()) {
    return res;
  }
  dll_node<T> *temp = tail;
  if (tail->prev) {
    tail->prev->next = NULL;
  } else {
    head = NULL;
  }
  tail = tail->prev;
  T value = temp->data;
  delete temp;
  res.value = value;
  res.status = 1;
  return res;
}

template <class T>
void DSdemo<T>::trash() {
  if (isEmpty()) {
    return;
  }
  while (head) {
    remove();
  }
  return;
}

template <class T>
Result<T> DSdemo<T>::index(int i) {
  Result<T> res;
  dll_node<T> *node = nodeAtIndex(i);
  if (node) {
    res.value = node->data;
    res.status = 1;
  } else {
    res.status = 0;
  }
  return res;
}

template <class T>
void DSdemo<T>::addAtIndex(T value, int i) {
  dll_node<T> *node = nodeAtIndex(i);
  if (node == NULL) {
    return;
  }
  dll_node<T> *newNode = new dll_node<T>;
  newNode->data = value;
  newNode->next = node;
  newNode->prev = node->prev;
  if (node->prev)
    node->prev->next = newNode;
  node->prev = newNode;

  if (node == head) {
    head = newNode;
  }
  if (node == tail) {
    tail = newNode;
  }
}

template <class T>
Result<T> DSdemo<T>::removeAtIndex(int i) {
  Result<T> res;
  dll_node<T> *node = nodeAtIndex(i);
  if (node == NULL) {
    return res;
  }

  if (node == head) {
    if (node->next) {
      head = node->next;
    } else {
    head = NULL;
    }
  }
  if (node == tail) {
    if (node->prev) {
      tail = node->prev;
    } else {
    tail = NULL;
    }
  }
  if (node->prev) {
    node->prev->next = node->next;
  }
  if (node->next){
    node->next->prev = node->prev;
  }
  res.value = node->data;
  res.status = 1;
  delete node;
  return res;
}

template <class T>
Result<T> DSdemo<T>::removeById(int id) {
  Result<T> res;
  res.status = 0;
  if (isEmpty()) {
    return res;
  }
  dll_node<T> *node = search(id);
  if (node == NULL) {
    return res;
  }


  if (node == head) {
    head = node->next;
  }
  if (node == tail) {
    tail = node->prev;
  }
  if (node->prev) {
    node->prev->next = node->next;
  }
  if (node->next){
    node->next->prev = node->prev;
  }

  res.value = node->data;
  res.status = 1;
  delete node;
  return res;
}

template <class T>
void DSdemo<T>::push(T value) {
  dll_node<T> *newNode = new dll_node<T>;
  newNode->data = value;
  newNode->next = head;
  newNode->prev = NULL;
  if (head != NULL) {
    head->prev = newNode;
  } 
  if (tail == NULL) {
    tail = newNode;
  }
  newNode->next = head;
  head = newNode;
}

template <class T>
void DSdemo<T>::enqueue_add(T value) {
  dll_node<T> *newNode = new dll_node<T>;
  newNode->data = value;
  newNode->prev = tail;

  if (head == NULL || tail == NULL) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = tail->next;
  }
}

template <class T>
Result<T> DSdemo<T>::dequeue_pop() {
  Result<T> res;
  if (isEmpty()) {
    res.status = 0;
    return res;
  }
  res.value = head->data;
  res.status = 1;
  dll_node<T> *del = head;
  if (head == tail) {
    head = NULL;
    tail = NULL;
  } else {
    head->next->prev = NULL;
    head = head->next;
  }
  delete del;
  return res;
}

template <class T>
dll_node<T>* DSdemo<T>::search(int id) {
  dll_node<T> *node = nodeAtIndex(0);
  bool found = false;
  while (node) {
    if (node->data.id == id) {
      found = true;
      break;
    }
    node = node->next;
  }
  if (found) {
    return node;
  }
  return NULL;
}

template <class T>
Result<DSdemo<T>> DSdemo<T>::sublist(int start, int end) {
  Result<DSdemo<T>> res;
  res.status = 0;
  DSdemo<T> sublist;
  if (isEmpty()) {
    return res;
  }
  T value;
  dll_node<T> *temp = head;
  int i = 0;
  while (i < start && temp) {
    temp = temp->next;
    i++;
  }
  if (!temp) {
    return res;
  }
  while (i < end && temp) {
    sublist.enqueue_add(temp->data);
    temp = temp->next;
    i++;
  }
  
  if (sublist.isEmpty()) {
    return res;
  }

  res.value = sublist;
  res.status = 1;
  return res;
}

template <class T>
Result<DSdemo<T>> DSdemo<T>::duplicate() {
  Result<DSdemo<T>> returnres; 
  DSdemo<T> dup;
  dll_node<T> *temp = head;
  while (temp) {
    dup.enqueue_add(temp->data);
    temp = temp->next;
  }
  returnres.status = 1;
  returnres.value = dup;
  return returnres;
}
#endif // !DS_H

