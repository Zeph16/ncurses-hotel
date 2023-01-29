#include "../headers/header.hpp"
#include <curses.h>

Result<User> login_signup(DSdemo<User> &allUsers) {
  Result<User> userRes;
  User currentUser;

  userRes = getLoggedInUser(allUsers);
  if (userRes.status) {
    return userRes;
  }

  string menu_items[3] = {"Login", "Sign up", "Exit"};
  while (true) {
    clear();
    attron(A_BOLD);
    attron(A_UNDERLINE);
    mvprintw(0, 0, "LOGIN/SIGN UP\n\n");
    attroff(A_UNDERLINE);
    Result<string> sel_result = menuLetters(menu_items, 3);
    attroff(A_BOLD);

    if (!sel_result.status) {
      return {currentUser, 0};
    }
    string sel = sel_result.value;
    if (sel == "Login") {
      userRes = login(allUsers);
      if (!userRes.status) {
        continue;
      } else {
        return userRes;
      }
    } else if (sel == "Sign up") {
      userRes = signup(allUsers);
      if (!userRes.status) {
        continue;
      } else {
        return userRes;
      }
    } else {
      bool ex = confirmPage("exit");
      if (ex) {
        return {currentUser, 0};
      }
    }
  }
}


Result<User> login(DSdemo<User> allUsers) {
  Result<User> res;
  User u;
  echo();

  clear();
  mvprintw(1, 3, "LOGIN");
  WINDOW *usernamewin = newwin(3, 30, 3, 3);
  box(usernamewin, 0, 0);
  wmove(usernamewin, 0, 1);
  wattron(usernamewin, A_REVERSE);
  wprintw(usernamewin, "Username");
  wattroff(usernamewin, A_REVERSE);
  refresh();
  wrefresh(usernamewin);
  wmove(usernamewin, 1, 2);
  string username = getstring(usernamewin);

  WINDOW *passwordwin = newwin(3, 30, 6, 3);
  box(passwordwin, 0, 0);
  wmove(passwordwin, 0, 1);
  wattron(passwordwin, A_REVERSE);
  wprintw(passwordwin, "Password");
  wattroff(passwordwin, A_REVERSE);
  refresh();
  wrefresh(passwordwin);
  wmove(passwordwin, 1, 2);
  string password = getstring(passwordwin);


  res = allUsers.index(0);
  for (int i = 0; res.status; i++) {
    if (res.value.username == username) {
      if (res.value.password == password) {
        break;
      }
    }
    res = allUsers.index(i + 1);
  }
  if (!res.status) {
    move(9, 4);
    printw("Username or password invalid\n");
    refresh();
    getchar();
    return res;
  }
  u = res.value;
  u.logged_in = true;
  u.last_login = time(0);
  dll_node<User> *node = allUsers.search(u.id);
  if (!node) {
    res.status = 0;
    return res;
  }
  node->data = u;
  saveUsers(allUsers);
  res.value = u;
  return res;
}

Result<User> signup(DSdemo<User> &allUsers) {
  Result<User> res;
  User u;

  bool repeat = true;
  bool found = false;
  clear();
  mvprintw(1, 3, "SIGN UP");
  WINDOW *inputwin = newwin(3, 30, 3, 3);
  while (repeat) {
    res = allUsers.index(0);
    if (!res.status) {
      return res;
    }
    box(inputwin, 0, 0);
    wmove(inputwin, 0, 1);
    wattron(inputwin, A_REVERSE);
    wprintw(inputwin, "Username");
    wattroff(inputwin, A_REVERSE);
    refresh();
    wrefresh(inputwin);
    wmove(inputwin, 1, 2);
    u.username = getstring(inputwin);


    for (int i = 0; res.status; i++) {
      if (res.value.username == u.username) {
        found = true;
        move(6, 3);
        printw("Username already exists...\n");
        refresh();
        wclear(inputwin);
        break;
      }
      res = allUsers.index(i + 1);
    }
    if (!found) {
      repeat = false;
    }
    found = false;
  }
  wclear(inputwin);
  move(6, 3);
  clrtoeol();
  box(inputwin, 0, 0);
  wmove(inputwin, 0, 1);
  wattron(inputwin, A_REVERSE);
  wprintw(inputwin, "Password");
  wattroff(inputwin, A_REVERSE);
  refresh();
  wrefresh(inputwin);
  wmove(inputwin, 1, 2);
  u.password = getstring(inputwin);

  wclear(inputwin);
  wmove(inputwin, 0, 1);
  box(inputwin, 0, 0);
  wattron(inputwin, A_REVERSE);
  wprintw(inputwin, "Name");
  wattroff(inputwin, A_REVERSE);
  refresh();
  wrefresh(inputwin);
  wmove(inputwin, 1, 2);
  u.name = getstring(inputwin);
  u.id = allUsers.index(allUsers.size() - 1).value.id + 1;

  u.logged_in = true;
  u.creation_time = time(0);
  u.last_login = time(0);
  allUsers.enqueue_add(u);
  string msg = "Successfully registered as " + u.name + "!";
  WINDOW *msgwin = newwin(3, msg.length() + 1, 3, 4);
  clear();
  box(msgwin, 0, 0);
  wmove(msgwin, 1, 1);
  wprintw(msgwin, "Succesfully registered as %s!", u.name.c_str());
  refresh();
  wrefresh(msgwin);
  getch();
  addUserToFile(u);

  res.value = u;
  res.status = 1;
  return res;
}


State landingPage(User currentUser, DSdemo<User> &allUsers, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms, DSdemo<Amenity> &amenities, DSdemo<History> &history) {
  Result<Room> res;
  State state;
  while (true) {
    res = getUserRoom(booked_rooms, currentUser);
    if (res.status) {
      Room r = res.value;
      state = HasRoomPage(currentUser, r, allUsers, available_rooms, booked_rooms, history, amenities);
      if (state != State::booked_out) {
        return state;
      }
    } else {
      state = NoRoomPage(currentUser, allUsers, available_rooms, booked_rooms, amenities, history);
      if (state != State::booked_in) {
        return state;
      }
    }
  }
}

