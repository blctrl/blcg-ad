/* C code for program exitOptxTest, generated by snc from ../exitOptx.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"
# line 7 "../exitOptx.st"
#include "../testSupport.h"

/* Variable declarations */
struct seqg_vars_opt_x {
# line 14 "../exitOptx.st"
	int exit1cnt;
# line 15 "../exitOptx.st"
	int exit2cnt;
# line 16 "../exitOptx.st"
	int delayed1;
# line 17 "../exitOptx.st"
	int delayed2;
} seqg_vars_opt_x;


/* Function declarations */

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
	{
# line 14 "../exitOptx.st"
	static int seqg_initvar_exit1cnt = 0;
	memcpy(&seqg_vars_opt_x.exit1cnt, &seqg_initvar_exit1cnt, sizeof(seqg_initvar_exit1cnt));
	}
	{
# line 15 "../exitOptx.st"
	static int seqg_initvar_exit2cnt = 0;
	memcpy(&seqg_vars_opt_x.exit2cnt, &seqg_initvar_exit2cnt, sizeof(seqg_initvar_exit2cnt));
	}
	{
# line 16 "../exitOptx.st"
	static int seqg_initvar_delayed1 = 0;
	memcpy(&seqg_vars_opt_x.delayed1, &seqg_initvar_delayed1, sizeof(seqg_initvar_delayed1));
	}
	{
# line 17 "../exitOptx.st"
	static int seqg_initvar_delayed2 = 0;
	memcpy(&seqg_vars_opt_x.delayed2, &seqg_initvar_delayed2, sizeof(seqg_initvar_delayed2));
	}
}

/* Program entry func */
static void seqg_entry(SS_ID seqg_env)
{
# line 10 "../exitOptx.st"
	seq_test_init(4);
}

/****** Code for state "plus_x" in state set "opt_x" ******/

/* Exit function for state "plus_x" in state set "opt_x" */
static void seqg_exit_opt_x_0_plus_x(SS_ID seqg_env)
{
# line 25 "../exitOptx.st"
	testOk1(seqg_vars_opt_x.exit1cnt == 0);
# line 26 "../exitOptx.st"
	seqg_vars_opt_x.exit1cnt++;
}

/* Event function for state "plus_x" in state set "opt_x" */
static seqBool seqg_event_opt_x_0_plus_x(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 19 "../exitOptx.st"
	if (seq_delay(seqg_env, 0.1))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 22 "../exitOptx.st"
	if (seqg_vars_opt_x.delayed1)
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "plus_x" in state set "opt_x" */
static void seqg_action_opt_x_0_plus_x(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 20 "../exitOptx.st"
			seqg_vars_opt_x.delayed1++;
		}
		return;
	case 1:
		{
		}
		return;
	}
}

/****** Code for state "minus_x" in state set "opt_x" ******/

/* Exit function for state "minus_x" in state set "opt_x" */
static void seqg_exit_opt_x_0_minus_x(SS_ID seqg_env)
{
# line 38 "../exitOptx.st"
	seqg_vars_opt_x.exit2cnt++;
# line 39 "../exitOptx.st"
	testOk1(seqg_vars_opt_x.exit2cnt == seqg_vars_opt_x.delayed2);
}

/* Event function for state "minus_x" in state set "opt_x" */
static seqBool seqg_event_opt_x_0_minus_x(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 31 "../exitOptx.st"
	if (seq_delay(seqg_env, 0.1))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 34 "../exitOptx.st"
	if (seqg_vars_opt_x.delayed2 > 1)
	{
		seq_exit(seqg_env);
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "minus_x" in state set "opt_x" */
static void seqg_action_opt_x_0_minus_x(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 32 "../exitOptx.st"
			seqg_vars_opt_x.delayed2++;
		}
		return;
	case 1:
		{
# line 35 "../exitOptx.st"
			testOk1(seqg_vars_opt_x.exit2cnt == seqg_vars_opt_x.delayed2);
		}
		return;
	}
}

/* Program exit func */
static void seqg_exit(SS_ID seqg_env)
{
# line 45 "../exitOptx.st"
	seq_test_done();
}

#undef seqg_var

/************************ Tables ************************/

/* No channel definitions */
#define seqg_chans 0

/* Event masks for state set "opt_x" */
static const seqMask seqg_mask_opt_x_0_plus_x[] = {
	0x00000000,
};
static const seqMask seqg_mask_opt_x_0_minus_x[] = {
	0x00000000,
};

/* State table for state set "opt_x" */
static seqState seqg_states_opt_x[] = {
	{
	/* state name */        "plus_x",
	/* action function */   seqg_action_opt_x_0_plus_x,
	/* event function */    seqg_event_opt_x_0_plus_x,
	/* entry function */    0,
	/* exit function */     seqg_exit_opt_x_0_plus_x,
	/* event mask array */  seqg_mask_opt_x_0_plus_x,
	/* state options */     (0)
	},
	{
	/* state name */        "minus_x",
	/* action function */   seqg_action_opt_x_0_minus_x,
	/* event function */    seqg_event_opt_x_0_minus_x,
	/* entry function */    0,
	/* exit function */     seqg_exit_opt_x_0_minus_x,
	/* event mask array */  seqg_mask_opt_x_0_minus_x,
	/* state options */     (0 | OPT_DOEXITTOSELF)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "opt_x",
	/* states */            seqg_states_opt_x,
	/* number of states */  2
	},
};

/* Program table (global) */
seqProgram exitOptxTest = {
	/* magic number */      2002005,
	/* program name */      "exitOptxTest",
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
	/* exit func */         seqg_exit,
	/* num. queues */       0
};

#define PROG_NAME exitOptxTest
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void exitOptxTestRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&exitOptxTest);
}
epicsExportRegistrar(exitOptxTestRegistrar);
