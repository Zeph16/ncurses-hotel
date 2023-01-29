#include "../headers/header.hpp"
using namespace std;

void addSampleUsers(DSdemo<User> &allUsers) {
  User u1;
  u1.id = 1;
  u1.username = "firstusername";
  u1.password = "firstpassword";
  u1.name = "firstname";
  u1.creation_time = time(0);
  u1.last_login = time(0);
  u1.logged_in = false;

  User u2;
  u2.id = 2;
  u2.username = "secondusername";
  u2.password = "secondpassword";
  u2.name = "secondname";
  u2.creation_time = time(0);
  u2.last_login = time(0);
  u2.logged_in = false;

  User u3;
  u3.id = 3;
  u3.username = "thirdusername";
  u3.password = "thirdpassword";
  u3.name = "thirdname";
  u3.creation_time = time(0);
  u3.last_login = time(0);
  u3.logged_in = false;

  User u4;
  u4.id = 4;
  u4.username = "fourthusername";
  u4.password = "fourthpassword";
  u4.name = "fourthname";
  u4.creation_time = time(0);
  u4.last_login = time(0);
  u4.logged_in = false;

  allUsers.enqueue_add(u1);
  allUsers.enqueue_add(u2);
  allUsers.enqueue_add(u3);
  allUsers.enqueue_add(u4);
}

void addSampleAvailableRooms(DSdemo<Room> &rooms) {
  Room r1;
  r1.id = 1;
  r1.name = "Room 101";
  r1.type = "Deluxe";
  r1.bed = "Single";
  r1.userId = 0;

  Room r2;
  r2.id = 2;
  r2.name = "Room 202";
  r2.type = "Basic";
  r2.bed = "Double";
  r2.userId = 0;

  Room r3;
  r3.id = 3;
  r3.name = "Room 303";
  r3.type = "Deluxe";
  r3.bed = "King-size";
  r3.userId = 0;

  Room r4;
  r4.id = 4;
  r4.name = "Suite 404";
  r4.type = "Suite";
  r4.bed = "Single";
  r4.userId = 0;

  Room r9;
  r9.id = 9;
  r9.name = "Room 909";
  r9.type = "Basic";
  r9.bed = "King-size";
  r9.userId = 0;

  Room r10;
  r10.id = 10;
  r10.name = "Room 110";
  r10.type = "Deluxe";
  r10.bed = "Single";
  r10.userId = 0;

  Room r11;
  r11.id = 11;
  r11.name = "Room 111";
  r11.type = "Basic";
  r11.bed = "Single";
  r11.userId = 0;

  rooms.enqueue_add(r1);
  rooms.enqueue_add(r2);
  rooms.enqueue_add(r3);
  rooms.enqueue_add(r4);
  rooms.enqueue_add(r9);
  rooms.enqueue_add(r11);
  rooms.enqueue_add(r10);
}

void addSampleBookedRooms(DSdemo<Room> &rooms) {
  Room r5;
  r5.id = 5;
  r5.name = "Room 505";
  r5.type = "Deluxe";
  r5.bed = "Twin";
  r5.userId = 1;

  Room r6;
  r6.id = 6;
  r6.name = "Room 606";
  r6.type = "Basic";
  r6.bed = "Single";
  r6.userId = 2;

  Room r7;
  r7.id = 7;
  r7.name = "Room 707";
  r7.type = "Deluxe";
  r7.bed = "Single";
  r7.userId = 3;

  Room r8;
  r8.id = 8;
  r8.name = "Room 808";
  r8.type = "Basic";
  r8.bed = "Twin";
  r8.userId = 4;

  rooms.enqueue_add(r5);
  rooms.enqueue_add(r6);
  rooms.enqueue_add(r7);
  rooms.enqueue_add(r8);
}

void addSampleAmenities(DSdemo<Amenity> &amenities) {
  Amenity a1;
  a1.id = 1;
  a1.name = "Dry Cleaning";
  a1.price = 10.5;

  Amenity a2;
  a2.id = 2;
  a2.name = "Massages";
  a2.price = 20.5;

  Amenity a3;
  a3.id = 3;
  a3.name = "Sauna";
  a3.price = 30.5;

  Amenity a4;
  a4.id = 4;
  a4.name = "24-hr Room Service";
  a4.price = 40.5;

  Amenity a5;
  a5.id = 5;
  a5.name = "Doctor on demand";
  a5.price = 50.5;

  amenities.enqueue_add(a1);
  amenities.enqueue_add(a2);
  amenities.enqueue_add(a3);
  amenities.enqueue_add(a4);
  amenities.enqueue_add(a5);
}

