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

    bool isPersistent() const;

    bool useValuesFromTriggerTime() const;

    bool isTriggered() const;

    rr::LLVMExecutableModel& model;
    uint id;
    double delay;
    double assignTime;
    uint dataSize;
    double* data;


    friend bool operator<(const Event& a, const Event& b);

};

std::ostream& operator <<(std::ostream& os, const Event& data);


class EventQueue :
        private std::priority_queue<uint, std::deque<rrllvm::Event> >
{
public:
    typedef std::priority_queue<uint, std::deque<rrllvm::Event> > _base;
    typedef _base::container_type::const_iterator const_iterator;
    typedef _base::container_type::iterator iterator;

    void make_heap();

    bool eraseExpiredEvents();

    bool hasCurrentEvents();

    bool applyEvent();

    uint size() const;

    const Event& top() const;

    void push(const Event& e);


};


} /* namespace rrllvm */



#endif /* EVENTQUEUE_H_ */
