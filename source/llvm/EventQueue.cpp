/*
 * EventQueue.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: andy
 */

#include "EventQueue.h"
#include "LLVMExecutableModel.h"
#include "rrLogger.h"
#include <algorithm>

namespace rr
{

void foo(LLVMExecutableModel& model)
{
    EventComparator c(model);

    typedef std::priority_queue<uint, std::deque<uint>, EventComparator> myqueue;

    myqueue m(c);

}

EventComparator::EventComparator(LLVMExecutableModel& model) :
        model(model)
{
}

bool EventComparator::operator ()(uint a, uint b)
{
    double a_time = model.getEventAssignTime(a);
    double b_time = model.getEventAssignTime(b);

    if (a_time != b_time)
    {
        return a_time > b_time;
    }
    else
    {
        double a_priority = model.getEventPriority(a);
        double b_priority = model.getEventPriority(b);
        return a_priority < b_priority;
    }
}

EventQueue::EventQueue(LLVMExecutableModel& model) :
        _base(EventComparator(model))
{
}

EventQueue::iterator EventQueue::find(uint key)
{
    return std::find(c.begin(), c.end(), key);
}

EventQueue::iterator EventQueue::end()
{
    return c.end();
}

EventQueue::iterator EventQueue::begin()
{
    return c.begin();
}

void EventQueue::make_heap()
{
    std::make_heap(c.begin(), c.end(), comp);
}

void EventQueue::erase(EventQueue::iterator pos)
{
    c.erase(pos);
    std::make_heap(c.begin(), c.end(), comp);
}

void EventQueue::eraseExpiredEvents()
{
    bool erased = false;
    iterator i = c.begin();
    while (i < c.end())
    {
        uint event = *i;
        if (comp.model.getEventPersistent(event)
                || comp.model.getEventTrigger(event))
        {
            ++i;
        }
        else
        {
            i = c.erase(i);
            Log(Logger::PRIO_DEBUG) << "removed expired event " << event;
            erased = true;
        }
    }
    if (erased)
    {
        make_heap();
    }
}

bool EventQueue::hasCurrentEvents()
{
    return size() && comp.model.getEventDelay(top()) == 0.0;
}

}