State HasRoomPage(User user, Room room, DSdemo<User> &allUsers, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms, DSdemo<History> &histories, DSdemo<Amenity> amenities) {
  clear();
  refresh();

  int winwidth = 50;
  int winheight = 11;
  string prof = "[P]rofile";
  string lg = "[L]og out";
  string msg = "No room currently booked";
  string bk = "[B]ook out of " + room.name;
  string ext = "[E]xit";
  WINDOW *profile = newwin(1, prof.length() + 2, 1, winwidth - prof.length() - lg.length() - 2);
  WINDOW *logout = newwin(1, lg.length() + 2, 1, winwidth - lg.length());;
  WINDOW *roomwin = newwin(winheight, winwidth, 4, 1);
  WINDOW *menuwin = newwin(2, winwidth - 8, winheight + 4, 5);
  State state;
  char input;
  char in;
  int mode = 0;

  DSdemo<Room> allRooms;
  dll_node<Room> *rnode;

  rnode = available_rooms.nodeAtIndex(0);
  while (rnode) {
    allRooms.enqueue_add(rnode->data);
    rnode = rnode->next;
  }
  rnode = booked_rooms.nodeAtIndex(0);
  while (rnode) {
    allRooms.enqueue_add(rnode->data);
    rnode = rnode->next;
  }

  History hist;
  Result<History> histres = histories.index(0);
  bool found = false;
  for (int i = 0; histres.status; i++) {
    if (histres.value.roomId == room.id) {
      if (!histres.value.booked_out) {
        hist = histres.value;
        found = true;
        break;
      }
    }
    histres = histories.index(i + 1);
  }
  if (!found) {
    clear();
    refresh();
    printw("No history found");
    getch();
    return State::logged_out;
  }
  while (true) {
    clear();
    refresh();
    wclear(profile);
    wclear(logout);
    wclear(roomwin);
    wclear(menuwin);
    move(3, 1);
    printw("Welcome %s, you are currently in %s!", user.name.c_str(), room.name.c_str());

    wattron(profile, A_UNDERLINE);
    wattron(logout, A_UNDERLINE);
    wattron(menuwin, A_UNDERLINE);
    wattron(profile, A_BOLD);
    wattron(logout, A_BOLD);
    wattron(menuwin, A_BOLD);
    wmove(profile, 0, 0);
    wprintw(profile, "%s", prof.c_str());
    wmove(logout, 0, 0);
    wprintw(logout, "%s", lg.c_str());
    wmove(menuwin, 0, (winwidth - 8 - bk.length()) / 2);
    wprintw(menuwin, "%s", bk.c_str());
    wmove(menuwin, 1, (winwidth - 8 - ext.length()) / 2);
    wprintw(menuwin, "%s", ext.c_str());

    switch (mode) {

      case 0:
        showHistoryDetails(roomwin, winwidth, winheight, hist, allRooms, amenities);
        break;
      case 1:
        showAmenityDetails(roomwin, winwidth, winheight, hist.amenityIds, amenities);
        break;
      case 2:
        showFeatures(roomwin, winwidth, winheight, room.id, allRooms);
        break;
    }
    box(roomwin, 0, 0);


    wrefresh(profile);
    wrefresh(logout);
    wrefresh(roomwin);
    wrefresh(menuwin);
    refresh();

    while (true) {
      input = getch();
      in = tolower(input);
      if ((in == 'p') || (in == 'b') || (in == 'l') || (in == 'e')) {
        break;
      }
      if ((in == 'a') && (mode == 0)) {
        mode = 1;
        break;
      }
      if ((in == 'f') && (mode == 0)) {
        mode = 2;
        break;
      }
      if ((in == 'd') && (mode != 0)) {
        mode = 0;
        break;
      }
    }

    if (in == 'p') {
      showProfile(user, allUsers, allRooms, amenities, histories);
    } else if (in == 'l') {
      bool logout = confirmPage("log out");
      if (logout) {
        return State::logged_out;
      } else {
        continue;
      }
    } else if (in == 'b') {
      bool confirm = confirmPage("book out of " + room.name);
      if (confirm) {
        room.userId = 0;
        available_rooms.enqueue_add(room);
        booked_rooms.removeById(room.id);
        Result<History> histres = histories.index(0);
        for (int i = 0; histres.status; i++) {
          History hist = histres.value;
          if (hist.roomId == room.id) {
            if (hist.booked_out == false) {
              move(10, 10);
              printw("%s", room.name.c_str());
              hist.booked_out = true;
              hist.time_left = time(0);
              histories.removeById(hist.id);
              histories.enqueue_add(hist);
              saveHistories(histories);
              break;
            }
          }
          histres = histories.index(i + 1);
        }
        saveRooms(available_rooms, false);
        saveRooms(booked_rooms, true);
        return State::booked_out;
      }
    } else if (in == 'e') {
      bool ex = confirmPage("exit");
      if (ex) {
        return State::back;
      }
    }
  }
  return State::booked_out;
}

State NoRoomPage(User user, DSdemo<User> &allUsers, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms, DSdemo<Amenity> &amenities, DSdemo<History> &history) {
  int winwidth = 50;
  int winheight = 11;
  string prof = "[P]rofile";
  string lg = "[L]og out";
  string msg = "No room currently booked";
  string bk = "[B]ook a room";
  string ext = "[E]xit";
  WINDOW *profile = newwin(1, prof.length() + 2, 1, winwidth - prof.length() - lg.length() - 2);
  WINDOW *logout = newwin(1, lg.length() + 2, 1, winwidth - lg.length());;
  WINDOW *roomwin = newwin(winheight, winwidth, 3, 1);
  WINDOW *menuwin = newwin(2, winwidth - 8, winheight + 3, 5);
  State state;
  char input;
  char in;

  DSdemo<Room> allRooms;
  dll_node<Room> *rnode;

  rnode = available_rooms.nodeAtIndex(0);
  while (rnode) {
    allRooms.enqueue_add(rnode->data);
    rnode = rnode->next;
  }
  rnode = booked_rooms.nodeAtIndex(0);
  while (rnode) {
    allRooms.enqueue_add(rnode->data);
    rnode = rnode->next;
  }

  while (true) {
    clear();
    refresh();
    wclear(profile);
    wclear(logout);
    wclear(roomwin);
    wclear(menuwin);
    move(2, 1);
    printw("Welcome %s!", user.name.c_str());
    // box(profile, 0, 0);
    // box(logout, 0, 0);
    box(roomwin, 0, 0);
    // box(menuwin, 0, 0);

    wattron(profile, A_UNDERLINE);
    wattron(logout, A_UNDERLINE);
    wattron(menuwin, A_UNDERLINE);
    wattron(profile, A_BOLD);
    wattron(logout, A_BOLD);
    wattron(menuwin, A_BOLD);
    wmove(profile, 0, 0);
    wprintw(profile, "%s", prof.c_str());
    wmove(logout, 0, 0);
    wprintw(logout, "%s", lg.c_str());
    wmove(roomwin, winheight / 2, (winwidth - msg.length())/ 2);
    wprintw(roomwin, "%s", msg.c_str());
    wmove(menuwin, 0, (winwidth - 8 - bk.length()) / 2);
    wprintw(menuwin, "%s", bk.c_str());
    wmove(menuwin, 1, (winwidth - 8 - ext.length()) / 2);
    wprintw(menuwin, "%s", ext.c_str());


    wrefresh(profile);
    wrefresh(logout);
    wrefresh(roomwin);
    wrefresh(menuwin);
    refresh();

    input = getch();
    while (true) {
      in = tolower(input);
      if ((in == 'p') || (in == 'b') || (in == 'l') || (in == 'e')) {
        break;
      }
      input = getch();
    }

    if (in == 'p') {
      showProfile(user, allUsers, allRooms, amenities, history);
    } else if (in == 'l') {
      bool logout = confirmPage("log out");
      if (logout) {
        return State::logged_out;
      } else {
        continue;
      }
    } else if (in == 'b') {
      State state = roomsMenu(user, available_rooms, booked_rooms, amenities, history);
      if (state == State::booked_in) {
        return state;
      }
    } else if (in == 'e') {
      bool ex = confirmPage("exit");
      if (ex) {
        return State::back;
      }
    }
  }

  getch();
  return State::back;
}

