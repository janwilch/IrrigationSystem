#ifndef STATE_HPP
#define STATE_HPP

enum State {
    NONE = 0,
    INIT = 1,
    AWAIT_MEASUREMENT = 2,
    MEASURING = 3,
    START_IRRIGATION = 4,
    IRRIGATING = 5,
    WAITING = 6
};

#endif // STATE_HPP
