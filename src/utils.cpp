#include "../headers/header.hpp"

void showFeatures(Room room, WINDOW *win) {
  box(win, 0, 0);
  wmove(win, 0, 1);
  wprintw(win, "Features of %s", room.name.c_str());
  DSdemo<const char *> features = getFeatures(room.type).value;
}

Result<string> menuNums(DSdemo<string> items) {
  Result<string> res = items.index(0);
  int items_size = items.size();
  for (int i = 0; res.status; i++) {
    move(3 + i, 3);
    printw("[ %d ] %s", i + 1, res.value.c_str());
    res = items.index(i + 1);
  }
  char sel = getchar();
  while ((sel < 49) || (sel > (48 + items_size))) {
    sel = getchar();
  }
  int selection = sel - 48;

  return items.index(selection - 1);
}

Result<string> menuLetters(string items[], int count) {
  string letters = "";
  string current;
  int i = 0;
  int letter_pos = INT32_MAX;
  char c;
  move(2, 3);
  for (int i = 0; i < count; i++) {
    move(2 + i, 3);
    current = items[i];
    for (int j = 0; j < current.length(); j++) {
      if (letters.find(current[j]) == string::npos) {
        letter_pos = j;
        break;
      }
    }
    if (letter_pos == INT32_MAX) {
      c = '0';
      while (letters.find(c)) {
        c++;
      }
      printw("[%c]%s", c, current.c_str());
      letters += c;
    } else {
      c = current[letter_pos];
      printw("%s[%c]%s", current.substr(0, letter_pos).c_str(), c, current.substr(letter_pos + 1).c_str());
      letters += c;
    }
    letter_pos = INT32_MAX;
  }
  bool input = true;
  while (input) {
    char sel = getch();
    for (int i = 0; i < letters.size(); i++) {
      if (sel == tolower(letters[i]) || sel == toupper(letters[i])) {
        current = items[i];
        input = false;
        break;
      }
    }
  }
  Result<string> res = {current, 1};
  return res;
}

std::string getstring(WINDOW* win)
{
    std::string input;

    // let the terminal do the line editing
    nocbreak();
    echo();

    // this reads from buffer after <ENTER>, not "raw"
    // so any backspacing etc. has already been taken care of
    int ch = wgetch(win);

    while ( ch != '\n' )
    {
        input.push_back( ch );
        ch = wgetch(win);
    }

    cbreak();
    noecho();
    // restore your cbreak / echo settings here

    return input;
}

bool confirmPage(string text) {
  clear();
  refresh();
  string msg = "Are you sure you want to " + text + "?";
  WINDOW *win = newwin(4, msg.length() + 4, 2, 2);
  box(win, 0, 0);
  wmove(win, 1, 1);
  wprintw(win, "%s", msg.c_str());
  wmove(win, 2, (msg.length() - 14)/2);
  wattron(win, A_BOLD);
  wprintw(win, "[Y]es         [N]o");
  wattroff(win, A_BOLD);
  wrefresh(win);
  char input;
  while (true) {
    input = getch();
    if ((input == 'y') || (input == 'Y')) {
      return true;
    } else if ((input == 'n') || (input == 'N')) {
      return false;
    }
  }
}

void showHistoryDetails(WINDOW* win, int winwidth, int winheight, History history, DSdemo<Room> allRooms, DSdemo<Amenity> allAmenities) {
  string namestr = "Name";
  string typestr = "Type";
  string bedstr = "Bed";
  string time_spentstr = "Time spent so far: ";
  string pricestr = "Price so far: ";
  string time_bookedstr = "Booking time: ";
  string amenstr = "[A]menities";
  string featstr = "[F]eatures";

  double time_spent = difftime(time(0), history.time_booked);
  double price = getHistoryPrice(history, allRooms, allAmenities).value;

  if (history.booked_out) {
    time_spent = difftime(history.time_left, history.time_booked);
    time_spentstr = "Time spent: ";
    pricestr = "Price paid: ";
  }


  Room room = getHistoryRoom(history, allRooms).value;

  wmove(win, 2, (winwidth - typestr.length()) / 2);
  wprintw(win, "%s", typestr.c_str());
  wmove(win, 3, (winwidth - room.type.length()) / 2);
  wattron(win, A_BOLD);
  wprintw(win, "%s", room.type.c_str());
  wattroff(win, A_BOLD);

  wmove(win, 2, (winwidth / 2 - namestr.length()) / 2);
  wprintw(win, "%s", namestr.c_str());
  wmove(win, 3, (winwidth / 2 - room.name.length()) / 2);
  wattron(win, A_BOLD);
  wprintw(win, "%s", room.name.c_str());
  wattroff(win, A_BOLD);

  wmove(win, 2, (winwidth - bedstr.length()) / 2 + winwidth / 4);
  wprintw(win, "%s", bedstr.c_str());
  wmove(win, 3, (winwidth - room.bed.length()) / 2 + winwidth / 4);
  wattron(win, A_BOLD);
  wprintw(win, "%s", room.bed.c_str());
  wattroff(win, A_BOLD);

  wmove(win, 5, 3);
  wprintw(win, "%s", time_bookedstr.c_str());
  wattron(win, A_BOLD);
  wprintw(win, "%s", ctime(&history.time_booked));
  wattroff(win, A_BOLD);

  wmove(win, 6, 3);
  wprintw(win, "%s", time_spentstr.c_str());
  wattron(win, A_BOLD);
  int days = (int)(time_spent / 86400);
  int hours = (int)(((int)time_spent % 86400) / 3600);
  int minutes = (((int)time_spent % 86400) % 3600) / 60;
  int seconds = ((((int)time_spent % 86400) % 3600) % 60) % 60;
  if (days) {
    wprintw(win, "%dD ", days);
  }
  if (hours) {
    wprintw(win, "%dH ", hours);
  }
  if (minutes) {
    wprintw(win, "%dM ", minutes);
  }
  wprintw(win, "%ds ", seconds);
  wattroff(win, A_BOLD);

  wmove(win, 7, 3);
  wprintw(win, "%s", pricestr.c_str());
  wattron(win, A_BOLD);
  wprintw(win, "%.2f", price);
  wattroff(win, A_BOLD);

  wattron(win, A_UNDERLINE);
  wattron(win, A_BOLD);
  wmove(win, 9, (winwidth / 2 - amenstr.length()) / 2);
  wprintw(win, "%s", amenstr.c_str());
  wmove(win, 9, (winwidth - amenstr.length()) / 2 + winwidth / 4);
  wprintw(win, "%s", featstr.c_str());
  wattroff(win, A_UNDERLINE);
  wattroff(win, A_BOLD);
}

