#include "../headers/header.hpp"

Result<DSdemo<const char*>> getFeatures(string type) {
  DSdemo<const char *> features;
  Result<DSdemo<const char *>> res;

  if (type == "Standard") {
    features.enqueue_add("Standard TV");
    features.enqueue_add("Basic wifi bandwidth");
    features.enqueue_add("Fit for 2 people");
    res.value = features;
    res.status = 1;
  }

  else if (type == "Deluxe") {
    features.enqueue_add("Extra standard bed");
    features.enqueue_add("Fit for 4 people");
    features.enqueue_add("Wonderful view");
    features.enqueue_add("Appliances for ironing and hairdressing");
    features.enqueue_add("Mini bar, bedside safe");
    features.enqueue_add("Better shower and bathroom robes");
    features.enqueue_add("Extra wifi bandwidth");
    res.value = features;
    res.status = 1;
  }

  else if (type == "Suite") {
    features.enqueue_add("Extra king size bed");
    features.enqueue_add("Fit for a standard family");
    features.enqueue_add("Extra living space isolated from bedroom");
    features.enqueue_add("Mini kitchen");
    features.enqueue_add("Fully equipped modern bathroom");
    features.enqueue_add("Maximum wifi bandwidth");
    res.value = features;
    res.status = 1;
  }

  else {
    res.status = 0;
  }

  return res;
}

Result<Room> getUserRoom(DSdemo<Room> rooms, User u) {
  Result<Room> res;
  res.status = 0;
  dll_node<Room> *r = rooms.nodeAtIndex(0);
  while (r) {
    if (r->data.userId == u.id) {
      res.value = r->data;
      res.status = 1;
      break;
    }
    r = r->next;
  }
  return res;
}

void logoutUser(User u, DSdemo<User> users) {
  u.logged_in = 0;
  dll_node<User> *node = users.search(u.id);
  if (!node) {
    return;
  }
  node->data = u;
  saveUsers(users);
}

void bookoutRoom(Room room, DSdemo<History> &history, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms) {
  Result<History> histres = history.index(0);
  for (int i = 0; histres.status; i++) {
    History hist = histres.value;
    if (hist.roomId == room.id) {
      if (hist.booked_out == false) {
        move(10, 10);
        printw("%s", room.name.c_str());
        hist.booked_out = true;
        hist.time_left = time(0);
        history.removeById(hist.id);
        history.enqueue_add(hist);
        saveHistories(history);
        break;
      }
    }
    histres = history.index(i + 1);
  }

  booked_rooms.removeById(room.id);
  room.userId = 0;
  available_rooms.enqueue_add(room);
  saveRooms(available_rooms, false);
  saveRooms(booked_rooms, true);
}

Result<DSdemo<History>> filterUserHistory(User u, DSdemo<History> history) {
  Result<DSdemo<History>> res;
  DSdemo<History> filtered;
  Result<History> histres = history.index(0);
  if (!histres.status) {
    res.status = 0;
    return res;
  }
  bool empty = true;
  for (int i = 0; histres.status; i++) {
    if (histres.value.userId == u.id) {
      empty = false;
      filtered.enqueue_add(histres.value);
    }
    histres = history.index(i + 1);
  }

  if (empty) {
    res.status = 0;
    return res;
  }
  res.value = filtered;
  res.status = 1;
  return res;
}

Result<Room> getHistoryRoom(History history, DSdemo<Room> rooms) {
  Result<Room> res;
  dll_node<Room> *rnode = rooms.search(history.roomId);
  if (!rnode) {
    res.status = 0;
    return res;
  }
  res.value = rnode->data;
  res.status = 1;
  return res;
}

Result<DSdemo<Amenity>> getHistoryAmenities(History history, DSdemo<Amenity> amenities) {
  Result<DSdemo<Amenity>> res;
  DSdemo<Amenity> amens;
  dll_node<Amenity> *anode;
  bool empty = true;
  for (int i = 0; i < MAX_AMENITIES; i++) {
    if (history.amenityIds[i] == 0){
      break;
    }
    anode = amenities.search(history.amenityIds[i]);
    if (anode) {
      amens.enqueue_add(anode->data);
      empty = false;
    }
  }
  if (empty) {
    res.status = 0;
    return res;
  }
  res.value = amens;
  res.status = 1;
  return res;
}

Result<User> getLoggedInUser(DSdemo<User> users) {
  Result<User> res;
  dll_node<User> *unode = users.nodeAtIndex(0);
  res.status = 0;
  int i = 0;
  move(i, 1);
  while (unode) {
    if (unode->data.logged_in) {
      res.value = unode->data;
      res.status = 1;
      break;
    }
    unode = unode->next;
  }
  return res;
}

Result<double> getHistoryPrice(History hist, DSdemo<Room> allRooms, DSdemo<Amenity> allAmenities) {
  Result<double> res;
  double price = 0;
  res.value = 0;
  res.status = 0;
  dll_node<Room> *rnode = allRooms.search(hist.roomId);
  if (!rnode) {
    return res;
  }
  Room room = rnode->data;
  if (room.bed == "Single") {
    price += SINGLE_PRICE;
  } else if (room.bed == "Double") {
    price += DOUBLE_PRICE;
  } else if (room.bed == "King-Size") {
    price += KING_PRICE;
  }

  double seconds;
  if (hist.booked_out) {
    seconds = difftime(hist.time_left, hist.time_booked);
  } else {
    seconds = difftime(time(0), hist.time_booked);
  }
  if (room.type == "Basic") {
    price += seconds * BASIC_PER_SEC;
  }
  if (room.type == "Deluxe") {
    price += seconds * DELUXE_PER_SEC;
  }
  if (room.type == "Suite") {
    price += seconds * SUITE_PER_SEC;
  }

  dll_node<Amenity> *anode;
  for (int i = 0; i < MAX_AMENITIES; i++) {
    if (hist.amenityIds[i] == 0) {
      break;
    }
    anode = allAmenities.search(hist.amenityIds[i]);
    if (anode) {
      price += anode->data.price;
    }
  }

  res.value = price;
  res.status = 1;
  return res;
}
Result<DSdemo<Room>> filterRooms(DSdemo<Room> rooms, string type, string bed) {
  Result<DSdemo<Room>> res;
  res.status = 0;
  DSdemo<Room> filtered;
  if (!rooms.nodeAtIndex(0)) {
    return res;
  }
  if ((type == "") && (bed == "")) {
    res.value = rooms;
    res.status = 1;
    return res;
  }
  Result<Room> roomres = rooms.index(0);
  Room r;
  bool empty = true;
  for (int i = 0; roomres.status; i++) {
    r = roomres.value;
    if (type != "" && bed != "") {
      if (r.type == type && r.bed == bed) {
        empty = false;
        filtered.enqueue_add(r);
      }
    } else if (type == "" && bed != "") {
      if (r.bed == bed) {
        empty = false;
        filtered.enqueue_add(r);
      }
    } else if ((type != "") && (bed == "")) {
      if (r.type == type) {
        empty = false;
        filtered.enqueue_add(r);
      }
    }
    roomres = rooms.index(i + 1);
  }
  if (empty) {
    return res;
  }
  res.value = filtered;
  res.status = 1;
  return res;
}
