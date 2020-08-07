/**
 * @file    List.h
 * @brief   Simple, generic list utility class.
 * @author  Ben Bornstein
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *  
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA 
 *  
 * Copyright (C) 2002-2005 jointly by the following organizations: 
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution and
 * also available online as http://sbml.org/software/libsbml/license.html
 * ------------------------------------------------------------------------ -->
 *
 * @class List
 * @sbmlbrief{core} Simple, plain, generic lists.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * This class implements basic vanilla lists for libSBML.  It was developed
 * in the time before libSBML was converted to C++ and used C++ STL library
 * classes more extensively.  At some point in the future, this List class
 * may be removed in favor of using standard C++ classes.
 *
 * This class is distinct from ListOf because the latter is derived from
 * the SBML SBase class, whereas this List class is not.  ListOf can only
 * be used when a list is actually intended to implement an SBML ListOfX
 * class.  This is why libSBML has both a List and a ListOf.
 */

#ifndef List_h
#define List_h


#include <sbml/common/extern.h>
#include <string.h>

LIBSBML_CPP_NAMESPACE_BEGIN

/**
 * A @sbmlconstant{ListItemComparator,} is a typedef for a pointer to a function that compares two list
 * items.  The return value semantics are the same as for the C library
 * function @c strcmp:
 * <ul>
 * <li> -1: @p item1 <  @p item2
 * <li> 0:  @p item1 == @p item2
 * <li> 1:  @p item1 >  @p item2
 * </ul>
 * @see List_find()
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
typedef int (*ListItemComparator) (const void *item1, const void *item2);

/**
 * A @sbmlconstant{ListItemPredicate,} is a typedef for a pointer to a function that takes a List item and
 * returns @c 1 (true) or @c 0 (false).
 *
 * @see List_countIf()
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
typedef int (*ListItemPredicate) (const void *item);

/** @cond doxygenLibsbmlInternal */
/**
 * A @sbmlconstant{ListDeleteItemFunc,} is a typedef for a pointer to a function that takes a List item and
 * deletes / frees it as apropriate. 
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
typedef void (*ListDeleteItemFunc) (void *item);
/** @endcond */

#ifdef __cplusplus

class List;
class ListNode;
class ListIterator;

#ifndef SWIG



/**
 * @cond doxygenLibsbmlInternal
 *
 * 
 * @class ListNode
 * @sbmlbrief{core} A node in a plain List.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * This class implements the children of the List class, storing the 
 * item itself, and a pointer to the next item in the list.
 */
class LIBSBML_EXTERN ListNode
{
public:
  ListNode (void* x): item(x), next(NULL) { }

  void*      item;
  ListNode*  next;


  friend class List;
  friend class ListIterator;

  void * getItem() { return item;} ;
};

class LIBSBML_EXTERN ListIterator
{
	/* Helper class to provide pointer like facilities around a node */
	friend class List;

  ListNode* position; //The node oriented with this instance of ListIterator.
	
	ListIterator(ListNode* _pNode) : position(_pNode) {}

public:
	
  void operator++(){ position = position->next; }
	
  void operator++(int){ position = position->next; }

	bool operator!=(ListIterator rval){ return !(position == rval.position); }
	
  bool operator==(ListIterator rval){ return (position == rval.position); }

	void * operator*(){	return position->getItem(); }

};
/** @endcond */


#endif  /* !SWIG */


class LIBSBML_EXTERN List
{
public:

  /**
   * Creates a new List object.
   */
  List ();


  /**
   * Destroys this List object.
   *
   * This function does not delete List items.  It destroys only the List
   * and its constituent ListNodes (if any).
   *
   * Presumably, you either i) have pointers to the individual list items
   * elsewhere in your program and you want to keep them around for awhile
   * longer or ii) the list has no items (<code>List.size() == 0</code>).
   * If neither are true, try the macro List_freeItems() instead.
   */
  virtual ~List ();


  /**
   * Adds @p item to the end of this List.
   *
   * @param item a pointer to the item to be added.
   */
  void add (void *item);


  /**
   * Return the count of items in this list satisfying a given predicate
   * function.
   *
   * The typedef for @sbmlconstant{ListItemPredicate,} is:
   * @code
   *   int (*ListItemPredicate) (const void *item);
   * @endcode
   * where a return value of @c 1 represents true and @c 0 represents
   * false.
   *
   * @param predicate the function applied to each item in this list.
   * 
   * @return the number of items in this List for which
   * <code>predicate(item)</code> returns @c 1 (true).
   */
  unsigned int countIf (ListItemPredicate  predicate) const;


