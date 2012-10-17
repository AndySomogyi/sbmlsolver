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
        - Added copy method, which copies one arraylist's data over to anothers (Does not empty the arraylist, it adds on)
          this method is now used in the copy constructor (arraylist(const arraylist<datatype> &al)) and the += operator

      Known issues:
        - You cannot pass an arraylist as a constant and then use the [] operator, or many of them, for that case.
          Reason: The [] operator uses the seek method which moves the internal pointer, and attributes of the class
                  can not be modified (Of course!)
          Suggested fix: When it is a const, it could simply make a dummy in the [] operator, and move the dummy pointer
                         until it has been moved to the correct position, but of course, could be very slow, but a fix
                         nonetheless.

*/

#ifndef __arraylist_CLASS__
#define __arraylist_CLASS__

#ifndef NULL
#define NULL ((void *)0)
#endif

#define arraylist_FIRST 0
#define arraylist_LAST -1
#define AL_FIRST arraylist_FIRST
#define AL_LAST arraylist_LAST

// We can't use the data type supplied to us without any modification,
// so we simply make a structure containing the value and the next and previous item (pointer).
template <class datatype>
struct arraylistnode
{
  arraylistnode *prev;
  datatype value;
  arraylistnode *next;
};

// The class definition of Queue
template <class datatype>
class arraylist
{
  private:
    arraylistnode<datatype> *top;
    arraylistnode<datatype> *bottom;
    arraylistnode<datatype> *current;
    int current_index;
    int items;
    bool seek(int index);
    void reset();

  public:
    arraylist();
    arraylist(const arraylist<datatype> &al);
    ~arraylist();
    void add(datatype item);
    bool insert(datatype item, int index);
    bool remove(int index);
    int length() const;
    void empty();
    void copy(const arraylist<datatype> &al);
    int indexOf(datatype search);
    int indexOf(datatype search, int start);
    int lastIndexOf(datatype search);
    int lastIndexOf(datatype search, int start);
    int trim();
    int trim(datatype empty_value);
    const datatype &operator [] (int index) const;
    datatype &operator [] (int index);
    const arraylist<datatype> &operator = (const arraylist<datatype> &assign);
    arraylist<datatype> operator + (const arraylist<datatype> &assign);
    const arraylist<datatype> &operator += (const arraylist<datatype> &assign);
    arraylist<datatype> operator - (const datatype &assign);
    const arraylist<datatype> &operator -= (const datatype &assign);
    arraylist<datatype> operator * (const int &assign);
    const arraylist<datatype> &operator *= (const int &assign);
    arraylist<datatype> operator / (const int &assign);
    const arraylist<datatype> &operator /= (const int &assign);
    bool operator == (const arraylist<datatype> &compare);
    bool operator != (const arraylist<datatype> &compare);
    bool operator > (const arraylist<datatype> &compare);
    bool operator < (const arraylist<datatype> &compare);
    bool operator >= (const arraylist<datatype> &compare);
    bool operator <= (const arraylist<datatype> &compare);
};

#include "arraylist.cpp"
#include "arraylist.operators.cpp"

#endif