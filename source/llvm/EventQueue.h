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
    Event(const Event& other);
    Event& operator=( const Event& rhs );
    ~Event();


    void assign() const;

    bool isExpired() const;

    bool isCurrent() const;

    double getPriority() const;

    rr::LLVMExecutableModel& model;
    uint id;
    double delay;
    double assignTime;
    uint dataSize;
    double* data;


    friend bool operator<(const Event& a, const Event& b);

};


class EventQueue :
        public std::priority_queue<uint, std::deque<rrllvm::Event> >
{
public:
    typedef std::priority_queue<uint, std::deque<rrllvm::Event> > _base;
    typedef _base::container_type::const_iterator const_iterator;
    typedef _base::container_type::iterator iterator;

    bool find(uint key);

    void make_heap();

    void erase(iterator pos);

    void eraseExpiredEvents();

    bool hasCurrentEvents();
};


} /* namespace rrllvm */



#endif /* EVENTQUEUE_H_ */
