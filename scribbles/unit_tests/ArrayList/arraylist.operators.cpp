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
  This operator allows you to treat the arraylist like an array (hence the reason it is called an arraylist) even though
  internally it is a doubly linked list. An arraylist can be slower than a conventional array, however, the seek method
  has some tweaks and such to get you to the data you want to be at much faster than starting at the beginning of the
  list every time you call on this operator.

  @method datatype &operator[] (int index);
    @int index - The position of the item you want to fetch.
  returns datatype

  See also: next, prev, seek
*/
template <class datatype>
datatype &arraylist<datatype>::operator [] (int index)
{
  // Seek returns false if you are out of range...
  if(!seek(index))
  {
    cerr << endl << "Illegal index: " << int(index) << ", Max index: " << (items - 1) << endl;
    system("Pause");
    exit(1);
  }

  return current->value;
}

template <class datatype>
const datatype &arraylist<datatype>::operator [] (int index) const
{
  // Seek returns false if you are out of range...
  if(!seek(index))
  {
    cerr << endl << "Illegal index: " << int(index) << ", Max index: " << (items - 1) << endl;
    system("Pause");
    exit(1);
  }

  return current->value;
}

/*
  This operator allows an arraylist to be set to the values of another arraylist, the difference between
  this operator and the usual = operator a class would have is that both arraylist's linked lists won't be
  in the same memory spot, allowing you to modify one list (delete, remove items, update, etc) without affecting
  the other arraylist.

  const arraylist<datatype> &operator = (const arraylist<datatype> &assign);

  Example:
    arraylist<int> list1, list2;
    list1.add(1);
    list1.add(2);
    list2 = list1; // list2 now contains ONLY the same values as list1, but different memory locations.
*/
template <class datatype>
const arraylist<datatype> &arraylist<datatype>::operator = (const arraylist<datatype> &assign)
{
  // Empty out our current list, we won't need that anymore.
  empty();

  // Now copy the data over...
  arraylistnode<datatype> *dummy = assign.top;

  while(dummy != NULL)
  {
    add(dummy->value);
    dummy = dummy->next;
  }

  return *this;
}

/*
  Adds one arraylist with another arraylist and it will return a new arraylist containing their combined values.

  arraylist<datatype> operator + (const arraylist<datatype> &assign);

  Example:
    arraylist<int> list1, list2, list3;
    list1.add(1);
    list2.add(10);
    list3 = list1 + list2; // list3 now contains 1 and 10, respectively.
*/
template <class datatype>
arraylist<datatype> arraylist<datatype>::operator + (const arraylist<datatype> &assign)
{
  arraylist<datatype> add;

  // Copy this arraylist's contents over.
  arraylistnode<datatype> *dummy = top;
  while(dummy != NULL)
  {
    add.add(dummy->value);
    dummy = dummy->next;
  }

  // Now the passed arraylist's contents...
  dummy = assign.top;
  while(dummy != NULL)
  {
    add.add(dummy->value);
    dummy = dummy->next;
  }

  return add;
}

/*
  Adds the arraylist on the right hand side of the operator to the left hand side arraylist.

  const arraylist<datatype> &operator += (const arraylist<datatype> &assign);
*/
template <class datatype>
const arraylist<datatype> &arraylist<datatype>::operator += (const arraylist<datatype> &assign)
{
  // Use our copy method...
  copy(assign);

  return *this;
}

/*
  Removes all instances of assign. For example, if you have an arraylist of int's containing 1, 2, 3, 2, 5 and you minus
  2, the arraylist afterwards will be 1, 3, 5

  arraylist<datatype> operator - (const datatype &assign);
*/
template <class datatype>
arraylist<datatype> arraylist<datatype>::operator - (const datatype &assign)
{
  arraylist<datatype> minus;

  arraylistnode<datatype> *dummy = top;
  while(dummy != NULL)
  {
    if(dummy->value != assign)
      minus.add(dummy->value);

    dummy = dummy->next;
  }

  return minus;
}

/*
  Removes all instances of assign in the arraylist, much like the - operator.

  const arraylist<datatype> &arraylist<datatype>::operator -= (const datatype &assign);
*/
template <class datatype>
const arraylist<datatype> &arraylist<datatype>::operator -= (const datatype &assign)
{
  arraylistnode<datatype> *dummy = top;
  arraylistnode<datatype> *next_dummy;
  int index = 0;
  while(dummy != NULL)
  {
    next_dummy = dummy->next;

    if(dummy->value == assign)
    {
      remove(index);
    }
    else
      index++;

    dummy = next_dummy;
  }

  return *this;
}

