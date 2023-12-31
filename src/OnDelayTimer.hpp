#ifndef ONDELAYTIMER_HPP
#define ONDELAYTIMER_HPP

class OnDelayTimer
{
private:
    unsigned long currentDelta;
    unsigned long endMillis;

public:
    OnDelayTimer();
    ~OnDelayTimer();

    /// @brief Check whether the specified time delta has elapsed. If the delta changes, the timer restarts.
    /// @param ms The time delta to wait for.
    /// @return True, if the specified delta has elapsed.
    bool IsElapsed(unsigned long ms);

    /// @brief Resets the time delta.
    void Reset();
};

#endif // ONDELAYTIMER_HPP
