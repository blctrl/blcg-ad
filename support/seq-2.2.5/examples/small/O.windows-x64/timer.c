/* C code for program timerExample, generated by snc from ../timer.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"

struct timer {
# line 10 "../timer.st"
	double delta_delay;
# line 11 "../timer.st"
	double delay;
};

#define seqg_var (*(struct seqg_vars *const *)seqg_env)
# line 16 "../timer.st"
static void timer_init(SS_ID seqg_env, struct timer *timer, double delay_time)
{
# line 17 "../timer.st"
	timer->delay = timer->delta_delay = delay_time;
}
#undef seqg_var

#define seqg_var (*(struct seqg_vars *const *)seqg_env)
# line 20 "../timer.st"
static void timer_reset(SS_ID seqg_env, struct timer *timer)
{
# line 21 "../timer.st"
	timer->delay += timer->delta_delay;
}
#undef seqg_var

#define seqg_var (*(struct seqg_vars *const *)seqg_env)
# line 24 "../timer.st"
static void dump_time_diff(SS_ID seqg_env, char *msg, struct epicsTimeStamp *relative_to)
{
# line 25 "../timer.st"
	struct epicsTimeStamp current_time;
# line 26 "../timer.st"
	epicsTimeGetCurrent(&current_time);
# line 28 "../timer.st"
	printf("%s: %.3f\n", msg, epicsTimeDiffInSeconds(&current_time, relative_to));
}
#undef seqg_var

/* Variable declarations */
# line 14 "../timer.st"
static	struct timer timer1;
# line 14 "../timer.st"
static	struct timer timer2;
# line 14 "../timer.st"
static	struct timer timer3;
# line 31 "../timer.st"
static	struct epicsTimeStamp start_time;
struct seqg_vars_timer {
# line 38 "../timer.st"
	struct epicsTimeStamp cycle_start_time;
} seqg_vars_timer;


/* Function declarations */
# line 16 "../timer.st"
static void timer_init(SS_ID seqg_env, struct timer *timer, double delay_time);
# line 20 "../timer.st"
static void timer_reset(SS_ID seqg_env, struct timer *timer);
# line 24 "../timer.st"
static void dump_time_diff(SS_ID seqg_env, char *msg, struct epicsTimeStamp *relative_to);

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
}

/* Program entry func */
static void seqg_entry(SS_ID seqg_env)
{
# line 34 "../timer.st"
	epicsTimeGetCurrent(&start_time);
}

/****** Code for state "reset" in state set "timer" ******/

/* Event function for state "reset" in state set "timer" */
static seqBool seqg_event_timer_0_reset(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
	if (TRUE)
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "reset" in state set "timer" */
static void seqg_action_timer_0_reset(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 41 "../timer.st"
			timer_init(seqg_env, &timer1, 0.333);
# line 42 "../timer.st"
			timer_init(seqg_env, &timer2, 0.877);
# line 43 "../timer.st"
			timer_init(seqg_env, &timer3, 2.797);
# line 44 "../timer.st"
			epicsTimeGetCurrent(&seqg_vars_timer.cycle_start_time);
# line 45 "../timer.st"
			dump_time_diff(seqg_env, "timers initialized", &start_time);
		}
		return;
	}
}

/****** Code for state "cycle" in state set "timer" ******/

/* Event function for state "cycle" in state set "timer" */
static seqBool seqg_event_timer_0_cycle(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 50 "../timer.st"
	if (seq_delay(seqg_env, 60.166))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 52 "../timer.st"
	if (seq_delay(seqg_env, timer1.delay))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 1;
		return TRUE;
	}
# line 56 "../timer.st"
	if (seq_delay(seqg_env, timer2.delay))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 2;
		return TRUE;
	}
# line 60 "../timer.st"
	if (seq_delay(seqg_env, timer3.delay))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 3;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "cycle" in state set "timer" */
static void seqg_action_timer_0_cycle(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
		}
		return;
	case 1:
		{
# line 53 "../timer.st"
			timer_reset(seqg_env, &timer1);
# line 54 "../timer.st"
			dump_time_diff(seqg_env, "timer1", &seqg_vars_timer.cycle_start_time);
		}
		return;
	case 2:
		{
# line 57 "../timer.st"
			timer_reset(seqg_env, &timer2);
# line 58 "../timer.st"
			dump_time_diff(seqg_env, "timer2", &seqg_vars_timer.cycle_start_time);
		}
		return;
	case 3:
		{
# line 61 "../timer.st"
			timer_reset(seqg_env, &timer3);
# line 62 "../timer.st"
			dump_time_diff(seqg_env, "timer3", &seqg_vars_timer.cycle_start_time);
		}
		return;
	}
}

#undef seqg_var

/************************ Tables ************************/

/* No channel definitions */
#define seqg_chans 0

/* Event masks for state set "timer" */
static const seqMask seqg_mask_timer_0_reset[] = {
	0x00000000,
};
static const seqMask seqg_mask_timer_0_cycle[] = {
	0x00000000,
};

/* State table for state set "timer" */
static seqState seqg_states_timer[] = {
	{
	/* state name */        "reset",
	/* action function */   seqg_action_timer_0_reset,
	/* event function */    seqg_event_timer_0_reset,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_timer_0_reset,
	/* state options */     (0)
	},
	{
	/* state name */        "cycle",
	/* action function */   seqg_action_timer_0_cycle,
	/* event function */    seqg_event_timer_0_cycle,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_timer_0_cycle,
	/* state options */     (0 | OPT_NORESETTIMERS)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "timer",
	/* states */            seqg_states_timer,
	/* number of states */  2
	},
};

/* Program table (global) */
seqProgram timerExample = {
	/* magic number */      2002005,
	/* program name */      "timerExample",
	/* channels */          seqg_chans,
	/* num. channels */     0,
	/* state sets */        seqg_statesets,
	/* num. state sets */   1,
	/* user var size */     0,
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF),
	/* init func */         seqg_init,
	/* entry func */        seqg_entry,
	/* exit func */         0,
	/* num. queues */       0
};

#define PROG_NAME timerExample
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void timerExampleRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&timerExample);
}
epicsExportRegistrar(timerExampleRegistrar);