/*
  Example:
  arraylist<int> a, b;
  a.add(1);
  a.add(100);

  b = a * 3; // b contains 1, 100, 1, 100, 1, 100 (a 3 times)

  arraylist<datatype> operator * (const int &assign);
*/
template <class datatype>
arraylist<datatype> arraylist<datatype>::operator * (const int &assign)
{
  arraylist<datatype> multiple;

  arraylistnode<datatype> *dummy;
  for(int i = 0; i < assign; i++)
  {
    dummy = top;
    while(dummy != NULL)
    {
      multiple.add(dummy->value);
      dummy = dummy->next;
    }
  }

  return multiple;
}

/*
  Same operator, just modifies the current arraylist.

  const arraylist<datatype> &operator *= (const int &assign);
*/
template <class datatype>
const arraylist<datatype> &arraylist<datatype>::operator *= (const int &assign)
{
  if(assign == 0)
    empty();
  else
  {
    int copy_length = items, copied;
    arraylistnode<datatype> *dummy;

    for(int i = 1; i < assign; i++)
    {
      dummy = top;
      copied = 0;

      while(dummy != NULL && copied < copy_length)
      {
        add(dummy->value);
        dummy = dummy->next;
        copied++;
      }
    }
  }

  return *this;
}

/*
  Division operator which takes the length and divides it by the given integer and truncates the arraylist
  to the new length.

  Example:
    arraylist<int> items, items2;
    items.add(6);
    items.add(4);
    items.add(10);
    items.add(5);

    items2 = items / 2; // items2 now contains 6 and 4

  arraylist<datatype> operator / (const int &assign);
*/
template <class datatype>
arraylist<datatype> arraylist<datatype>::operator / (const int &assign)
{
  arraylist<datatype> divide;

  if(assign > 0)
  {
    arraylistnode<datatype> *dummy = top;
    int new_length = items / assign, copied = 0;

    while(dummy != NULL && copied < new_length)
    {
      divide.add(dummy->value);
      dummy = dummy->next;
    }
  }

  return divide;
}

/*
  Look at the * operator.

  const arraylist<datatype> &operator /= (const int &assign);
*/
template <class datatype>
const arraylist<datatype> &arraylist<datatype>::operator /= (const int &assign)
{
  if(assign > 0)
  {
    int cur_length = items, new_length = items / assign;

    for(int i = 0; i < (cur_length - new_length); i++)
      remove(arraylist_LAST);
  }

  return *this;
}

/*
  Checks to see if the arraylist's contain the same values. If the arraylist's do not have the same amount of items,
  FALSE is immediately returned.

  bool operator == (const arraylist<datatype> &compare);
*/
template <class datatype>
bool arraylist<datatype>::operator == (const arraylist<datatype> &compare)
{
  // Length not the same? Then they are definitely not the same.
  if(length() != compare.items)
    return false;

  arraylistnode<datatype> *dummy = top;
  arraylistnode<datatype> *dummy2 = compare.top;
  while(dummy != NULL)
  {
    if(dummy->value != dummy2->value)
      return false;

    dummy = dummy->next;
    dummy2 = dummy2->next;
  }

  return true;
}

/*
  Checks to see if the arraylist's do not contain the same values. If the arraylist's do not have the same amount of items,
  TRUE is immediately returned.

  bool operator != (const arraylist<datatype> &compare);
*/
template <class datatype>
bool arraylist<datatype>::operator != (const arraylist<datatype> &compare)
{
  // Length not the same? Then they are definitely not the same.
  if(length() != compare.items)
    return true;

  arraylistnode<datatype> *dummy = top;
  arraylistnode<datatype> *dummy2 = compare.top;
  while(dummy != NULL)
  {
    if(dummy->value != dummy2->value)
      return true;

    dummy = dummy->next;
    dummy2 = dummy2->next;
  }

  return false;
}

/*
  Checks to see which arraylist is greater (less than or, greater than or less than equal to), compares them via the number of items they contain.
*/
template <class datatype>
bool arraylist<datatype>::operator > (const arraylist<datatype> &compare)
{
  // Does this arraylist have more items than the other?
  return items > compare.items;
}

template <class datatype>
bool arraylist<datatype>::operator < (const arraylist<datatype> &compare)
{
  // The opposite of >
  return items < compare.items;
}

template <class datatype>
bool arraylist<datatype>::operator >= (const arraylist<datatype> &compare)
{
  return items >= compare.items;
}

template <class datatype>
bool arraylist<datatype>::operator <= (const arraylist<datatype> &compare)
{
  return items <= compare.items;
}