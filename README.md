## Hotel Room Booker in C++ using the ncurses library

# !! Important
#### Some execution paths may fail because there are memory leaks in certain places that I haven't been able to figure out. All functions and menus work perfectly by themselves but some of them lead to memory leaks that might lead other parts to cause segmentation faults
 - **Example: After going to the pick amenities page, if you return back to the main menu in some cases you can not enter the rooms list menu again since it causes a segmentation fault.**
- **If the program crashes at any point, restart it and go directly to the place you want to go and it should work fine as expected.**


## Usage

### Currently the program only functions as expected on a linux operating system. It can be compiled on Windows but it has numerous unexpected behaviours so I've decided to not include compilation instructions and necessary libraries for the OS. You have to link the **PDCurses** library to compile the program on windows.

### For the Linux operating system:
- Requirement: **ncurses** package
    * Installation:
    * On Ubuntu/Debian
  ```
  sudo apt-get install libncurses5-dev libncursesw5-dev
  ```
    * On Arch linux
  ```
  sudo pacman -S ncurses
  ```
- To compile and run:
```
g++ src/*.cpp -o hotel -lncurses
./hotel
```

## Description
- This is a hotel room booker demo program built entirely in cpp. It features a single class called **DSdemo**, which is the highlight of the project.
DSdemo can be used as a singly linked list, a doubly linked list, a stack, a queue or any other derivatives. It features a head and tail pointer to a **dll_node** structure and a total of 15 functions listed below:
- nodeAtIndex - returns a pointer to a node at an index
- size - returns the size of the list/stack/queue
- isEmpty - returns true if the list/stack/queue is empty
- trash - deletes all nodes of the list/stack/queue
- remove - deletes the last node and returns the value
- index - returns the value of a node at an index
- addAtIndex - adds a node at an index
- removeAtIndex - removes a node at an index and returns its value
- removeById - Assumes the node data of the list has an object with the attribute *id* and removes a node associated with a certain id

- enqueue_add - functions as either a queue enqueue function or a list add function by adding an element at the end of the list/queue
- dequeue_pop - functions as either a queue dequeue function or a stack pop function by removing the first/top element in a queue/stack
- push - removes the first element of the list/queue/stack

## Features
- Things you can do in the program
   * Sign up as a user
   * View your profile
   * Edit your profilee
   * View your previous history
   * View all available rooms
   * View room details
   * Pick amenities for your stay
   * Book a room
   * View your current booking status
   * Book out of a room
- Not necessary to log back in after restart
- The code was written to be very extensible with window width and page size wherever necessary.
- It can also be easily expanded on to add an admin user capable of viewing booked rooms, adding editing and deleting rooms, adding editing or removing amenities as well as adjusting the prices of room types.

## Unique Features
- Special Result structure that contains a value and a status, used as a function return type. It contains a value attribute containing the return value of a function and a status attribute to indicate if the function failed or not. Useful for functions returning object types since objects can't be assigned NULL to indicate failure.
- A state enum used as a return type for some functions to indicate a return status. Can either be *logged_in*, *logged_out*, *booked_in*, *booked_out* or *back*.

## Data Structure Operations in a nutshell
- Doubly Linked lists (Singly linked lists) - For basic storage
- Stack - For picking and undoing amenities
- Queue - For storing available rooms to view rooms that were not occupied for the longest first
- Doubly linked list of doubly linked lists - To utilize pages in viewing rooms and picking amenities

## Menus
- Front page
- Login page
- Sign up page
- Landing page (With a Room and Without a Room)
- Profile page
- Edit profile page
- History page
- Rooms page
- Amenities page
- Room details page
- Confirm booking page

## Classes
- User
   * Username, Password, Name, Creation Time, Last Login, Logged In
- Room
  - Type:
	  - Basic
	  - Deluxe
	  - Suite
  - Bed:
	   - Single
	   - Double
	   - King-Size
   - Name, User ID

- Amenity
	- Name, Price

- History
	- User ID, Room ID, Amenity IDs, booked_out, time_booked, time_left

## File Descriptions
- main.cpp - contains the main function and add sample items functions for first time execution
- file_ops.cpp - contains functions to read from and write into files
- object_ops.cpp - contains functions for manipulating objects
- menus - contains all the menus used in the program
- utils.cpp - extra utility functions

## Storage Files
- amenities - stores amenities
- available_rooms - stores available rooms
- booked_rooms - stores booked rooms
- users - stores user data
- history - stores all past and present bookings (essentially logs)



## This project is also a very good demo of the ncurses library for making console applications so I highly recommend you try it out, either to see what it's capable of or to tinker around and make improvements

## Any recommendations for fixing the segmentation faults would be very appreciated (commented in the code)