void showProfile(User &u, DSdemo<User> &allUsers, DSdemo<Room> allRooms, DSdemo<Amenity> amenities, DSdemo<History> histories) {
  clear();
  refresh();

  int winwidth = 50;
  int winheight = 11;
  string ret = "[R]eturn";
  WINDOW* back = newwin(1, ret.length() + 1, 1, winwidth - ret.length());
  WINDOW* main = newwin(winheight, winwidth, 3, 1);
  WINDOW* menu = newwin(2, winwidth, winheight + 3, 1);

  string username = "Username";
  string name = "Name";
  string created_at = "Joined At";
  string lastlogin = "Last Login";
  string date_create = ctime(&u.creation_time);
  string date_login = ctime(&u.last_login);

  string edit = "[E]dit profile";
  string history = "[H]istory";

  char input;
  char in;


  while (true) {
    clear();
    refresh();
    wclear(back);
    wclear(main);
    wclear(menu);

    move(2, 1);
    printw("Your profile");

    wmove(back, 1, 1);
    wattron(back, A_UNDERLINE);
    wattron(back, A_BOLD);
    wprintw(back, "%s", ret.c_str());
    wattroff(back, A_UNDERLINE);
    wattroff(back, A_BOLD);

    wmove(main, 2, (winwidth / 2 - username.length()) / 2);
    wprintw(main, "%s", username.c_str());
    wmove(main, 3, (winwidth / 2 - u.username.length()) / 2);
    wattron(main, A_BOLD);
    wprintw(main, "%s", u.username.c_str());
    wattroff(main, A_BOLD);
    wmove(main, 2, (winwidth - name.length()) / 2 + winwidth / 4);
    wprintw(main, "%s", name.c_str());
    wmove(main, 3, (winwidth - u.name.length()) / 2 + winwidth / 4);
    wattron(main, A_BOLD);
    wprintw(main, "%s", u.name.c_str());
    wattroff(main, A_BOLD);
    wmove(main, 5, (winwidth - created_at.length()) / 2);
    wprintw(main, "%s", created_at.c_str());
    wmove(main, 6, (winwidth - date_create.length()) / 2);
    wattron(main, A_BOLD);
    wprintw(main, "%s", date_create.c_str());
    wattroff(main, A_BOLD);
    wmove(main, 8, (winwidth - lastlogin.length()) / 2);
    wprintw(main, "%s", lastlogin.c_str());
    wmove(main, 9, (winwidth - date_login.length()) / 2);
    wattron(main, A_BOLD);
    wprintw(main, "%s", date_login.c_str());
    wattroff(main, A_BOLD);
    box(main, 0, 0);

    wattron(menu, A_UNDERLINE);
    wattron(menu, A_BOLD);
    wmove(menu, 0, (winwidth - edit.length()) / 2);
    wprintw(menu, "%s", edit.c_str());
    wmove(menu, 1, (winwidth - history.length()) / 2);
    wprintw(menu, "%s", history.c_str());
    wattroff(menu, A_UNDERLINE);
    wattroff(menu, A_BOLD);


    wrefresh(back);
    wrefresh(main);
    wrefresh(menu);
    input = getch();
    in = tolower(input);
    switch(in) {
      case 'e':
        editProfile(u, allUsers);

        break;
      case 'h':
        userHistoryMenu(u, allRooms, histories, amenities);

        break;
      case 'r':
        return;
    }
  }
}

void editProfile(User &u, DSdemo<User> &allUsers) {
  Result<User> res;

  string username;
  string password1;
  string password2;
  string name;

  bool repeat = true;
  bool found = false;

  clear();
  mvprintw(1, 3, "EDIT YOUR PROFILE");
  WINDOW *inputwin = newwin(3, 30, 3, 3);
  WINDOW *infowin = newwin(2, 40, 7, 3);
  wattron(infowin, A_BOLD);

  while (repeat) {
    box(inputwin, 0, 0);
    wmove(inputwin, 0, 1);
    wattron(inputwin, A_REVERSE);
    wprintw(inputwin, "Username");
    wattroff(inputwin, A_REVERSE);
    wmove(infowin, 0, 0);
    wprintw(infowin, "Current username: %s", u.username.c_str());
    wmove(infowin, 1, 0);
    wprintw(infowin, "Submit 0 anytime to cancel editing");
    refresh();
    wrefresh(inputwin);
    wrefresh(infowin);
    wmove(inputwin, 1, 2);
    username = getstring(inputwin);
    if (username == "0") {
      return;
    }


    res = allUsers.index(0);
    for (int i = 0; res.status; i++) {
      if (res.value.username == username) {
        found = true;
        move(6, 3);
        printw("Username already exists...\n");
        refresh();
        wclear(inputwin);
        break;
      }
      res = allUsers.index(i + 1);
    }
    if (!found) {
      repeat = false;
    }
    found = false;
  }

  move(6, 3);
  clrtoeol();
  wclear(inputwin);
  wclear(infowin);
  wmove(inputwin, 0, 1);
  box(inputwin, 0, 0);
  wattron(inputwin, A_REVERSE);
  wprintw(inputwin, "Name");
  wattroff(inputwin, A_REVERSE);
  wmove(infowin, 0, 0);
  wprintw(infowin, "Current name: %s", u.name.c_str());
  wmove(infowin, 1, 0);
  wprintw(infowin, "Submit 0 anytime to cancel editing");
  refresh();
  wrefresh(inputwin);
  wrefresh(infowin);
  wmove(inputwin, 1, 2);
  name = getstring(inputwin);
  if (name == "0") {
    return;
  }

  wclear(infowin);
  wmove(infowin, 1, 0);
  wprintw(infowin, "Submit 0 anytime to cancel editing");
  while (true) {
    wclear(inputwin);
    box(inputwin, 0, 0);
    wmove(inputwin, 0, 1);
    wattron(inputwin, A_REVERSE);
    wprintw(inputwin, "Current Password");
    wattroff(inputwin, A_REVERSE);
    refresh();
    wrefresh(inputwin);
    wrefresh(infowin);
    wmove(inputwin, 1, 2);
    password1 = getstring(inputwin);
    if (password1 == "0") {
      return;
    }
    if (password1 != u.password) {
      move(6, 3);
      printw("Password doesn't match!");
      refresh();
      wclear(inputwin);
    } else {
      break;
    }

  }

  move(6, 0);
  clrtoeol();
  wclear(inputwin);
  wclear(infowin);
  box(inputwin, 0, 0);
  wmove(inputwin, 0, 1);
  wattron(inputwin, A_REVERSE);
  wprintw(inputwin, "New Password");
  wattroff(inputwin, A_REVERSE);
  wmove(infowin, 0, 0);
  wprintw(infowin, "Retype current password to keep it");
  wmove(infowin, 1, 0);
  wprintw(infowin, "Submit 0 anytime to cancel editing");
  refresh();
  wrefresh(inputwin);
  wrefresh(infowin);
  wmove(inputwin, 1, 2);
  password2 = getstring(inputwin);
  if (password2 == "0") {
    return;
  }

  u.username = username;
  u.name = name;
  u.password = password2;

  dll_node<User> *unode = allUsers.search(u.id);
  if (!unode) {
    return;
  }
  unode->data = u;
  saveUsers(allUsers);

  clear();
  int msgwidth;
  WINDOW *msgwin;
  string msg = "Successfully edited your profile!";
  string msg2 = "New password: " + password2;
  if (password2 != password1) {
    msgwidth = msg.length() > msg2.length() ? msg.length() : msg2.length();
    msgwidth += 2;
    msgwin = newwin(4, msgwidth, 3, 4);
    wattron(msgwin, A_BOLD);
    wmove(msgwin, 2, 1);
    wprintw(msgwin, "%s", msg2.c_str());
    wattroff(msgwin, A_BOLD);
  } else {
    msgwin = newwin(3, msg.length() + 2, 3, 4);
  }
  box(msgwin, 0, 0);
  wmove(msgwin, 1, 1);
  wprintw(msgwin, "%s", msg.c_str());
  refresh();
  wrefresh(msgwin);
  getch();
}


