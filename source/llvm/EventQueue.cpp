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

using namespace rr;

namespace rrllvm
{

Event::Event(rr::LLVMExecutableModel& model, uint id) :
        model(model),
        id(id),
        delay(model.getEventDelay(id)),
        assignTime(delay + model.getTime()),
        dataSize(model.getEventBufferSize(id)),
        data(new double[model.getEventBufferSize(id)])
{
    if (model.getEventUseValuesFromTriggerTime(id))
    {
        model.getEventData(id, data);
    }
}

Event::Event(const Event& o) :
        model(o.model),
        id(o.id),
        delay(o.delay),
        assignTime(o.assignTime),
        dataSize(o.dataSize),
        data(new double[o.dataSize])
{
    std::copy(o.data, o.data + o.dataSize, data);
}

Event& Event::operator=(const Event& rhs )
{
    delete data;
    model = rhs.model;
    id = rhs.id;
    delay = rhs.delay;
    assignTime = rhs.assignTime;
    dataSize = rhs.dataSize;
    data = new double[rhs.dataSize];
    std::copy(rhs.data, rhs.data + rhs.dataSize, data);
    return *this;
}

Event::~Event()
{
    delete data;
}

bool Event::isExpired() const
{
    return !(model.getEventTrigger(id) || model.getEventPersistent(id));
}

bool Event::isCurrent() const
{
    return delay == 0.0 && (model.getEventPersistent(id) ||
            model.getEventTrigger(id));
}

double Event::getPriority() const
{
    return model.getEventPriority(id);
}

void Event::assign() const
{
    if (!model.getEventUseValuesFromTriggerTime(id))
    {
        model.getEventData(id, data);
    }
    model.assignEvent(id, data);
}

bool operator<(const Event& a, const Event& b)
{
    if (a.assignTime != b.assignTime)
    {
        return a.assignTime > b.assignTime;
    }
    else
    {
        return a.getPriority() < b.getPriority();
    }
}

struct EventPredicate
{
    EventPredicate(uint eventId) : eventId(eventId) {};

    bool operator()(const rrllvm::Event& event) const
    {
        return event.id == eventId;
    }

    uint eventId;
};

bool EventQueue::find(uint key)
{
    return std::find_if(c.begin(), c.end(), EventPredicate(key)) != c.end();
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
        uint id = (*i).id;
        if (!(*i).isExpired())
        {
            ++i;
        }
        else
        {
            i = c.erase(i);
            Log(Logger::PRIO_DEBUG) << "removed expired event " << id;
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
    return size() && top().isCurrent();
}

}




