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
#include <cstring>
#include <iomanip>

static void dump_array(std::ostream &os, int n, const double *p)
{
    if (p)
    {
        os << std::setiosflags(std::ios::floatfield) << std::setprecision(8);
        os << '[';
        for (int i = 0; i < n; ++i)
        {
            os << p[i];
            if (i < n - 1)
            {
                os << ", ";
            }
        }
        os << ']' ;
    }
    else
    {
        os << "NULL" ;
    }
}

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
    else
    {
        std::memset(data, 0, dataSize * sizeof(double));
    }

    Log(Logger::PRIO_DEBUG) << "created event at time " << model.getTime() <<
            ": " << *this;
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
    Log(Logger::PRIO_DEBUG) << "assigning event: " << *this;
    model.assignEvent(id, data);
}

bool Event::isPersistent() const
{
    return model.getEventPersistent(id);
}

bool Event::useValuesFromTriggerTime() const
{
    return model.getEventUseValuesFromTriggerTime(id);
}

bool Event::isTriggered() const
{
    return model.getEventTrigger(id);
}

bool operator<(const Event& a, const Event& b)
{
    if (a.assignTime != b.assignTime)
    {
        return a.assignTime > b.assignTime;
    }
    else
    {
        double ap = a.getPriority(); double bp = b.getPriority();
        if (ap != bp)
        {
            return ap < bp;
        }
        else
        {
            return a.model.getEventTieBreak(a.id, b.id);
        }
    }
}

std::ostream& operator <<(std::ostream& os, const Event& event)
{
    os << "Event{ " << event.id << ", " <<
            event.model.getEventTrigger(event.id) << ", " <<
            event.isExpired() << ", " << event.isCurrent() << ", " <<
            event.getPriority() << ", " << event.delay << ", " <<
            event.assignTime << ", ";

    dump_array(os, event.dataSize, event.data);

    os << "}";

    return os;
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

bool EventQueue::eraseExpiredEvents()
{
    bool erased = false;
    iterator i = c.begin();
    while (i < c.end())
    {
        if (!(*i).isExpired())
        {
            ++i;
        }
        else
        {
            Log(Logger::PRIO_DEBUG) << "removing expired event: " << *i;
            i = c.erase(i);
            erased = true;
        }
    }
    if (erased)
    {
        make_heap();
    }
    return erased;
}

bool EventQueue::hasCurrentEvents()
{
    return size() && top().isCurrent();
}

bool EventQueue::applyEvent()
{
    bool applied = false;
    if (_base::size())
    {
        const Event& e = _base::top();
        if (e.isPersistent() || e.isTriggered())
        {
            if (e.delay == 0.0 || e.assignTime <= e.model.getTime())
            {
                e.assign();
                _base::pop(); // note, e is no longer valid after pop
                applied = true;
            }
        }
    }

    if(applied)
    {
        // erase doesn't care about heap property, but heapifies
        // if something was erased, we need to return the queue
        // in heapified state, so heapify in not already heapified
        // by erase.
        if (!eraseExpiredEvents())
        {
            make_heap();
        }
    }

    return applied;
}

}