void userHistoryMenu(User user, DSdemo<Room> allRooms, DSdemo<History> allHistories, DSdemo<Amenity> allAmenities) {
  clear();
  refresh();
  string bk = "[R]eturn";
  int winwidth = 50;
  int winheight = 11;

  WINDOW* back = newwin(1, bk.length() + 1, 1, winwidth - bk.length());
  WINDOW* main = newwin(winheight, winwidth, 3, 1);
  WINDOW* prevnext = newwin(1, winwidth, winheight + 3, 1);
  WINDOW* stats = newwin(5, winwidth, winheight + 5, 1);

  move(2, 1);
  printw("Your history");

  wmove(back, 1, 1);
  wattron(back, A_UNDERLINE);
  wattron(back, A_BOLD);
  wprintw(back, "%s", bk.c_str());
  wattroff(back, A_UNDERLINE);
  wattroff(back, A_BOLD);

  int mode = 1;
  dll_node<History> *hnode = NULL;
  Result<DSdemo<History>> userHistoryRes = filterUserHistory(user, allHistories);
  DSdemo<History> userHistory;
  if (userHistoryRes.status) {
    userHistory = userHistoryRes.value;
    hnode = userHistory.nodeAtIndex(0);
  }
  if (!hnode) {
    mode = 0;
  }
  string msg;
  string prevstr = "[P]revious";
  string nextstr = "[N]ext";
  string pricestr = "Total money spent:";
  string timestr = "Total time spent:";
  string bookedstr = "Number of bookings:";

  double total_price = 0;
  double total_time = 0;
  int total_booked = 0;
  double timedup;
  dll_node<History> *temp = hnode;
  while (temp) {
    if (temp->data.booked_out) {
      total_price += getHistoryPrice(temp->data, allRooms, allAmenities).value;
      total_time += difftime(temp->data.time_left, temp->data.time_booked);
    } else {
      total_time += difftime(temp->data.time_booked, time(0));
    }
    total_booked++;
    temp = temp->next;
  }

  wmove(stats, 1, 3);
  wprintw(stats, "%s %.2f", pricestr.c_str(), total_price);

  wmove(stats, 2, 3);
  wprintw(stats, "%s ", timestr.c_str());
  int days = (int)(total_time / 86400);
  int hours = (int)(((int)total_time % 86400) / 3600);
  int minutes = (((int)total_time % 86400) % 3600) / 60;
  int seconds = ((((int)total_time % 86400) % 3600) % 60) % 60;
  if (days) {
    wprintw(stats, "%dD ", days);
  }
  if (hours) {
    wprintw(stats, "%dH ", hours);
  }
  if (minutes) {
    wprintw(stats, "%dM ", minutes);
  }
  wprintw(stats, "%ds ", seconds);


  wmove(stats, 3, 3);
  wprintw(stats, "%s %d", bookedstr.c_str(), total_booked);
  box(stats, 0, 0);
  wmove(stats, 0, 1);
  wprintw(stats, "Stats");

  wrefresh(stats);
  wrefresh(back);
  refresh();

  int total_history;
  int current_history;
  if (mode != 0) {
    total_history = userHistory.size();
    current_history = 1;
  }

  char input;
  char in;


  wattron(prevnext, A_UNDERLINE);
  wattron(prevnext, A_BOLD);
  while (true) {
    refresh();
    wclear(main);
    wclear(prevnext);
    switch (mode) {
      case 0:
        msg = "No previous history";
        wmove(main, winheight/2, (winwidth - msg.length())/2);
        wprintw(main, "%s", msg.c_str());
        break;
      case 1:
        showHistoryDetails(main, winwidth, winheight, hnode->data, allRooms, allAmenities);
        break;
      case 2:
        showAmenityDetails(main, winwidth, winheight, hnode->data.amenityIds, allAmenities);
        break;
      case 3:
        showFeatures(main, winwidth, winheight, hnode->data.roomId, allRooms);
        break;
    }


    box(main, 0, 0);
    if (mode != 0) {
      if (hnode->prev) {
        wmove(prevnext, 0, (winwidth / 2 - prevstr.length()) / 2);
        wprintw(prevnext, "%s", prevstr.c_str());
      }
      if (hnode->next) {
        wmove(prevnext, 0, (winwidth - nextstr.length()) / 2 + winwidth / 4);
        wprintw(prevnext, "%s", nextstr.c_str());
      }
      wmove(main, 0, 1);
      wprintw(main, "Booking %d/%d", current_history, total_history);
    }
    refresh();
    wrefresh(main);
    wrefresh(prevnext);

    while (true) {
      input = getch();
      in = tolower(input);
      if (mode == 0) {
        if (in == 'r') {
          return;
        } else {
          continue;
        }
      }
      if (in == 'r') {
        return;
      }
      if ((in == 'p') && hnode->prev) {
        hnode = hnode->prev;
        current_history--;
        break;
      }
      if ((in == 'n') && hnode->next) {
        hnode = hnode->next;
        current_history++;
        break;
      }
      if ((in == 'a') && (mode == 1)) {
        mode = 2;
        break;
      }
      if ((in == 'f') && (mode == 1)) {
        mode = 3;
        break;
      }
      if ((in == 'd') && (mode != 1)) {
        mode = 1;
        break;
      }
    }

    // Show history number
    // Room name type and bed
    // Fix statistics
    //
  }
}




