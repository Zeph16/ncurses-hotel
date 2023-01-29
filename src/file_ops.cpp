#include "../headers/header.hpp"

Result<DSdemo<User>> loadUsers() {
  Result<DSdemo<User>> res;
  DSdemo<User> allUsers;

  ifstream users;
  users.open("../storage/users.txt", ios::in);
  if (!users) {
    return res;
  }

  User user;
  while (users.good()) {
    users.read((char *)&user.id, sizeof(user.id));
    getline(users, user.username, '\0');
    getline(users, user.password, '\0');
    getline(users, user.name, '\0');
    users.read((char *)&user.creation_time, sizeof(user.creation_time));
    users.read((char *)&user.last_login, sizeof(user.last_login));
    users.read((char *)&user.logged_in, sizeof(user.logged_in));

    allUsers.enqueue_add(user);
  }

  /* Last item is either added to file twice or added twice for some reason */
  allUsers.remove();
  users.close();

  res.value = allUsers;
  res.status = 1;

  return res;
}

Result<DSdemo<Room>> loadRooms( bool booked) {
  Result<DSdemo<Room>> res;
  DSdemo<Room> roomsList;
  ifstream rooms;
  if (booked) {
    rooms.open("../storage/booked_rooms.txt", ios::in);
  } else {
    rooms.open("../storage/available_rooms.txt", ios::in);
  }
  if (!rooms) {
    return res;
  }

  Room room;
  while (!rooms.eof()) {
    rooms.read((char *)&room.id, sizeof(room.id));
    getline(rooms, room.name, '\0');
    getline(rooms, room.type, '\0');
    getline(rooms, room.bed, '\0');
    rooms.read((char *)&room.userId, sizeof(room.userId));
    roomsList.enqueue_add(room);
  }
  /* Last item is either added to file twice or added twice for some reason */
  roomsList.remove();
  rooms.close();

  res.value = roomsList;
  res.status = 1;


  return res;
}

Result<DSdemo<Amenity>> loadAmenities() {
  Result<DSdemo<Amenity>> res;
  DSdemo<Amenity> allAmenities;
  ifstream amenities;
  amenities.open("../storage/amenities.txt", ios::in);
  if (!amenities) {
    return res;
  }

  Amenity amenity;
  while (!amenities.eof()) {
    amenities.read((char *)&amenity.id, sizeof(amenity.id));
    getline(amenities, amenity.name, '\0');
    amenities.read((char *)&amenity.price, sizeof(amenity.price));
    allAmenities.enqueue_add(amenity);
  }

  /* Last item is either added to file twice or added twice for some reason */
  allAmenities.remove();
  amenities.close();

  res.value = allAmenities;
  res.status = 1;
  return res;
}

Result<DSdemo<History>> loadHistories() {
  Result<DSdemo<History>> res;
  DSdemo<History> allHistories;
  ifstream histories;
  histories.open("../storage/history.txt", ios::in);
  if (!histories) {
    return res;
  }

  History history;
  while (!histories.eof()) {
    histories.read((char *)&history.id, sizeof(history.id));
    histories.read((char *)&history.userId, sizeof(history.userId));
    histories.read((char *)&history.roomId, sizeof(history.roomId));
    histories.read((char *)&history.amenityIds[0], sizeof(history.amenityIds[0]));
    histories.read((char *)&history.amenityIds[1], sizeof(history.amenityIds[1]));
    histories.read((char *)&history.amenityIds[2], sizeof(history.amenityIds[2]));
    histories.read((char *)&history.amenityIds[3], sizeof(history.amenityIds[3]));
    histories.read((char *)&history.amenityIds[4], sizeof(history.amenityIds[4]));
    histories.read((char *)&history.booked_out, sizeof(history.booked_out));
    histories.read((char *)&history.time_booked, sizeof(history.time_booked));
    histories.read((char *)&history.time_left, sizeof(history.time_left));

    allHistories.enqueue_add(history);
  }

  /* Last item is either added to file twice or added twice for some reason */
  allHistories.remove();
  histories.close();

  res.value = allHistories;
  res.status = 1;
  return res;
}


bool saveUsers(DSdemo<User> allUsers) {
  ofstream users;
  User user;
  users.open("../storage/users.txt", ios::trunc);
  Result<User> res = allUsers.index(0);
  for (int i = 0; res.status; i++) {
    user = res.value;
    users.write((char *)&user.id, sizeof(user.id));
    users.write(user.username.c_str(), user.username.length() + 1);
    users.write(user.password.c_str(), user.password.length() + 1);
    users.write(user.name.c_str(), user.name.length() + 1);
    users.write((char *)&user.creation_time, sizeof(user.creation_time));
    users.write((char *)&user.last_login, sizeof(user.last_login));
    users.write((char *)&user.logged_in, sizeof(user.logged_in));
    res = allUsers.index(i + 1);
  }
  users.close();

  return true;
}

