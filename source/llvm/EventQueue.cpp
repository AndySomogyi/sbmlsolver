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
        return a.assignTime < b.assignTime;
    }
    else
    {
        double ap = a.getPriority(); double bp = b.getPriority();
        //if (ap != bp)
        //{
            return ap > bp;
        //}
        //else
        //{
        //    return a.model.getEventTieBreak(a.id, b.id);
        //}
    }
}

bool Event::isRipe() const
{
    return ((isPersistent() || isTriggered()) &&
            (delay == 0.0 || assignTime <= model.getTime()));

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



void EventQueue::make_heap()
{
}


bool EventQueue::eraseExpiredEvents()
{
    bool erased = false;
    iterator i = c.begin();
    while (i != c.end())
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
    return erased;
}

bool EventQueue::hasCurrentEvents()
{
    return size() && top().isCurrent();
}



bool EventQueue::applyEvent()
{
    bool applied = false;
    if (c.size())
    {
        Log(Logger::PRIO_DEBUG) << "event list before sort: " << *this;

        c.sort();

        Log(Logger::PRIO_DEBUG) << "event list after sort, before apply: " << *this;

        std::deque<iterator> ripe;
        iterator i = c.begin();
        while (i != c.end())
        {
            if (i->isRipe())
            {
                ripe.push_back(i);
            }

            const Event& a = *i;
            ++i;
            if (i != c.end() && a < (*i))
            {
                break;
            }
        }

        Log(Logger::PRIO_DEBUG) << "found " << ripe.size() << " ripe events";

        if (ripe.size())
        {
            uint index = std::rand() % ripe.size();
            iterator i = ripe[index];

            Log(Logger::PRIO_DEBUG) << "assigning the " << index << "\'th item";
            (*i).assign();

            c.erase(i);

            applied = true;

            Log(Logger::PRIO_DEBUG) << "event list after apply: " << *this;
        }
    }

    if(applied)
    {
        // erase doesn't care about heap property, but heapifies
        // if something was erased, we need to return the queue
        // in heapified state, so heapify in not already heapified
        // by erase.
        eraseExpiredEvents();
    }

    return applied;
}

uint EventQueue::size() const
{
    return c.size();
}

void EventQueue::push(const Event& e)
{
    c.push_back(e);
}

EventQueue::const_reference EventQueue::top()
{
    c.sort();
    return c.front();
}

void EventQueue::pop()
{

}

uint EventQueue::packTop()
{
    /*
    Log(Logger::PRIO_DEBUG) << "before pack top " << *this;
    uint topSize = 0;
    if (c.size())
    {
        _Sequence::iterator i = c.begin() + 1;

        while(i != c.end())
        {
            std::make_heap(i, c.end(), comp);
            ++topSize;
            ++i;
            if (*i < c.front())
            {
                break;
            }
        }
    }
    Log(Logger::PRIO_DEBUG) << "after pack top " << *this;
    return topSize;
    */
    return 0;
}

double EventQueue::getNextPendingEventTime()
{
    if (size())
    {
        return top().assignTime;
    }
    else
    {
        return 0;
    }
}

std::ostream& operator<< (std::ostream& stream, const EventQueue& queue)
{
    stream << "EventQueue {" << std::endl;
    int j = 0;
    for(EventQueue::const_iterator i = queue.c.begin(); i != queue.c.end(); ++i)
    {
        stream << "event " << j++ << ": " << *i << std::endl;
    }
    stream << "}";

    return stream;
}

}