State roomsMenu(User user, DSdemo<Room>& available_rooms, DSdemo<Room>& booked_rooms, DSdemo<Amenity> &amenities, DSdemo<History>&history) {
  clear();
  refresh();
  string back = "[R]eturn";
  string title = "Available Rooms";
  string msg = "No rooms available at the moment";
  string filterbed = "";
  string filtertype = "";
  WINDOW* backwin = newwin(1, back.length(), 1, (50 - back.length()));
  WINDOW* titlewin = newwin(1, title.length(), 2, (50 - title.length()) / 2);
  wattron(backwin, A_BOLD);
  wattron(backwin, A_UNDERLINE);
  wattron(titlewin, A_BOLD);

  bool noroom = false;
  bool filter_changed = false;

  int page_size = 4;





  int winwidth = 17;
  int winheight = 6;

  WINDOW *roomwins[10];
  int winx = 8;
  int winy = 4;
  int iwin;
  for (iwin = 0; iwin < page_size; iwin++) {
    roomwins[iwin] = newwin(winheight, winwidth, winy, winx);
    winx += winwidth + 1;
    if (iwin % 2 != 0) {
      winx = 8;
      winy += winheight;
    }
  }

  WINDOW* prevnext;
  WINDOW* filterswin;
  if (iwin % 2 == 0) {
    prevnext = newwin(1, 50, winy, 1);
    filterswin = newwin(5, 50, winy + 2, 1);
  } else {
    prevnext = newwin(1, 50, winy + winheight, 1);
    filterswin = newwin(5, 50, winy + winheight + 2, 1);
  }
  winx = 8;
  winy = 4;



  refresh();
  


  Result<DSdemo<Room>> filteredres = filterRooms(available_rooms, filtertype, filterbed);
  DSdemo<Room> filtered;
  DSdemo<DSdemo<Room>> pages;
  Result<DSdemo<Room>> roomssublist; 
  DSdemo<Room> current;
  dll_node<DSdemo<Room>> *pnode = NULL;
  if (filteredres.status) {
    filtered = filteredres.value;
    roomssublist = filtered.sublist(0, page_size);
    for (int i = 0; roomssublist.status; i++) {
      pages.enqueue_add(roomssublist.value);
      roomssublist = filtered.sublist((i + 1) * page_size, (i + 1) * page_size + page_size);
    }
    pnode = pages.nodeAtIndex(0); // SEGMENTATION FAULT!
  }

  char input;
  string type = "";
  string bed = "";
  string prevstr = "[P]revious";
  string nextstr = "[N]ext";
  bool selected = false;
  int roomindex = 0;
  wattron(prevnext, A_BOLD);
  wattron(prevnext, A_UNDERLINE);
  while (true) {
    if (filter_changed) {
      filteredres = filterRooms(available_rooms, filtertype, filterbed);
      pages.trash();
      if (filteredres.status) {
        filtered = filteredres.value;
        roomssublist = filtered.sublist(0, page_size);
        for (int i = 0; roomssublist.status; i++) {
          pages.enqueue_add(roomssublist.value);
          roomssublist = filtered.sublist((i + 1) * page_size, (i + 1) * page_size + page_size);
        }
      }
      pnode = pages.nodeAtIndex(0);
      filter_changed = false;
    }
    clear();
    refresh();

    wclear(filterswin);
    

    for (int i = 0; i < page_size; i++) {
      wclear(roomwins[i]);
    }

    wmove(backwin, 0, 0);
    wprintw(backwin, "%s", back.c_str());
    wmove(titlewin, 0, 0);
    wprintw(titlewin, "%s", title.c_str());
    if (!pnode) {
      attron(A_BOLD);
      move(winy + ((page_size / 2 + 1) * winheight) / 2, winx + ((winwidth * 2) - msg.length()) / 2);
      printw("No available rooms at the moment");
      attroff(A_BOLD);
    }
    else {
      current = pnode->data;
      Result<Room> roomres = current.index(0);
      int loc = 0;
      for (int i = 0; roomres.status; i++) {
        Room r = roomres.value;
        wmove(roomwins[i], 1, (winwidth - r.name.length()) / 2);
        wattron(roomwins[i], A_BOLD);
        wprintw(roomwins[i], "%s", r.name.c_str());
        wattroff(roomwins[i], A_BOLD);

        type = r.type + " Room";
        wmove(roomwins[i], 3, (winwidth - type.length()) / 2);
        wprintw(roomwins[i], "%s", type.c_str());

        bed = r.bed + " Bed";
        wmove(roomwins[i], 4, (winwidth - bed.length()) / 2);
        wprintw(roomwins[i], "%s", bed.c_str());

        box(roomwins[i], 0, 0);
        wmove(roomwins[i], 0, 2);
        
        wattron(roomwins[i], A_BOLD);
        wprintw(roomwins[i], "[%d]", i + 1);
        wattroff(roomwins[i], A_BOLD);
        wrefresh(roomwins[i]);
        roomres = current.index(i + 1);
      }


      if (pnode->prev) {
        wmove(prevnext, 0, (50 / 2 - prevstr.length()) / 2);
        wprintw(prevnext, "%s", prevstr.c_str());
      }
      if (pnode->next) {
        wmove(prevnext, 0, (50 - nextstr.length()) / 2 + 50 / 4);
        wprintw(prevnext, "%s", nextstr.c_str());
      } 
    }


    box(filterswin, 0, 0);
    wmove(filterswin, 0, 2);
    wprintw(filterswin, "Filters");

    wmove(filterswin, 1, 2);
    wprintw(filterswin, "Type: ");
    wattron(filterswin, A_BOLD);
    if (filtertype == "Basic") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 1, 9);
    wprintw(filterswin, "[B]asic");
    if (filtertype == "Basic") {
      wattroff(filterswin, A_STANDOUT);
    }
    if (filtertype == "Deluxe") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 1, 22);
    wprintw(filterswin, "[D]eluxe");
    if (filtertype == "Deluxe") {
      wattroff(filterswin, A_STANDOUT);
    }
    if (filtertype == "Suite") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 1, 33);
    wprintw(filterswin, "S[u]ite");
    if (filtertype == "Suite") {
      wattroff(filterswin, A_STANDOUT);
    }

    wattroff(filterswin, A_BOLD);
    wmove(filterswin, 3, 2);
    wprintw(filterswin, " Bed: ");
    wattron(filterswin, A_BOLD);
    if (filterbed == "Single") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 3, 9);
    wprintw(filterswin, "[S]ingle");
    if (filterbed == "Single") {
      wattroff(filterswin, A_STANDOUT);
    }
    if (filterbed == "Double") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 3, 22);
    wprintw(filterswin, "D[o]uble");
    if (filterbed == "Double") {
      wattroff(filterswin, A_STANDOUT);
    }
    if (filterbed == "King-Size") {
      wattron(filterswin, A_STANDOUT);
    }
    wmove(filterswin, 3, 33);
    wprintw(filterswin, "[K]ing-Size");
    if (filterbed == "King-Size") {
      wattroff(filterswin, A_STANDOUT);
    }
    


    wrefresh(backwin);
    wrefresh(titlewin);
    wrefresh(prevnext);
    wrefresh(filterswin);
    while (true) {  
      input = tolower(getch());
      if (input == 'p' && pnode) {
        if (pnode->prev) {
          pnode = pnode->prev;
          break;
        }
      }
      if (input == 'n' && pnode) {
        if (pnode->next) {
          pnode = pnode->next;
          break;
        }
      }
      if (input == 'r') {
        return State::back;
      }
      if (input == 'b') {
        if (filtertype == "Basic") {
          filtertype = "";
        } else {
          filtertype = "Basic";
        }
        filter_changed = true;
        break;
      }
      if (input == 'd') {
        if (filtertype == "Deluxe") {
          filtertype = "";
        } else {
          filtertype = "Deluxe";
        }
        filter_changed = true;
        break;
      }
      if (input == 'u') {
        if (filtertype == "Suite") {
          filtertype = "";
        } else {
          filtertype = "Suite";
        }
        filter_changed = true;
        break;
      }
      if (input == 's') {
        if (filterbed == "Single") {
          filterbed = "";
        } else {
          filterbed = "Single";
        }
        filter_changed = true;
        break;
      }
      if (input == 'o') {
        if (filterbed == "Double") {
          filterbed = "";
        } else {
          filterbed = "Double";
        }
        filter_changed = true;
        break;
      }
      if (input == 'k') {
        if (filterbed == "King-Size") {
          filterbed = "";
        } else {
          filterbed = "King-Size";
        }
        filter_changed = true;
        break;
      }
      for (int i = 49; i < current.size() + 49; i++) {
        if (input == i) {
          selected = true;
          roomindex = i - 49;
          break;
        }
      }
      if (selected) {
        break;
      }
    }

    if (selected) {
      selected = false;
      Room r = current.index(roomindex).value;
      State state = roomDetails(r.id, filtered);
      if (state == State::back) {
        continue;
      } else {
        Result<DSdemo<Amenity>> pickedres = pickAmenities(amenities);
        if (!pickedres.status) {
          continue;
        }
        History hist;
        hist.id = history.index(history.size() - 1).value.id + 1;
        hist.userId = user.id;
        hist.roomId = r.id;
        Result<Amenity> amenres = pickedres.value.index(0);
        for (int i = 0; amenres.status; i++) {
          hist.amenityIds[i] = amenres.value.id;
          amenres = pickedres.value.index(i + 1);
        }
        bool confirm = confirmBooking(hist, r, available_rooms, amenities);
        if (confirm) {
          hist.time_booked = time(0);
          hist.booked_out = false;
          r.userId = user.id;
          booked_rooms.enqueue_add(r);
          available_rooms.removeById(r.id);
          history.enqueue_add(hist);
          saveRooms(available_rooms, false);
          saveRooms(booked_rooms, true);
          saveHistories(history);
          return State::booked_in;
        }
      }
    }
  }


  return State::back;
}

