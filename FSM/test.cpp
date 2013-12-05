#include "fsm.h"


#include <cassert>

using namespace gem;

class CDPlayer
{
public:
    // Events:
    struct play {};
    struct open_close {};
    struct cd_detected {};
 
    // States:
    enum State {
        Empty, Open, Stopped, Playing, Paused
    };
 
private:
    void start_playback(play const&);
    void open_drawer(open_close const&);
    void close_drawer(open_close const&);
    void store_cd_info(cd_detected const&);

public:
    typedef fsm::STT<CDPlayer> ST;


    typedef std::tuple<
    //               State     Event         Next      Action
    //             <---------+-------------+---------+--------------------------->
    ST::Transition < Empty   , open_close  , Open    , &CDPlayer::open_drawer    >,
    ST::Transition < Empty   , cd_detected , Stopped , &CDPlayer::store_cd_info  >,
    ST::Transition < Open    , open_close  , Empty   , &CDPlayer::close_drawer   >,
    ST::Transition < Stopped , play        , Playing , &CDPlayer::start_playback >
 
    > TransitionTable;


    template<class Event>
    int process_event(Event const& e) {
        return fsm::process_event(*this, e);
    }

    int state_;

    CDPlayer():state_(Empty){}

    template<class Event>
    int no_transition(int state, Event const& e)
    {
        assert(false);
        return state;
    }
};

#include <iostream>
using namespace std;

void CDPlayer::open_drawer(open_close const&)
{
    cout << "state=" << state_ << " action=open_drawer\n";
}

void CDPlayer::close_drawer(open_close const&)
{
    cout << "state=" << state_ << " action=close_drawer\n";
}

void CDPlayer::store_cd_info(cd_detected const&)
{
    cout << "state=" << state_ << " action=store_cd_info\n";
}

void CDPlayer::start_playback(play const&)
{
    cout << "state=" << state_ << " action=start_playback\n";
}

int main()
{
    CDPlayer player;

    player.process_event(CDPlayer::open_close()); // open
    player.process_event(CDPlayer::open_close()); // insert CD and close
    player.process_event(CDPlayer::cd_detected());

    return 0;
}

