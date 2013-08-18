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

/**
 * compare functor to determine event priority
 */
struct EventComparator
{
    EventComparator(LLVMExecutableModel&);
    bool operator()(uint a, uint b);

    LLVMExecutableModel &model;
};


class EventQueue :
        public std::priority_queue<uint, std::deque<uint>, EventComparator>
{
public:
    typedef std::priority_queue<uint, std::deque<uint>, EventComparator> _base;
    typedef _base::container_type::const_iterator const_iterator;
    typedef _base::container_type::iterator iterator;

    EventQueue(LLVMExecutableModel&);

    iterator find(uint key);

    iterator end();

    iterator begin();

    void make_heap();

    void erase(iterator pos);

    void eraseExpiredEvents();

    bool hasCurrentEvents();
};


} /* namespace rr */
#endif /* EVENTQUEUE_H_ */
