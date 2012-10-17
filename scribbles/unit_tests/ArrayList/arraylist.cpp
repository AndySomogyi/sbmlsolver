/*

    Class: arraylist
    Created: 11-04-2009
    Last Modified: 11-13-2009
    Version: 1.0
    Website: http://code.google.com/p/arraylist/
    License: GNU GPL v3 <http://www.gnu.org/licenses/gpl-3.0.txt>
    Description: Quite obviously, this is a C++ class which allows you to create queue's
                 with any data type (int, double, classes, etc.). Check out the implementation
                 file for documentation on all the methods of this class.

    THIS CLASS IS FREE TO USE FOR WHATEVER REASON YOU MAY WANT, YOU MAY EDIT, COPY,
    REDISTRIBUTE, ETC. THESE FILES. ALL I ASK IS THAT I AT LEAST GET SOME KIND OF CREDIT!!!
    THIS CLASS IS PROVIDED "AS IS" WITHOUT WARRANTY, I NOR ANY PERSON/THING/WHAT-HAVE-YOU
    WHO GIVES, DISTRIBUTES, ETC. IS NOT RESPONSIBLE FOR ANYTHING THAT MAY OR MAY NOT OCCUR
    WHILE USING THIS CLASS, USE AT YOUR OWN RISK, OR AS SOME MAY SAY "USER BEWARE".

    Changelog:
      11-04-2009
        - Initially completed. (Queue class)

      11-05-2009
        - Queue has now transformed into an arraylist.
        - seek() method added, along with QUEUE_FIRST and QUEUE_LAST.
        - QueueNode now has a newer pointer prev added.
        - prev() method added, much like next() method, but it goes to the previous item, not the next.
        - Operator [] added, use just as you would an array operator.
        - position() method added, this returns the current "index" that the current internal pointer
          is pointing to.
        - remove() method is no longer an alias of pop(), it now removes an item at the specified index.
        - insert() method added, which places an item into the specified index position.

      11-06-2009
        - Added =, +, +=, -, ==, !=, >, <, >=, <= operators.
        - Added new constructor (arraylist(const arraylist<datatype> &al))

      11-13-2009
        - Removed push method, add now does what push does.
        - Removed pop method, use remove method instead.
        - Removed front and back method, use [] operator instead.
        - Removed position method, not needed.
        - seek method now a private method, use [] operator.
        - Removed next and prev method, use [] operator instead.
        - reset method now a private method.
        - Added indexOf, lastIndexOf and trim methods.
        - Renamed arraylist.class.h to arraylist.h and arraylist.class.cpp to arraylist.cpp

      11-16-09
        - Fixed empty error, on occasion, a memory access error occurred while destructing the arraylist object.

      11-17-09
        - int length() changed to int length() const (Allows length to be called when an arraylist is passed as a const)

      Known issues:
        - You cannot pass an arraylist as a constant and then use the [] operator, or many of them, for that case.
          Reason: The [] operator uses the seek method which moves the internal pointer, and attributes of the class
                  can not be modified (Of course!)
          Suggested fix: When it is a const, it could simply make a dummy in the [] operator, and move the dummy pointer
                         until it has been moved to the correct position, but of course, could be very slow, but a fix
                         nonetheless.

*/

#ifndef __arraylist_CLASS__
#include "arraylist.h"
#endif

/*
  The default (and only) constructor of the Queue class.

  @arraylist<datatype>();
*/
template <class datatype>
arraylist<datatype>::arraylist()
{
  // Top needs to point to NULL
  top = NULL;

  // Same goes for bottom...
  bottom = NULL;

  // No items, quite yet.
  items = 0;
  current_index = 0;
}

/*
  Copy constructor for arraylist.

  @arraylist<datatype>(const arraylist<datatype> &al);
    const arraylist<datatype> &al - The arraylist you wish to copy over.
*/
template <class datatype>
arraylist<datatype>::arraylist(const arraylist<datatype> &al)
{
  // Set all that needs to be set.
  top = NULL;
  bottom = NULL;
  items = 0;
  current_index = 0;

  // Copy all the data over (not the pointers!!!)
  copy(al);
}

/*
  Called upon when the class destructs, in this case, it empties all the pointers
  and the data that they point to.

  @~arraylist<datatype>();
*/
template <class datatype>
arraylist<datatype>::~arraylist()
{
  empty();
}

/*
  Adds an item to the end of the arraylist.

  @method void add(datatype item);
    @datatype item - The item to add to the end of the queue.
  returns void

  See also: insert
*/
template <class datatype>
void arraylist<datatype>::add(datatype item)
{
  // We can't use the item supplied exactly, so we need
  // a struct to hold the value and the next item (pointer)
  arraylistnode<datatype> *dummy;
  dummy = new(arraylistnode<datatype>);

  // Now set dummys value.
  dummy->value = item;

  if(top == NULL)
  {
    // Top now needs to point to dummy and bottom needs to point to top ;)
    top = dummy;
    bottom = dummy;
    reset();
  }

  // Now we need to link the last item to the next one.
  bottom->next = dummy;

  // Point dummys last to the bottom...
  dummy->prev = bottom;

  // Bottom now needs to go to dummy, since it is now the last one.
  bottom = dummy;

  // The very last item points to nothing, for now.
  bottom->next = NULL;

  // That's one more item!!!
  items++;

  // Fixes an issue when you add something for the first time (top is set to dummy, and so is bottom,
  // so when you have dummys previous set to bottom you set the previous to itself, which is bad!)
  if(items == 1)
    top->prev = NULL;
}

