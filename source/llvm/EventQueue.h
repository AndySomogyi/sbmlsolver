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
#include <list>

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

    /**
     * is this event ready to be applied
     */
    bool isRipe() const;

    rr::LLVMExecutableModel& model;
    uint id;
    double delay;
    double assignTime;
    uint dataSize;
    double* data;


    friend bool operator<(const Event& a, const Event& b);

};

std::ostream& operator <<(std::ostream& os, const Event& data);


class EventQueue
{
public:
    typedef std::list<rrllvm::Event> _Sequence;
    typedef std::less<_Sequence::value_type> _Compare;
    typedef _Sequence::const_iterator const_iterator;
    typedef _Sequence::iterator iterator;
    typedef _Sequence::const_reference const_reference;

    _Sequence  c;
    _Compare   comp;


    void make_heap();

    bool eraseExpiredEvents();

    bool hasCurrentEvents();

    bool applyEvent();

    uint size() const;

    const_reference top();

    void push(const Event& e);

    double getNextPendingEventTime();


    friend std::ostream& operator<< (std::ostream& stream, const EventQueue& queue);

private:


    void pop();

    uint packTop();

};

std::ostream& operator<< (std::ostream& stream, const EventQueue& queue);




} /* namespace rrllvm */



#endif /* EVENTQUEUE_H_ */
