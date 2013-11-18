#pragma hdrstop
#include "rrEvent.h"
#include "rrRandom.h"
#include <iostream>
//---------------------------------------------------------------------------

namespace rr
{

Event::Event(int id, double prior, double delay)
:
mID(id),
mPriority(prior),
mDelay(delay)
{}

Event::Event(const Event& rhs)
{
    (*this) = rhs;
}

Event& Event::operator=(const Event& rhs)
{
    if(this != &rhs)
    {
        (*this).mID         = rhs.mID;
        (*this).mPriority     = rhs.mPriority;
        (*this).mDelay         = rhs.mDelay;
    }

    return *this;
}

int    Event::GetID() const
{
    return mID;
}

void Event::SetPriority(double prior)
{
    mPriority = prior;
}

double Event::GetPriority() const
{
    return mPriority;
}

//Friend functions
bool operator==(const Event &e1, const Event &e2)
{
    if(e1.mID == e2.mID && e1.mPriority == e2.mPriority && e1.mDelay == e2.mDelay)
    {
        return true;
    }
      return false;
}

bool operator<(const Event &e1, const Event &e2)
{
     if(e1.mPriority == e2.mPriority && e1.mPriority !=0 && e1.mID != e2.mID)
    {
        //Random toss...
        return (e1.mRandom.NextDouble() > 0.5) ? false : true;
    }

    return e1.mPriority >= e2.mPriority;    //Used in sorting algorithm
}

std::ostream& operator<<(std::ostream& stream, const Event& anEvent)
{
    // something bizarre in gcc was causing this to fail if the event fields are
    // dumped directly to the ostream. Seems fine if we make a stack based copy
    // though

    stream << "Event ID: ";
    stream << anEvent.GetID();
    stream << " Priority: ";
    stream << anEvent.GetPriority();
    return stream;
}

}//namespace