  /**
   * Find the first occurrence of an item in a list according to a given
   * comparator function.
   *
   * The typedef for @sbmlconstant{ListItemComparator,} is:
   * @code
   *   int (*ListItemComparator) (void *item1, void *item2);
   * @endcode
   * The return value semantics are the same as for the C library function
   * @c strcmp:
   * <ul>
   * <li> -1: @p item1 <  @p item2
   * <li> 0:  @p item1 == @p item2
   * <li> 1:  @p item1 >  @p item2
   * </ul>
   * 
   * @param item1 a pointer to the item being sought.
   *
   * @param comparator a pointer to a @sbmlconstant{ListItemComparator,} function used to
   * find the item of interest in this list.
   *
   * @return the first occurrence of @p item1 in this List or @c NULL if
   * @p item1 was not found.
   */
  void* find (const void *item1, ListItemComparator comparator) const;


  /**
   * Find all items in this list satisfying a given predicate function.
   *
   * The typedef for @sbmlconstant{ListItemPredicate,} is:
   * @code
   *   int (*ListItemPredicate) (const void *item);
   * @endcode
   * where a return value of @c 1 represents true and @c 0 represents
   * false.
   *
   * The caller owns the returned list (but not its constituent items) and
   * is responsible for deleting it.
   *
   * @param predicate the function applied to each item in this list.
   * 
   * @return a new List containing (pointers to) all items in this List for
   * which <code>predicate(item)</code> returned nonzero (true).  The
   * returned list may be empty if none of the items satisfy the @p
   * predicate.
   */
  List* findIf (ListItemPredicate  predicate) const;


  /**
   * Get the nth item in this List.
   *
   * If @p n > <code>List.size()</code>, this method returns @c 0.
   *
   * @return the nth item in this List.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see remove()
   */
  void* get (unsigned int n) const;


  /**
   * Adds a given item to the beginning of this List.
   *
   * @param item a pointer to the item to be added to this list.
   */
  void prepend (void *item);


  /**
   * Removes the nth item from this List and returns a pointer to it.
   *
   * If @p n > <code>List.size()</code>, this method returns @c 0.
   *
   * @return the nth item in this List.
   * If the index @p n is invalid, @c NULL is returned.
   *
   * @see get()
   */
  void* remove (unsigned int n);


  /**
   * Get the number of items in this List.
   * 
   * @return the number of elements in this List.
   */
  unsigned int getSize () const;

  /**
   * Merge this elements of the second list into this list (by pointing the last ListNode to the first ListNode in the supplied List).
   *
   */
  void transferFrom(List* list);

  /** @cond doxygenLibsbmlInternal */
 /**
  * Delete all child elements of the given list, and then the list itself.
  *
  * The child elements are to be deleted by the @sbmlconstant{ListDeleteItemFunc,} predicate, that will be called with 
  * each element.
  *
  */
  static void deleteListAndChildrenWith(List* list, ListDeleteItemFunc delteFunc);

  /** @endcond */


  /** @cond doxygenLibsbmlInternal */

  ListIterator begin();
  ListIterator end();

  /** @endcond */

protected:
  /** @cond doxygenLibsbmlInternal */
  unsigned int size;
  ListNode*    head;
  ListNode*    tail;

  /** @endcond */
};

#endif  /* __cplusplus */


/**
 * @def List_freeItems(list, free_item, type)
 * Frees the items in the given List.
 *
 * Iterates over the items in this List and frees each one in turn by
 * calling the passed-in 'void free_item(type *)' function.
 *
 * The List itself will not be freed and so may be re-used.  To free
 * the List, use the destructor.
 *
 * While the function prototype cannot be expressed precisely in C syntax,
 * it is roughly:
 * @code
 *  List_freeItems(List_t *lst, void (*free_item)(type *), type)
 * @endcode
 * where @c type is a C type resolved at compile time.
 *
 * Believe it or not, defining List_freeItems() as a macro is actually more
 * type safe than can be acheived with straight C.  That is, in C, the
 * free_item() function would need to take a void pointer argument,
 * requiring any type safe XXX_free() functions to be re-written to be less
 * safe.
 *
 * As with all line-continuation macros, compile-time errors will still
 * report the correct line number.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
#define List_freeItems(list, free_item, type)                \
{                                                            \
  unsigned int size = List_size(list);                       \
  while (size--) free_item( (type *) List_remove(list, 0) ); \
}

LIBSBML_CPP_NAMESPACE_END


#ifndef SWIG
/*BEGIN_C_DECLS */

#include <sbml/common/sbmlfwd.h>

/* END_C_DECLS */


LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new List_t and returns a pointer to it.
 *
 * The pointer that is returned by this function is owned by the caller, 
 * who is responsible for deleting it.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
List_t *
List_create (void);