/*
  Allows you to insert an item into the list at the specified index. In this case, the
  range is 0 - (number of items), as if you specify (number of items) it will be placed
  at the very end of the list. TRUE is returned if placement was a success, FALSE for failure.

  @method bool insert(datatype item, int index);
    @datatype item - The item you want to place in index's position.
    @int index
  returns bool

  See also: add
*/
template <class datatype>
bool arraylist<datatype>::insert(datatype item, int index)
{
  // Range check.
  if((index < 0 || items < index) && index != arraylist_LAST)
    return false;
  else if(index == arraylist_LAST)
  {
    // Just use the add method if you are adding to the bottom.
    add(item);

    return true;
  }

  // Make a dummy to hold the new value.
  arraylistnode<datatype> *dummy;
  dummy = new(arraylistnode<datatype>);
  dummy->value = item;

  // Move to where we want to place the item
  seek(index);

  // Move those pointers around to where they need to point to, if we can.
  if(current->prev != NULL)
    current->prev->next = dummy;

  dummy->prev = current->prev;
  current->prev = dummy;
  dummy->next = current;

  // We now have one more item in the list.
  items++;

  // Move the current pointer back, which gets us to the dummy we inserted which is
  // now in the place of where current used to be.
  current = current->prev;

  // If index is 0, we need top to point to current.
  if(index == 0)
    top = current;

  return true;
}

/*
  Returns the current number of items which are contained within the queue.

  @method int length();
  returns int
*/
template <class datatype>
int arraylist<datatype>::length() const
{
  return items;
}

/*
  Empties out the queue of all pointers and the data contained within.

  @method void empty();
  returns void

  Also see: remove
*/
template <class datatype>
void arraylist<datatype>::empty()
{
  // Get rid of everything! If anything, that is.
  if(items > 0)
  {
    arraylistnode<datatype> *next_dummy;
    while(top != NULL)
    {
      next_dummy = top->next;
      delete(top);
      top = next_dummy;
    }

    // Now set them back to NULL
    top = NULL;
    bottom = NULL;
    reset();

    items = 0;
  }
}

/*
  Copies all the data over from the arraylist passed, please note that this does NOT copy the pointers, it copies all
  the actual data from the other arraylist.

  @method void copy(const arraylist<datatype> &al);
    @const arraylist<datatype> &al - The arraylist you want to copy all the data from.
  returns void
*/
template <class datatype>
void copy(const arraylist<datatype> &al)
{
  // Make a dummy pointer which points to where the other arraylist's top pointers too
  arraylistnode<datatype> *dummy = al.top;
  while(dummy != NULL)
  {
    add(dummy->value);
    dummy = dummy->next;
  }
}

/*
  Removes an item from the arraylist at the specified index. Returns TRUE if the item was
  removed successfully, FALSE if not.

  @method bool remove(int index);
    @int index
  returns bool

  See also: add, empty
*/
template <class datatype>
bool arraylist<datatype>::remove(int index)
{
  // Valid index..?
  if((index < 0 || items <= index) && index != arraylist_LAST)
    return false;
  // If the range checking was a success, and there is only 1 item, simply erase everything.
  else if(items == 1)
  {
    empty();
    return true;
  }
  else if(index == arraylist_LAST)
    index = items - 1;

  // Move to where we want to be.
  seek(index);

  // Now here comes the confusing part.
  arraylistnode<datatype> *dummy = current;

  // Does dummy have a next? If not, we shouldn't (and can't) do this part here:
  if(dummy->next != NULL)
    dummy->next->prev = dummy->prev;

  // How about a previous?
  if(dummy->prev != NULL)
    dummy->prev->next = dummy->next;

  // Current now needs to point to dummys next.
  current = dummy->next;

  if(index == 0)
  {
    // If you removed the first item, we need to move top to the next item, which is now
    // the first item.
    top = dummy->next;
  }
  else if(index == (items - 1))
  {
    // Just like the top, here we need to move the bottom up one...
    bottom = dummy->prev;

    // Bottom is a bit different in the current fixing case, since the next item is null...
    current = dummy->prev;
    current_index--;
  }

  // Dummy is no longer needed...
  delete(dummy);

  // That is one less item...
  items--;

  return true;
}

/*
  Sets the internal current pointer back to the very beginning of the queue.

  @method void reset();
  returns void

  Also see: seek
*/
template <class datatype>
void arraylist<datatype>::reset()
{
  // Reset current to the top.
  current = top;

  // And the current "index" to 0.
  current_index = 0;
}

