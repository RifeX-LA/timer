#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <functional>

namespace tb {

    template <typename Duration = std::chrono::seconds>
    class timer {
    public:
        using clock_t = std::chrono::steady_clock;
        using dur = Duration;

        timer() : m_start(clock_t::now()) {}

        // returns elapsed time (default in seconds)
        [[nodiscard]] double elapsed() const {
            using basic_dur = std::chrono::nanoseconds;

            auto elapsed_duration = std::chrono::duration_cast<basic_dur>(clock_t::now() - m_start).count();
            return static_cast<double>(elapsed_duration) * basic_dur::period::num * dur::period::den
                   / basic_dur::period::den / dur::period::num;
        }

        // returns callable's execution time
        template <typename Callable, typename ... Args>
        [[nodiscard]] static double duration(Callable&& callable, Args&& ... args) {
            tb::timer<dur> t;
            std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
            return t.elapsed();
        }

        // resets the timer
        void reset() {
            m_start = clock_t::now();
        }

    private:
        std::chrono::time_point<clock_t> m_start;
    };

}

#endif //TIMER_HPP