/**
 * @cond doxygenLibsbmlInternal
 *
 * Creates a new ListNode_t (with @p item) and returns a pointer to the node.
 *
 * @param item the item to create a ListNode_t for.
 *
 * The pointer that is returned by this function is owned by the caller,
 * who is responsible for deleting it.
 *
 * @if conly
 * @memberof ListNode_t
 * @endif
 */
LIBSBML_EXTERN
ListNode_t *
ListNode_create (void *item);
/** @endcond */

/**
 * Frees the given List_t.
 *
 * This function does not free List_t items.  It frees only the List_t
 * structure and its constituent internal structures (if any).
 *
 * Presumably, you either i) have pointers to the individual list items
 * elsewhere in your program and you want to keep them around for awhile
 * longer or ii) the list has no items ( List_size() == @c 0 ).  If neither
 * are true, try List_freeItems() instead.
 *
 * @param lst the List_t structure.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void
List_free (List_t *lst);


/**
 * @cond doxygenLibsbmlInternal
 *
 * Frees the given ListNode_t.
 *
 * @param node the ListNode_t structure.
 *
 * @memberof ListNode_t
 */
LIBSBML_EXTERN
void
ListNode_free (ListNode_t *node);
/** @endcond */

/**
 * Adds a pointer to @p item to the end of this List_t.
 *
 * @param lst the List_t structure.
 * @param item the item to add to the end of the list.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void
List_add (List_t *lst, void *item);


/**
 * @return the number of items in this List_t for which 
 * <code>predicate(item)</code> returns true.
 *
 * The typedef for @sbmlconstant{ListItemPredicate,} is:
 *
 *   int (*ListItemPredicate) (const void *item);
 *
 * where a return value of @c 1 represents true and @c 0 represents
 * false.
 *
 * @param lst the List_t structure.
 * @param predicate the predicate to test the elements of the list against.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
unsigned int
List_countIf (const List_t *lst, ListItemPredicate predicate);


/**
 * Searches the List_t for an entry that matches @p item1, according
 * to @p comparator.
 *
 * The @sbmlconstant{ListItemComparator,} is a pointer to a function used to find
 * a matching entry.  The typedef for @sbmlconstant{ListItemComparator,} is:
 *
 * int (*ListItemComparator) (void *item1, void *item2);
 *
 * The return value semantics are the same as for strcmp:
 * <ul>
 * <li>  -1    item1 <  item2,
 * <li>   0    item1 == item 2
 * <li>   1    item1 >  item2
 * </ul>
 *
 * @param lst the List_t structure.
 * @param item1 the item to look for.
 * @param comparator the pointer to the function used to find the item.
 *
 * @return the first occurrence of an entry matching @p item1 in this 
 * List_t or @c NULL if item was not found.
 *
 * The pointer that is returned by this function is not owned by the caller, 
 * but may be queried and modified.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void *
List_find ( const List_t       *lst,
            const void         *item1,
            ListItemComparator comparator );


/**
 * Create and return a new List_t with the subset of items for which
 * <code>predicate(item)</code> returns true.  The returned list may be empty.
 *
 * The caller owns the returned list (but not its constituent items) and is
 * responsible for freeing it with List_free().
 *
 * @param lst the List_t structure.
 * @param predicate the predicate to test the elements of the list against.
 *
 * @return a new List_t containing (pointers to) all items in this List_t for
 * which <code>predicate(item)</code> was true.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
List_t *
List_findIf (const List_t *lst, ListItemPredicate predicate);


/**
 * Returns the nth item in this List_t.  If n > List_size() returns NULL.
 *
 * @param lst the List_t structure.
 * @param n the index of the item to find.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void *
List_get (const List_t *lst, unsigned int n);


/**
 * Adds item to the beginning of this List_t.
 *
 * @param lst the List_t structure.
 * @param item the item to add to the list.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void
List_prepend (List_t *lst, void *item);


/**
 * Removes the nth item from this List_t and returns a pointer to it.  If n >
 * List_size() returns NULL.
 *
 * @param lst the List_t structure.
 * @param n the index of the item to remove.
 *
 * Unlike other 'remove' functions in libsbml, the caller does not own the 
 * returned item: it continues to be owned by whoever owned it before being
 * put on the List_t.  It may be queried or changed by the caller, like other
 * elements of the List_t.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
void *
List_remove (List_t *lst, unsigned int n);


/**
 * The number of elements in @p lst.
 *
 * @param lst the List_t structure.
 *
 * @return the number of elements in the given List_t.
 *
 * @if conly
 * @memberof List_t
 * @endif
 */
LIBSBML_EXTERN
unsigned int
List_size (const List_t *lst);


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG  */
#endif  /* List_h */