void addSampleHistory(DSdemo<History> &history) {
  History h1;
  h1.amenityIds[0] = 1;
  h1.amenityIds[1] = 2;
  h1.amenityIds[2] = 0;
  h1.amenityIds[3] = 0;
  h1.amenityIds[4] = 0;
  h1.booked_out = true;
  h1.id = 1;
  h1.roomId = 1;
  h1.time_booked = time(0);
  h1.time_left = time(0);
  h1.userId = 1;

  History h2;
  h2.amenityIds[0] = 2;
  h2.amenityIds[1] = 3;
  h2.amenityIds[2] = 0;
  h2.amenityIds[3] = 0;
  h2.amenityIds[4] = 0;
  h2.booked_out = true;
  h2.id = 2;
  h2.roomId = 2;
  h2.time_booked = time(0);
  h2.time_left = time(0);
  h2.userId = 2;

  History h3;
  h3.amenityIds[0] = 1;
  h3.amenityIds[1] = 2;
  h3.amenityIds[2] = 3;
  h3.amenityIds[3] = 0;
  h3.amenityIds[4] = 0;
  h3.booked_out = true;
  h3.id = 3;
  h3.roomId = 3;
  h3.time_booked = time(0);
  h3.time_left = time(0);
  h3.userId = 3;

  History h4;
  h4.amenityIds[0] = 2;
  h4.amenityIds[1] = 1;
  h4.amenityIds[2] = 0;
  h4.amenityIds[3] = 0;
  h4.amenityIds[4] = 0;
  h4.booked_out = true;
  h4.id = 4;
  h4.roomId = 4;
  h4.time_left = time(0);
  tm *timestruct = localtime(&h4.time_left);
  timestruct->tm_mday -= 1;
  timestruct->tm_hour -= 1;
  h4.time_booked = mktime(timestruct);
  h4.userId = 3;

  History h5;
  h5.amenityIds[0] = 1;
  h5.amenityIds[1] = 2;
  h5.amenityIds[2] = 3;
  h5.amenityIds[3] = 0;
  h5.amenityIds[4] = 0;
  h5.booked_out = true;
  h5.id = 5;
  h5.roomId = 5;
  h5.time_left = time(0);
  timestruct = localtime(&h5.time_left);
  timestruct->tm_mday -= 1;
  timestruct->tm_hour -= 1;
  h5.time_booked = mktime(timestruct);
  h5.userId = 3;

  history.enqueue_add(h1);
  history.enqueue_add(h2);
  history.enqueue_add(h3);
  history.enqueue_add(h4);
  history.enqueue_add(h5);
}
int main ()
{
  initscr();
  noecho();
  curs_set(0);
  DSdemo<User> allUsers;
  DSdemo<Room> available_rooms;
  DSdemo<Room> booked_rooms;
  DSdemo<Amenity> allAmenities;
  DSdemo<History> history;

  User current_user;
  Result<User> res;
  if (!loadUsers().status) {
    addSampleUsers(allUsers);
    addSampleAmenities(allAmenities);
    addSampleAvailableRooms(available_rooms);
    addSampleBookedRooms(booked_rooms);
    addSampleHistory(history);

    saveUsers(allUsers);
    saveAmenities(allAmenities);
    saveRooms(available_rooms, false);
    saveRooms(booked_rooms, true);
    saveHistories(history);
  }



  Result<DSdemo<User>> usersres = loadUsers();
  Result<DSdemo<Room>> availres = loadRooms(false);
  Result<DSdemo<Room>> bookedres = loadRooms(true);
  Result<DSdemo<Amenity>> amenres = loadAmenities();
  Result<DSdemo<History>> histres = loadHistories();
  if (!usersres.status) {
    mvprintw(0, 0, "Error while loading users\n");
    getch();
    endwin();
    return 0;
  }
  if (!availres.status) {
    mvprintw(0, 0, "Error while loading available rooms\n");
    getch();
    endwin();
    return 0;
  }
  if (!bookedres.status) {
    mvprintw(0, 0, "Error while loading booked rooms\n");
    getch();
    endwin();
    return 0;
  }
  if (!amenres.status) {
    mvprintw(0, 0, "Error while loading amenities\n");
    getch();
    endwin();
    return 0;
  }
  if (!histres.status) {
    mvprintw(0, 0, "Error while loading history\n");
    getch();
    endwin();
    return 0;
  }


  allUsers = usersres.value;
  allAmenities = amenres.value;
  available_rooms = availres.value;
  booked_rooms = bookedres.value;
  history = histres.value;


  while (true) {


    res = login_signup(allUsers);
    if (!res.status) {
      endwin();
      return 0;
    }
    current_user = res.value;
    State state = landingPage(current_user, allUsers, available_rooms, booked_rooms, allAmenities, history);
    if (state == State::logged_out) {
      logoutUser(current_user, allUsers);
    } else {
      endwin();
      return 0;
    }
  }

  endwin();
  return 0;
}

