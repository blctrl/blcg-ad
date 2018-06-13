/* C code for program snctest, generated by snc from ../sncExample.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"

/* Variable declarations */
struct seqg_vars {
# line 7 "../sncExample.st"
	float v;
};


/* Function declarations */

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
}

/****** Code for state "low" in state set "ss1" ******/

/* Event function for state "low" in state set "ss1" */
static seqBool seqg_event_ss1_0_low(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 15 "../sncExample.st"
	if (seqg_var->v > 5.0)
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 19 "../sncExample.st"
	if (seq_delay(seqg_env, .1))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "low" in state set "ss1" */
static void seqg_action_ss1_0_low(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 17 "../sncExample.st"
			printf("now changing to high\n");
		}
		return;
	case 1:
		{
		}
		return;
	}
}

/****** Code for state "high" in state set "ss1" ******/

/* Event function for state "high" in state set "ss1" */
static seqBool seqg_event_ss1_0_high(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 25 "../sncExample.st"
	if (seqg_var->v <= 5.0)
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 29 "../sncExample.st"
	if (seq_delay(seqg_env, .1))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "high" in state set "ss1" */
static void seqg_action_ss1_0_high(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 27 "../sncExample.st"
			printf("changing to low\n");
		}
		return;
	case 1:
		{
		}
		return;
	}
}

#undef seqg_var

/************************ Tables ************************/

/* Channel table */
static seqChan seqg_chans[] = {
	/* chName, offset, varName, varType, count, eventNum, efId, monitored, queueSize, queueIndex */
	{"{user}:xxxExample", offsetof(struct seqg_vars, v), "v", P_FLOAT, 1, 1, 0, 1, 0, 0},
};

/* Event masks for state set "ss1" */
static const seqMask seqg_mask_ss1_0_low[] = {
	0x00000002,
};
static const seqMask seqg_mask_ss1_0_high[] = {
	0x00000002,
};

/* State table for state set "ss1" */
static seqState seqg_states_ss1[] = {
	{
	/* state name */        "low",
	/* action function */   seqg_action_ss1_0_low,
	/* event function */    seqg_event_ss1_0_low,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_ss1_0_low,
	/* state options */     (0)
	},
	{
	/* state name */        "high",
	/* action function */   seqg_action_ss1_0_high,
	/* event function */    seqg_event_ss1_0_high,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_ss1_0_high,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "ss1",
	/* states */            seqg_states_ss1,
	/* number of states */  2
	},
};

/* Program table (global) */
seqProgram snctest = {
	/* magic number */      2002005,
	/* program name */      "snctest",
	/* channels */          seqg_chans,
	/* num. channels */     1,
	/* state sets */        seqg_statesets,
	/* num. state sets */   1,
	/* user var size */     sizeof(struct seqg_vars),
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF | OPT_REENT),
	/* init func */         seqg_init,
	/* entry func */        0,
	/* exit func */         0,
	/* num. queues */       0
};

#define PROG_NAME snctest
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void snctestRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&snctest);
}
epicsExportRegistrar(snctestRegistrar);
