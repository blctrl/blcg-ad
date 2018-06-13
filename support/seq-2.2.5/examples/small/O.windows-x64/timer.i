#line 1 "../timer.st"
// This small example program demonstrates how user-defined data structures and
// functions can be used to implement resettable timers. The code is a slightly
// polished version of what I wrote in response to a question from Yevgeny A.
// Gusev <Ye.A.Gusev@inp.nsk.su> who suggested a new state option to make the
// built-in delay() function behave in this way.

program timerExample

struct timer {
    double delta_delay; /* initial delay */
    double delay;       /* current delay */
};

struct timer timer1, timer2, timer3;

void timer_init(struct timer *timer, double delay_time) {
    timer->delay = timer->delta_delay = delay_time;
}

void timer_reset(struct timer *timer) {
    timer->delay += timer->delta_delay;
}

void dump_time_diff(char *msg, struct epicsTimeStamp *relative_to) {
    struct epicsTimeStamp current_time;
    epicsTimeGetCurrent(&current_time);
    printf("%s: %.3f\n", msg,
        epicsTimeDiffInSeconds(&current_time, relative_to));
}

struct epicsTimeStamp start_time;

entry {
    epicsTimeGetCurrent(&start_time);
}

ss timer {
    struct epicsTimeStamp cycle_start_time;
    state reset {
        when () {
            timer_init(&timer1, 0.333);
            timer_init(&timer2, 0.877);
            timer_init(&timer3, 2.797);
            epicsTimeGetCurrent(&cycle_start_time);
            dump_time_diff("timers initialized", &start_time);
        } state cycle
    }
    state cycle {
        option -t;
        when (delay(60.166)) {
        } state reset
        when (delay(timer1.delay)) {
            timer_reset(&timer1);
            dump_time_diff("timer1", &cycle_start_time);
        } state cycle
        when (delay(timer2.delay)) {
            timer_reset(&timer2);
            dump_time_diff("timer2", &cycle_start_time);
        } state cycle
        when (delay(timer3.delay)) {
            timer_reset(&timer3);
            dump_time_diff("timer3", &cycle_start_time);
        } state cycle
    }
}


































#line 101 "../timer.st"