/*
  Moves the internal current pointer to the position you enter. Range: 0 - (number of items - 1).
  Returns if the pointer was moved successfully.

  @method bool seek(int index);
    @int index - Where the pointer should be moved to, a number or QUEUE_FIRST to move to the first item
                 or QUEUE_LAST to move to the last item of the queue.
  returns bool

  See also: reset
*/
template <class datatype>
bool arraylist<datatype>::seek(int index)
{
  // arraylist_LAST is just -1, which tells us you want the last item (items - 1)
  if(index == arraylist_LAST && items > 0)
    index = items - 1;
  // Make sure the item you want to move to even exists...
  else if(index < 0 || items <= index)
    return false;

  // If the current internal pointer has been moved out of bounds (Such as with next() or
  // last(), we will have to reset the pointer)
  if(current_index < 0 || items <= current_index)
    reset();

  // Where we need to be where we are at..?
  if(current_index == index)
    return true;
  // How about if we need to go up more..?
  else
  {
    // Let's check quickly, as it might be faster to move the pointer elsewhere.
    int difference = abs(current_index - index);
    if(index < difference)
    {
      // Faster to go from the start than your current position.
      reset();
    }
    else if((items - 1) - index < difference)
    {
      // Faster to go from the back than your current position.
      current = bottom;
      current_index = items - 1;
    }

    // Going up?
    if(index > current_index)
    {
      for(int i = current_index; i < index; i++)
        current = current->next;
    }
    // Down it is!
    else
    {
      for(int i = current_index; i > index; i--)
        current = current->prev;
    }
  }

  // Our new current "index"
  current_index = index;

  return true;
}

/*
  Allows you to search through the arraylist and find the first occurrence of that item and returns the index
  of the first appearence, -1 is returned if it was not found.

  @method int indexOf(datatype search);
    datatype search - The item you want to find in the arraylist.
  returns int

  NOTE: This will begin to search from the very first item in the arraylist, if you want to specify where to start
        searching from, check out the next indexOf method.
*/
template <class datatype>
int arraylist<datatype>::indexOf(datatype search)
{
  return indexOf(search, 0);
}

/*
  Allows you to search through the arraylist starting at the specified index. If the item you are searching for is
  found, the index is returned, if the item isn't found, -1 is returned.

  @method int indexOf(datatype search, int start);
    datatype search - The item you want to find in the arraylist.
    int start - The index at which you want to start the searching for "search"
  returns int
*/
template <class datatype>
int arraylist<datatype>::indexOf(datatype search, int start)
{
  if(start == arraylist_LAST)
    start = items - 1;

  // Make sure the start is valid...
  if(start < 0 || items <= start)
    return -1;

  arraylistnode<datatype> *dummy = top;
  int index = 0;

  while(dummy != NULL)
  {
    if(index >= start && dummy->value == search)
      return index;

    dummy = dummy->next;
    index++;
  }

  return -1;
}

/*
  Allows you to search through the arraylist starting at the end of the arraylist and working to the front. Returns the
  index at which the item was found or -1 if it wasn't found.

  @method int lastIndexOf(datatype search);
    datatype search - The item you want to find in the arraylist.
  returns int
*/
template <class datatype>
int arraylist<datatype>::lastIndexOf(datatype search)
{
  return lastIndexOf(search, arraylist_LAST);
}

/*
  Allows you to search through the arraylist starting at the end of the arraylist (or where you specified to begin) and working to the front. Returns the
  index at which the item was found or -1 if it wasn't found.

  @method int lastIndexOf(datatype search);
    datatype search - The item you want to find in the arraylist.
  returns int
*/
template <class datatype>
int arraylist<datatype>::lastIndexOf(datatype search, int start)
{
  if(start == arraylist_LAST)
    start = items - 1;

  if(start < 0 || items <= start)
    return -1;

  arraylistnode<datatype> *dummy = bottom;
  int index = items;

  while(dummy != NULL)
  {
    if(index <= start && dummy->value == search)
      return index;

    dummy = dummy->prev;
    index--;
  }

  return -1;
}

/*
  The trim method will remove all empty items in the arraylist. Using this version of the method trim it will remove
  all items which are NULL (0). This method will return the new length of the arraylist after trimming.

  @method int trim();
  returns int
*/
template <class datatype>
int arraylist<datatype>::trim()
{
  return trim(NULL);
}

/*
  Much like the trim above, however, this one allows you to specify what is considered to be an empty item.

  @method int trim(datatype empty_value);
    datatype empty_value - The value that is considered to be empty.
  returns int
*/
template <class datatype>
int arraylist<datatype>::trim(datatype empty_value)
{
  arraylistnode<datatype> *dummy = top;
  arraylistnode<datatype> *dummy_next;
  int index = 0;

  while(dummy != NULL)
  {
    dummy_next = dummy->next;

    if(dummy->value == empty_value)
      remove(index);
    else
      index++;

    dummy = dummy_next;
  }

  return items;
}