bool saveRooms(DSdemo<Room> allRooms, bool booked) {
  ofstream rooms;
  Room room;
  if (booked) {
    rooms.open("../storage/booked_rooms.txt", ios::trunc);
  } else {
    rooms.open("../storage/available_rooms.txt", ios::trunc);
  }
  if (!rooms) {
    return false;
  }

  Result<Room> res = allRooms.index(0);
  for (int i = 0; res.status; i++) {
    room = res.value;
    rooms.write((char *)&room.id, sizeof(room.id));
    rooms.write(room.name.c_str(), room.name.length() + 1);
    rooms.write(room.type.c_str(), room.type.length() + 1);
    rooms.write(room.bed.c_str(), room.bed.length() + 1);
    rooms.write((char *)&room.userId, sizeof(room.userId));
    res = allRooms.index(i + 1);
  }
  rooms.close();

  return true;
}

bool saveAmenities(DSdemo<Amenity> allAmenities) {
  ofstream amenities;
  Amenity amenity;
  amenities.open("../storage/amenities.txt", ios::trunc);
  if (!amenities) {
    return false;
  }

  Result<Amenity> res = allAmenities.index(0);
  for (int i = 0; res.status; i++) {
    amenity = res.value;
    amenities.write((char *)&amenity.id, sizeof(amenity.id));
    amenities.write(amenity.name.c_str(), amenity.name.length() + 1);
    amenities.write((char *)&amenity.price, sizeof(amenity.price));
    res = allAmenities.index(i + 1);
  }
  amenities.close();

  return true;
}

bool saveHistories(DSdemo<History> allHistories) {
  ofstream histories;
  History history;
  histories.open("../storage/history.txt", ios::trunc);
  if (!histories) {
    return false;
  }

  Result<History> res = allHistories.index(0);
  for (int i = 0; res.status; i++) {
    history = res.value;
    histories.write((char *)&history.id, sizeof(history.id));
    histories.write((char *)&history.userId, sizeof(history.userId));
    histories.write((char *)&history.roomId, sizeof(history.roomId));
    histories.write((char *)&history.amenityIds[0], sizeof(history.amenityIds[0]));
    histories.write((char *)&history.amenityIds[1], sizeof(history.amenityIds[1]));
    histories.write((char *)&history.amenityIds[2], sizeof(history.amenityIds[2]));
    histories.write((char *)&history.amenityIds[3], sizeof(history.amenityIds[3]));
    histories.write((char *)&history.amenityIds[4], sizeof(history.amenityIds[4]));
    histories.write((char *)&history.booked_out, sizeof(history.booked_out));
    histories.write((char *)&history.time_booked, sizeof(history.time_booked));
    histories.write((char *)&history.time_left, sizeof(history.time_left));
    res = allHistories.index(i + 1);
  }
  histories.close();

  return true;
}



bool addUserToFile(User user) {
  ofstream users;
  users.open("../storage/users.txt", ios::app);
  if (!users) {
    return false;
  }

  users.write((char *)&user.id, sizeof(user.id));
  users.write(user.username.c_str(), user.username.length() + 1);
  users.write(user.password.c_str(), user.password.length() + 1);
  users.write(user.name.c_str(), user.name.length() + 1);
  users.write((char *)&user.creation_time, sizeof(user.creation_time));
  users.write((char *)&user.last_login, sizeof(user.last_login));
  users.write((char *)&user.logged_in, sizeof(user.logged_in));
  users.close();

  return true;
}

bool addRoomToFile(Room room, bool booked) {
  ofstream rooms;
  if (booked) {
    rooms.open("../storage/booked_rooms.txt", ios::app);
  } else {
    rooms.open("../storage/available_rooms.txt", ios::app);
  }
  if (!rooms) {
    return false;
  }

  rooms.write((char *)&room.id, sizeof(room.id));
  rooms.write(room.name.c_str(), room.name.length() + 1);
  rooms.write(room.type.c_str(), room.type.length() + 1);
  rooms.write(room.bed.c_str(), room.bed.length() + 1);
  rooms.write((char *)&room.userId, sizeof(room.userId));

  rooms.close();

  return true;
}

bool addAmenityToFile(Amenity amenity) {
  ofstream amenities;
  amenities.open("../storage/amenities.txt", ios::app);
  if (!amenities) {
    return false;
  }

  amenities.write((char *)&amenity.id, sizeof(amenity.id));
  amenities.write(amenity.name.c_str(), amenity.name.length() + 1);
  amenities.write((char *)&amenity.price, sizeof(amenity.price));

  amenities.close();

  return true;
}

bool addHistoryToFile(History history) {
  ofstream histories;
  histories.open("../storage/history.txt", ios::app);
  if (!histories) {
    return false;
  }

  histories.write((char *)&history.id, sizeof(history.id));
  histories.write((char *)&history.userId, sizeof(history.userId));
  histories.write((char *)&history.roomId, sizeof(history.roomId));
  histories.write((char *)&history.amenityIds[0], sizeof(history.amenityIds[0]));
  histories.write((char *)&history.amenityIds[1], sizeof(history.amenityIds[1]));
  histories.write((char *)&history.amenityIds[2], sizeof(history.amenityIds[2]));
  histories.write((char *)&history.amenityIds[3], sizeof(history.amenityIds[3]));
  histories.write((char *)&history.amenityIds[4], sizeof(history.amenityIds[4]));
  histories.write((char *)&history.booked_out, sizeof(history.booked_out));
  histories.write((char *)&history.time_booked, sizeof(history.time_booked));
  histories.write((char *)&history.time_left, sizeof(history.time_left));

  histories.close();

  return true;
}
