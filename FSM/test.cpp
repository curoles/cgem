
#include <functional>
#include <tuple>

namespace gem { namespace fsm {

/// State Transition Table
template<class Implementor>
struct STT
{
template<
    int   CurrentState,
    class Event,
    int   NextState,
    void (Implementor::*action)(const Event&)
>
struct Transition
{
    static int const current_state = CurrentState;
    static int const next_state    = NextState;
    typedef Event event;
    typedef Implementor fsm_t;
 
    static
    void execute(Implementor& m, const Event& e) {
        (m.*action)(e);
    }
};
};

template<
    class Transition,
    class Next
>
struct EventDispatcher
{
    typedef typename Transition::fsm_t fsm_t;
    typedef typename Transition::event event;

    static int dispatch(fsm_t& fsm, int state, event const& e)
    {
        if (state == Transition::current_state)
        {
            Transition::execute(fsm, e);
            return Transition::next_state;
        }
        else
        {
            return Next::dispatch(fsm, state, e);
        }
    }
};

struct DefaultEventDispatcher
{
    template<class FSM, class Event>
    static int dispatch(/*StateMachine<FSM>&*/FSM& m, int state, Event const& e) {
        return m.no_transition(state, e);
    }
};

template<typename Head, typename ...Tail>
struct fold_event_dispatchers
{
    typedef Head type;
};
 
template<typename Head, typename Next, typename... Tail>
struct fold_event_dispatchers<Head, Next, Tail...>
{
    typedef typename fold_event_dispatchers<
        EventDispatcher<Head,Next>,
        Tail...
    >::type type;
};

template<typename Head, typename Next, typename Next2, typename... Tail>
struct fold_event_dispatchers<Head, Next, Next2, Tail...>
{
    typedef typename fold_event_dispatchers<
        EventDispatcher<Head, EventDispatcher<Next,Next2> >,
        Tail...
    >::type type;
};

template<class DefaultDispatcher, typename Tuple>
struct fold_dispatchers;

template<class DefaultDispatcher, typename... T>
struct fold_dispatchers<DefaultDispatcher, std::tuple<T...> >
{
    typedef typename fold_event_dispatchers<
        T...,
        DefaultDispatcher
    >::type type;
};

template<class Event, typename Tuple>
struct filter_transitions_by_event;

template<class Event>
struct filter_transitions_by_event<Event, std::tuple<> >
{
    using type = std::tuple<>;
};

template<typename T, typename Tuple>
struct cons_tuple;

template<typename T, typename ...Args>
struct cons_tuple<T, std::tuple<Args...> >
{
    using type = std::tuple<T, Args...>;
};

template<class Event, typename Head, typename... Tail>
struct filter_transitions_by_event<Event, std::tuple<Head, Tail...> >
{
    using type = typename std::conditional<
        std::is_same<Event, typename Head::event>::value,
        typename cons_tuple<
            Head,
            typename filter_transitions_by_event<Event, std::tuple<Tail...>>::type
        >::type,
        typename filter_transitions_by_event<Event, std::tuple<Tail...> >::type
    >::type;
};

template<class Table, class Event>
struct generate_dispatcher :
    fold_dispatchers<
        DefaultEventDispatcher,
        typename filter_transitions_by_event<Event, Table>::type
    >
{};


template<class Implementor, class Event>
int process_event(Implementor& m, Event const& e)
{

    // generate the dispatcher type
    typedef typename generate_dispatcher<
        typename Implementor::TransitionTable, Event
    >::type dispatcher;

    m.state_ = dispatcher::dispatch(
        m, m.state_, e
    );

    // return new state
    return m.state_;
}


}} // gem::fsm

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

