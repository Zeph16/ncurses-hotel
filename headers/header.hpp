#ifndef HEAD_H
#define HEAD_H


#include "data_structure.hpp"
using namespace std;

Result<DSdemo<User>> loadUsers();
Result<DSdemo<Room>> loadRooms(bool);
Result<DSdemo<Amenity>> loadAmenities();
Result<DSdemo<History>> loadHistories();
bool saveUsers(DSdemo<User>);
bool saveRooms(DSdemo<Room>, bool);
bool saveAmenities(DSdemo<Amenity>);
bool saveHistories(DSdemo<History>);
bool addUserToFile(User user);
bool addRoomToFile(Room room, bool);
bool addAmenityToFile(Amenity amenity);
bool addHistoryToFile(History);

Result<string> menuNums(DSdemo<string> items);
Result<string> menuLetters(string[], int);
string getstring(WINDOW *win);
bool confirmPage(string text);
void showHistoryDetails(WINDOW* win, int winwidth, int winheight, History history, DSdemo<Room> allRooms, DSdemo<Amenity> allAmenities);
void showAmenityDetails(WINDOW* win, int winwidth, int winheight, int amenities[], DSdemo<Amenity> allAmenities);
void showFeatures(WINDOW* win, int winwidth, int winheight, int roomId, DSdemo<Room> allRooms);

Result<Room> getUserRoom(DSdemo<Room>, User);
void logoutUser(User u, DSdemo<User> users);
void bookoutRoom(Room room, DSdemo<History>&, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms);
Result<DSdemo<const char*>> getFeatures(string type);
Result<User> getLoggedInUser(DSdemo<User> users);
Result<DSdemo<History>> filterUserHistory(User u, DSdemo<History> history);
Result<Room> getHistoryRoom(History history, DSdemo<Room> rooms);
Result<DSdemo<Amenity>> getHistoryAmenities(History history, DSdemo<Amenity> amenities);
Result<double> getHistoryPrice(History hist, DSdemo<Room> allRooms, DSdemo<Amenity> allAmenities);
Result<DSdemo<Room>> filterRooms(DSdemo<Room> rooms, string type, string bed);

State landingPage(User currentUser, DSdemo<User> &allUsers, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms, DSdemo<Amenity> &amenities, DSdemo<History> &history);
Result<User> login_signup(DSdemo<User> &allUsers);
Result<User> login(DSdemo<User> allUsers);
Result<User> signup(DSdemo<User> &allUsers);
State HasRoomPage(User, Room, DSdemo<User> &allUsers, DSdemo<Room>&, DSdemo<Room>&, DSdemo<History> &allHistories, DSdemo<Amenity> allAmenities);
State NoRoomPage(User user, DSdemo<User> &allUsers, DSdemo<Room> &available_rooms, DSdemo<Room> &booked_rooms, DSdemo<Amenity> &amenities, DSdemo<History> &history);
void showProfile(User &u, DSdemo<User> &allUsers, DSdemo<Room> allRooms, DSdemo<Amenity> allAmenities, DSdemo<History> allHistories);
void editProfile(User &u, DSdemo<User> &allUsers);
void userHistoryMenu(User user, DSdemo<Room> allRooms, DSdemo<History> allHistories, DSdemo<Amenity> allAmenities);
State roomsMenu(User, DSdemo<Room>&, DSdemo<Room>&, DSdemo<Amenity>&, DSdemo<History>&);
State roomDetails(int roomId, DSdemo<Room> rooms);
Result<DSdemo<Amenity>> pickAmenities(DSdemo<Amenity> allAmenities);
bool confirmBooking(History history, Room room, DSdemo<Room> allRooms, DSdemo<Amenity> amenities);
#endif // !HEAD_H