void showAmenityDetails(WINDOW* win, int winwidth, int winheight, int amenities[], DSdemo<Amenity> allAmenities) {
  string empty = "No amenities chosen";
  string amentitle = "Amenities";
  string pricetitle = "Price";
  double price = 0;

  if (amenities[0] == 0) {
    wmove(win, winheight / 2, (winwidth - empty.length()) / 2);
    wprintw(win, "%s", empty.c_str());
    return;
  }
  int pricewidth = (winwidth - pricetitle.length()) / 2 + winwidth / 4;
  wattron(win, A_STANDOUT);
  wmove(win, 2, (winwidth / 2 - amentitle.length()) / 2);
  wprintw(win, "%s", amentitle.c_str());
  wmove(win, 2, (winwidth - pricetitle.length()) / 2 + winwidth / 4);
  wprintw(win, "%s", pricetitle.c_str());
  wattroff(win, A_STANDOUT);

  dll_node<Amenity> *anode;
  for(int i = 0; i < MAX_AMENITIES; i++) {
    if (amenities[i] == 0) {
      break;
    }
    anode = allAmenities.search(amenities[i]);
    if (!anode) {
      continue;
    }
    wmove(win, 3 + i, winwidth / 4);
    for (int i = 0; i < winwidth / 2; i++) {
      waddch(win, '-');
    }
    wmove(win, 3 + i, (winwidth / 2 - anode->data.name.length()) / 2);
    wprintw(win, "%s", anode->data.name.c_str());

    wmove(win, 3 + i, pricewidth);
    wprintw(win, "%.2f", anode->data.price);
    price += anode->data.price;
  }

  string pricestr = "Total";
  wmove(win, winheight - 3, winwidth / 4);
  for (int i = 0; i < winwidth / 2; i++) {
    waddch(win, '-');
  }
  wmove(win, winheight - 3, (winwidth / 2 - pricestr.length()) / 2);
  wprintw(win, "%s", pricestr.c_str());
  wmove(win, winheight - 3, pricewidth);
  wprintw(win, "%.2f", price);
  string back = "Back to [D]etails";
  wmove(win, winheight - 2, (winwidth - back.length()) / 2);
  wprintw(win, "%s", back.c_str());
}

void showFeatures(WINDOW* win, int winwidth, int winheight, int roomId, DSdemo<Room> allRooms) {
  dll_node<Room> *rnode = allRooms.search(roomId);
  if (!rnode) {
    wmove(win, 1, 1);
    wprintw(win, "Room ID: %d", roomId);
    getch();
    return;
  }
  Room r = rnode->data;

  wattron(win, A_BOLD);
  string title = "Room Features";
  wmove(win, 1, (winwidth - title.length()) / 2);
  wprintw(win, "%s", title.c_str());
  wattroff(win, A_BOLD);

  DSdemo<const char *> features = getFeatures(r.type).value;
  Result<const char*> feat = features.index(0);
  for (int i = 0; feat.status; i++) {
    wmove(win, 2 + i, 3);
    wprintw(win, "* %s", feat.value);
    feat = features.index(i + 1);
  }
  string back = "Back to [D]etails";
  wmove(win, winheight - 2, (winwidth - back.length()) / 2);
  wprintw(win, "%s", back.c_str());
}