State roomDetails(int roomId, DSdemo<Room> rooms) {
  dll_node<Room> *rnode = rooms.search(roomId);
  if (!rnode) {
    return State::back;
  }
  Room room = rnode->data;
  string back = "[R]eturn";
  string title = room.name + " details";
  string typestr = "Type";
  string bedstr = "Bed";
  string basestr = "Base Price";
  string persecstr = "Price Per Second";
  string features = "[F]eatures";
  string book = "[B]ook this room";
  int winwidth = 50;
  int winheight = 11;
  char input;

  WINDOW* backwin = newwin(1, back.length(), 1, (50 - back.length()));
  WINDOW* titlewin = newwin(1, title.length(), 2, (50 - title.length()) / 2);
  WINDOW* main = newwin(winheight, winwidth, 3, 1);
  WINDOW* bookwin = newwin(1, book.length(), winheight + 4, (50 - book.length()) / 2);

  clear();
  refresh();

  int mode = 0;
  wmove(backwin, 0, 0);
  wprintw(backwin, "%s", back.c_str());
  wmove(titlewin, 0, 0);
  wprintw(titlewin, "%s", title.c_str());
  wattron(bookwin, A_BOLD);
  wattron(bookwin, A_UNDERLINE);
  wmove(bookwin, 0, 0);
  wprintw(bookwin, "%s", book.c_str());
  wrefresh(backwin);
  wrefresh(titlewin);
  wrefresh(bookwin);
  refresh();

  while (true) {
    wclear(main);
    box(main, 0, 0);

    if (mode == 0) {
      wmove(main, 2, (winwidth / 2 - typestr.length()) / 2);
      wprintw(main, "%s", typestr.c_str());
      wmove(main, 3, (winwidth / 2 - room.type.length()) / 2);
      wattron(main, A_BOLD);
      wprintw(main, "%s", room.type.c_str());
      wattroff(main, A_BOLD);

      wmove(main, 2, (winwidth - bedstr.length()) / 2 + winwidth / 4);
      wprintw(main, "%s", bedstr.c_str());
      wmove(main, 3, (winwidth - room.bed.length()) / 2 + winwidth / 4);
      wattron(main, A_BOLD);
      wprintw(main, "%s", room.bed.c_str());
      wattroff(main, A_BOLD);

      wmove(main, 4, (winwidth - basestr.length()) / 2);
      wprintw(main, "%s", basestr.c_str());
      wmove(main, 5, (winwidth - 4) / 2);
      wattron(main, A_BOLD);
      if (room.bed == "Single") {
        wprintw(main, "%d", SINGLE_PRICE);
      } else if (room.bed == "Double") {
        wprintw(main, "%d", DOUBLE_PRICE);
      } else {
        wprintw(main, "%d", KING_PRICE);
      }
      wattroff(main, A_BOLD);

      wmove(main, 6, (winwidth - persecstr.length()) / 2);
      wprintw(main, "%s", persecstr.c_str());
      wmove(main, 7, (winwidth - 4) / 2);
      wattron(main, A_BOLD);
      if (room.type == "Basic") {
        wprintw(main, "%.2f", BASIC_PER_SEC);
      } else if (room.type == "Deluxe") {
        wprintw(main, "%.2f", DELUXE_PER_SEC);
      } else {
        wprintw(main, "%.2f", SUITE_PER_SEC);
      }
      wattroff(main, A_BOLD);

      wmove(main, 9, (winwidth - features.length()) / 2);
      wattron(main, A_BOLD);
      wattron(main, A_UNDERLINE);
      wprintw(main, "%s", features.c_str());
      wattroff(main, A_BOLD);
      wattroff(main, A_UNDERLINE);
    } else {
      showFeatures(main, winwidth, winheight, roomId, rooms);
    }
    wrefresh(main);

    while (true) {
      input = tolower(getch());
      if (input == 'r') {
        return State::back;
      }
      if (input == 'b') {
        return State::booked_in;
      }
      if (input == 'f' && !mode) {
        mode = 1;
        break;
      }
      if (input == 'd' && mode) {
        mode = 0;
        break;
      }
    }

  }

}

