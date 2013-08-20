/*
 * EventQueue.h
 *
 *  Created on: Aug 16, 2013
 *      Author: andy
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include "rrOSSpecifics.h"
#include <deque>
#include <queue>

namespace rr
{
class LLVMExecutableModel;
}

namespace rrllvm {

class Event
{
public:
    Event(rr::LLVMExecutableModel&, uint id);
    ~Event();
    void assign();

    bool getExpired();

    rr::LLVMExecutableModel& model;
    uint id;
    double priority;
    double assignTime;
    double* data;
};

}

namespace rr
{
class LLVMExecutableModel;

/**
 * compare functor to determine event priority
 */
struct EventComparator
{
    EventComparator(LLVMExecutableModel&);
    bool operator()(uint a, uint b);

    LLVMExecutableModel &model;
};

/**
 * specialazation of less comparator for the Event pointer type.
 */
template<> struct std::less<rrllvm::Event*>
{
    bool operator()(const rrllvm::Event *a, const rrllvm::Event *b) const
    {
        if (a->assignTime != b->assignTime)
        {
            return a->assignTime > b->assignTime;
        }
        else
        {
            return a->priority < b->priority;
        }
    }
};


class EventQueue :
        private std::priority_queue<uint, std::deque<rrllvm::Event*> >
{
public:
    typedef std::priority_queue<uint, std::deque<rrllvm::Event*> > _base;
    typedef _base::container_type::const_iterator const_iterator;
    typedef _base::container_type::iterator iterator;

    EventQueue(LLVMExecutableModel&);

    /**
     *  Returns true if the %queue is empty.
     */
    bool empty() const
    { return _base::empty(); }

    /**  Returns the number of elements in the %queue.  */
    size_type
    size() const
    { return c.size(); }

    /**
     *  Returns a read-only (constant) reference to the data at the first
     *  element of the %queue.
     */
    const_reference top() const
    {
        return _base::top();
    }

    void push(uint eventId)
    {
        _base::push(new rrllvm::Event(model, eventId));
    }

    /**
     *  @brief  Removes first element.
     *
     *  This is a typical %queue operation.  It shrinks the %queue
     *  by one.  The time complexity of the operation depends on the
     *  underlying sequence.
     *
     *  Note that no data is returned, and if the first element's
     *  data is needed, it should be retrieved before pop() is
     *  called.
     */
    void pop()
    {
        std::pop_heap(c.begin(), c.end(), comp);
        rrllvm::Event *p = c.end();
        c.pop_back();
        delete p;
    }

    iterator find(uint key);

    iterator end();

    iterator begin();

    void make_heap();

    void erase(iterator pos);

    void eraseExpiredEvents();

    bool hasCurrentEvents();

private:
    LLVMExecutableModel& model;
};


} /* namespace rr */



#endif /* EVENTQUEUE_H_ */
