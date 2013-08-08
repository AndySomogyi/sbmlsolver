#ifndef rrEventH
#define rrEventH
#include "rrObject.h"
#include "rrRandom.h"
//---------------------------------------------------------------------------
namespace rr
{


class RR_DECLSPEC Event
{


public:
    Event(int id, double prior = 0, double delay = 0);
    Event(const Event& id);
    ~Event() {}
    double                  GetPriority() const;
    void                    SetPriority(double prior);
    int                     GetID() const;
    Event&                  operator = (const Event& rhs);
    friend bool             operator < (const Event& e1, const Event& e2);
    friend bool             operator == (const Event& e1, const Event& e2);

protected:
    int                     mID;
    double                  mPriority;
    double                  mDelay;
    Random                  mRandom;    //If we need randomness..
};

// compare and printing functions,
// there is some bizzare problem with gcc when these are
// declared as friend functions, so use the public interface,
// and all seems to work.
RR_DECLSPEC std::ostream& operator << (std::ostream& str, const Event& event);


struct RR_DECLSPEC SortByPriority
{
    bool operator()( const rr::Event& lx, const rr::Event& rx ) const
    {
        return lx.GetPriority() > rx.GetPriority();
    }
};

}
#endif