Result<DSdemo<Amenity>> pickAmenities(DSdemo<Amenity> allAmenities) {
  DSdemo<Amenity> allAmenitiesDup = allAmenities.duplicate().value;
  Result<DSdemo<Amenity>> returnres;
  returnres.status = 1;
  DSdemo<Amenity> pickedAmenities;
  clear();
  refresh();
  string confirmstr = "[C]onfirm Amenities";
  string titlestr = "Pick optional amenities";
  string backstr = "[R]eturn to rooms list";
  string changedstr = "Added ";
  string pricestr = "Total Price: ";
  string prevstr = "[P]revious";
  string nextstr = "[N]ext";
  string namestr = "Name";
  string pstr = "Price";
  string msg = "No more amenities";
  string dots = "..................................";
  string undostr = "[U]ndo previous selection";

  int page_size = 4;
  int winwidth = 40;
  WINDOW* backwin = newwin(1, backstr.length(), 1, (50 - backstr.length()));
  WINDOW* titlewin = newwin(1, titlestr.length(), 3, (50 - titlestr.length()) / 2);
  WINDOW *amentitlewin = newwin(1, winwidth, 4, 5);
  WINDOW *amenwins[10];
  int winx = 5;
  int winy = 5;
  int iwin;
  
  for (iwin = 0; iwin < page_size; iwin++) {
    amenwins[iwin] = newwin(3, winwidth, winy, winx);
    winy += 3;
  }
  WINDOW *prevnext = newwin(1, 50, winy, 1);
  WINDOW *selectwin = newwin(1, 50, winy + 3, 1);
  WINDOW *pricewin = newwin(4, winwidth, winy + 4, winx);
  WINDOW *confirmwin = newwin(1, confirmstr.length(), winy + 8, (50 - confirmstr.length()) / 2);
  
  DSdemo<DSdemo<Amenity>> pages;
  Result<DSdemo<Amenity>> amensublist; 
  amensublist = allAmenitiesDup.sublist(0, page_size);
  for (int i = 0; amensublist.status; i++) {
    pages.enqueue_add(amensublist.value);
    amensublist = allAmenitiesDup.sublist((i + 1) * page_size, (i + 1) * page_size + page_size);
  }


  dll_node<DSdemo<Amenity>> *pnode = pages.nodeAtIndex(0); // SEGMENTATION FAULT!!
  bool added = false;
  bool undoed = false;
  double price = 0.00;
  DSdemo<Amenity> current;
  char input;
  Result<Amenity> amenres;
  int loc = 0;
  int amenindex = 0;
  wattron(selectwin, A_BOLD);
  wattron(prevnext, A_BOLD);
  wattron(prevnext, A_UNDERLINE);

  while (true) {
    clear();
    refresh();
    wclear(pricewin);
    wclear(backwin);
    wclear(titlewin);
    wclear(amentitlewin);
    wclear(confirmwin);
    wclear(selectwin);
    for (int i = 0; i < page_size; i++) {
      wclear(amenwins[i]);
      wrefresh(amenwins[i]);
    }

    wattron(backwin, A_BOLD);
    wattron(backwin, A_UNDERLINE);
    wmove(backwin, 0, 0);
    wprintw(backwin, "%s", backstr.c_str());
    wmove(titlewin, 0, 0);
    wprintw(titlewin, "%s", titlestr.c_str());
    wmove(amentitlewin, 0, 0);
    wprintw(amentitlewin, "%s", namestr.c_str());
    wmove(amentitlewin, 0, winwidth - 6);
    wprintw(amentitlewin, "%s", pstr.c_str());
    wmove(confirmwin, 0, 0);
    wattron(confirmwin, A_BOLD);
    wattron(confirmwin, A_UNDERLINE);
    wprintw(confirmwin, "%s", confirmstr.c_str());
    if (added || undoed) {
      added = false;
      undoed = false;
      wmove(selectwin, 0, (50 - changedstr.length()) / 2);
      wprintw(selectwin, "%s", changedstr.c_str());
      if (allAmenitiesDup.isEmpty()) {
        pnode = NULL;
      } else {
        pages.trash();
        amensublist = allAmenitiesDup.sublist(0, page_size);
        for (int i = 0; amensublist.status; i++) {
          pages.enqueue_add(amensublist.value);
          amensublist = allAmenitiesDup.sublist((i + 1) * page_size, (i + 1) * page_size + page_size);
        }
        pnode = pages.nodeAtIndex(0);
      }
    }



    box(pricewin, 0, 0);
    wmove(pricewin, 1, 1);
    wprintw(pricewin, "%s", dots.c_str());
    wmove(pricewin, 1, 1);
    wprintw(pricewin, "Total Price");
    wmove(pricewin, 1, winwidth - 6);
    wprintw(pricewin, "%.2f", price);
    if (!pickedAmenities.isEmpty()) {
      wmove(pricewin, 2, (winwidth - undostr.length()) / 2);
      wattron(pricewin, A_BOLD);
      wattron(pricewin, A_UNDERLINE);
      wprintw(pricewin, "%s", undostr.c_str());
      wattroff(pricewin, A_BOLD);
      wattroff(pricewin, A_UNDERLINE);
    }

    if (!pnode) {
      attron(A_BOLD);
      move(winy - 10,  winx + (winwidth - msg.length()) / 2);
      printw("%s", msg.c_str());
      attroff(A_BOLD);
    } else {

      current = pnode->data;
      amenres = current.index(0);
      loc = 0;
      for (int i = 0; amenres.status; i++) {
        Amenity a = amenres.value;
        box(amenwins[i], 0, 0);
        wmove(amenwins[i], 0, 1);
        wprintw(amenwins[i], "[ %d ]", i + 1);
        wmove(amenwins[i], 1, 1);
        wprintw(amenwins[i], "%s", dots.c_str());
        wattron(amenwins[i], A_BOLD);
        wmove(amenwins[i], 1, 1);
        wprintw(amenwins[i], "%s", a.name.c_str());
        wmove(amenwins[i], 1, winwidth - 6);
        wprintw(amenwins[i], "%.2f", a.price);
        wattroff(amenwins[i], A_BOLD);

        wrefresh(amenwins[i]);
        amenres = current.index(i + 1);
      }

      if (pnode->prev) {
        wmove(prevnext, 0, (50 / 2 - prevstr.length()) / 2);
        wprintw(prevnext, "%s", prevstr.c_str());
      }
      if (pnode->next) {
        wmove(prevnext, 0, (50 - nextstr.length()) / 2 + 50 / 4);
        wprintw(prevnext, "%s", nextstr.c_str());
      }
    }


    wrefresh(prevnext);
    wrefresh(pricewin);
    wrefresh(selectwin);
    wrefresh(backwin);
    wrefresh(amentitlewin);
    wrefresh(titlewin);
    wrefresh(confirmwin);
    refresh();

    while (true) {
      input = tolower(getch());
      if (input == 'r') {
        returnres.value = pickedAmenities;
        returnres.status = 0;
        return returnres;
      }
      if (input == 'c') {
        returnres.value = pickedAmenities;
        return returnres;
      }
      if (input == 'u') {
        if (!pickedAmenities.isEmpty()) {
          undoed = true;
          break;
        }
      }
      if (pnode) {
      if (input == 'p') {
        if (pnode->prev) {
          pnode = pnode->prev;
          break;
        }
      }
      if (input == 'n') {
        if (pnode->next) {
          pnode = pnode->next;
          break;
        }
      }
        for (int i = 49; i < current.size() + 49; i++) {
          if (input == i) {
            added = true;
            amenindex = i - 49;
            break;
          }
        }
      }
      if (added || undoed) {
        break;
      }
    }
    if (added) {
      Amenity amenity = current.index(amenindex).value;
      pickedAmenities.push(amenity);
      allAmenitiesDup.removeById(amenity.id);
      price += amenity.price;
      changedstr = "Added " + amenity.name;
    }
    if (undoed) {
      Amenity amenity = pickedAmenities.dequeue_pop().value;
      allAmenitiesDup.push(amenity);
      price -= amenity.price;
      changedstr = "Removed " + amenity.name;
    }
  }

  return returnres;
  
}

bool confirmBooking(History history, Room room, DSdemo<Room> allRooms, DSdemo<Amenity> amenities) {
  clear();
  refresh();
  string titlestr = "Are you sure you want to book " + room.name + "?";
  string yesstr = "[Y]es, book the room.";
  string nostr = "[N]o, return to rooms list.";
  string typestr = "Type";
  string bedstr = "Bed";
  string basestr = "Base Price";
  string persecstr = "Price Per Second";
  string amenstr = "[A]menities";
  string features = "[F]eatures";
  int winwidth = 50;
  int winheight = 11;
  WINDOW* titlewin = newwin(1, titlestr.length(), 2, (50 - titlestr.length()) / 2);
  WINDOW *mainwin = newwin(winheight, winwidth, 3, 1);
  WINDOW *confirmwin = newwin(4, winwidth, 3 + winheight, 1);
  int mode = 0;
  wmove(titlewin, 0, 0);
  wprintw(titlewin, "%s", titlestr.c_str());
  wattron(confirmwin, A_BOLD);
  wattron(confirmwin, A_UNDERLINE);
  wmove(confirmwin, 0, (winwidth - yesstr.length()) / 2);
  wprintw(confirmwin, "%s", yesstr.c_str());
  wmove(confirmwin, 1, (winwidth - nostr.length()) / 2);
  wprintw(confirmwin, "%s", nostr.c_str());
  wrefresh(titlewin);
  wrefresh(confirmwin);
  char input;
  while (true) {
    refresh();
    wclear(mainwin);
    if (mode == 0) {
      wmove(mainwin, 2, (winwidth / 2 - typestr.length()) / 2);
      wprintw(mainwin, "%s", typestr.c_str());
      wmove(mainwin, 3, (winwidth / 2 - room.type.length()) / 2);
      wattron(mainwin, A_BOLD);
      wprintw(mainwin, "%s", room.type.c_str());
      wattroff(mainwin, A_BOLD);

      wmove(mainwin, 2, (winwidth - bedstr.length()) / 2 + winwidth / 4);
      wprintw(mainwin, "%s", bedstr.c_str());
      wmove(mainwin, 3, (winwidth - room.bed.length()) / 2 + winwidth / 4);
      wattron(mainwin, A_BOLD);
      wprintw(mainwin, "%s", room.bed.c_str());
      wattroff(mainwin, A_BOLD);

      wmove(mainwin, 4, (winwidth - basestr.length()) / 2);
      wprintw(mainwin, "%s", basestr.c_str());
      wmove(mainwin, 5, (winwidth - 4) / 2);
      wattron(mainwin, A_BOLD);
      if (room.bed == "Single") {
        wprintw(mainwin, "%d", SINGLE_PRICE);
      } else if (room.bed == "Double") {
        wprintw(mainwin, "%d", DOUBLE_PRICE);
      } else {
        wprintw(mainwin, "%d", KING_PRICE);
      }
      wattroff(mainwin, A_BOLD);

      wmove(mainwin, 6, (winwidth - persecstr.length()) / 2);
      wprintw(mainwin, "%s", persecstr.c_str());
      wmove(mainwin, 7, (winwidth - 4) / 2);
      wattron(mainwin, A_BOLD);
      if (room.type == "Basic") {
        wprintw(mainwin, "%.2f", BASIC_PER_SEC);
      } else if (room.type == "Deluxe") {
        wprintw(mainwin, "%.2f", DELUXE_PER_SEC);
      } else {
        wprintw(mainwin, "%.2f", SUITE_PER_SEC);
      }
      wattroff(mainwin, A_BOLD);

      wattron(mainwin, A_BOLD);
      wattron(mainwin, A_UNDERLINE);
      wmove(mainwin, 9, (winwidth / 2 - amenstr.length()) / 2);
      wprintw(mainwin, "%s", amenstr.c_str());
      wmove(mainwin, 9, (winwidth - features.length()) / 2 + winwidth / 4);
      wprintw(mainwin, "%s", features.c_str());
      wattroff(mainwin, A_BOLD);
      wattroff(mainwin, A_UNDERLINE);
    }
    if (mode == 1) {
      showAmenityDetails(mainwin, winwidth, winheight, history.amenityIds, amenities);
    }
    if (mode == 2) {
      showFeatures(mainwin, winwidth, winheight, room.id, allRooms);
    }
    box(mainwin, 0, 0);
    wrefresh(mainwin);
    while (true) {
      input = tolower(getch());
      if (input == 'y') {
        return true;
      }
      if (input == 'n') {
        return false;
      }
      if (input == 'd' && mode) {
        mode = 0;
        break;
      }
      if (input == 'f' && !mode) {
        mode = 2;
        break;
      }
      if (input == 'a' && !mode) {
        mode = 1;
        break;
      }
    }
  }

    
}
