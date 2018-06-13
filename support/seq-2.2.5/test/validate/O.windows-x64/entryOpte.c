/* C code for program entryOpteTest, generated by snc from ../entryOpte.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"
# line 7 "../entryOpte.st"
#include "../testSupport.h"

/* Variable declarations */
struct seqg_vars_opt_e {
# line 14 "../entryOpte.st"
	int entry1cnt;
# line 15 "../entryOpte.st"
	int entry2cnt;
# line 16 "../entryOpte.st"
	int delayed1;
# line 17 "../entryOpte.st"
	int delayed2;
} seqg_vars_opt_e;


/* Function declarations */

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
	{
# line 14 "../entryOpte.st"
	static int seqg_initvar_entry1cnt = 0;
	memcpy(&seqg_vars_opt_e.entry1cnt, &seqg_initvar_entry1cnt, sizeof(seqg_initvar_entry1cnt));
	}
	{
# line 15 "../entryOpte.st"
	static int seqg_initvar_entry2cnt = 0;
	memcpy(&seqg_vars_opt_e.entry2cnt, &seqg_initvar_entry2cnt, sizeof(seqg_initvar_entry2cnt));
	}
	{
# line 16 "../entryOpte.st"
	static int seqg_initvar_delayed1 = 0;
	memcpy(&seqg_vars_opt_e.delayed1, &seqg_initvar_delayed1, sizeof(seqg_initvar_delayed1));
	}
	{
# line 17 "../entryOpte.st"
	static int seqg_initvar_delayed2 = 0;
	memcpy(&seqg_vars_opt_e.delayed2, &seqg_initvar_delayed2, sizeof(seqg_initvar_delayed2));
	}
}

/* Program entry func */
static void seqg_entry(SS_ID seqg_env)
{
# line 10 "../entryOpte.st"
	seq_test_init(5);
}

/****** Code for state "plus_e" in state set "opt_e" ******/

/* Entry function for state "plus_e" in state set "opt_e" */
static void seqg_entry_opt_e_0_plus_e(SS_ID seqg_env)
{
# line 21 "../entryOpte.st"
	testOk1(seqg_vars_opt_e.entry1cnt == 0);
# line 22 "../entryOpte.st"
	seqg_vars_opt_e.entry1cnt++;
}

/* Event function for state "plus_e" in state set "opt_e" */
static seqBool seqg_event_opt_e_0_plus_e(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 24 "../entryOpte.st"
	if (seq_delay(seqg_env, 0.1))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 27 "../entryOpte.st"
	if (seqg_vars_opt_e.delayed1)
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "plus_e" in state set "opt_e" */
static void seqg_action_opt_e_0_plus_e(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 25 "../entryOpte.st"
			seqg_vars_opt_e.delayed1++;
		}
		return;
	case 1:
		{
		}
		return;
	}
}

/****** Code for state "minus_e" in state set "opt_e" ******/

/* Entry function for state "minus_e" in state set "opt_e" */
static void seqg_entry_opt_e_0_minus_e(SS_ID seqg_env)
{
# line 33 "../entryOpte.st"
	seqg_vars_opt_e.entry2cnt++;
# line 34 "../entryOpte.st"
	testOk1(seqg_vars_opt_e.entry2cnt == seqg_vars_opt_e.delayed2 + 1);
}

/* Event function for state "minus_e" in state set "opt_e" */
static seqBool seqg_event_opt_e_0_minus_e(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 36 "../entryOpte.st"
	if (seq_delay(seqg_env, 0.1))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
# line 39 "../entryOpte.st"
	if (seqg_vars_opt_e.delayed2 > 1)
	{
		seq_exit(seqg_env);
		*seqg_ptrn = 1;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "minus_e" in state set "opt_e" */
static void seqg_action_opt_e_0_minus_e(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 37 "../entryOpte.st"
			seqg_vars_opt_e.delayed2++;
		}
		return;
	case 1:
		{
# line 40 "../entryOpte.st"
			testOk1(seqg_vars_opt_e.entry2cnt == seqg_vars_opt_e.delayed2 + 1);
		}
		return;
	}
}

/* Program exit func */
static void seqg_exit(SS_ID seqg_env)
{
# line 46 "../entryOpte.st"
	seq_test_done();
}

#undef seqg_var

/************************ Tables ************************/

/* No channel definitions */
#define seqg_chans 0

/* Event masks for state set "opt_e" */
static const seqMask seqg_mask_opt_e_0_plus_e[] = {
	0x00000000,
};
static const seqMask seqg_mask_opt_e_0_minus_e[] = {
	0x00000000,
};

/* State table for state set "opt_e" */
static seqState seqg_states_opt_e[] = {
	{
	/* state name */        "plus_e",
	/* action function */   seqg_action_opt_e_0_plus_e,
	/* event function */    seqg_event_opt_e_0_plus_e,
	/* entry function */    seqg_entry_opt_e_0_plus_e,
	/* exit function */     0,
	/* event mask array */  seqg_mask_opt_e_0_plus_e,
	/* state options */     (0)
	},
	{
	/* state name */        "minus_e",
	/* action function */   seqg_action_opt_e_0_minus_e,
	/* event function */    seqg_event_opt_e_0_minus_e,
	/* entry function */    seqg_entry_opt_e_0_minus_e,
	/* exit function */     0,
	/* event mask array */  seqg_mask_opt_e_0_minus_e,
	/* state options */     (0 | OPT_DOENTRYFROMSELF)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "opt_e",
	/* states */            seqg_states_opt_e,
	/* number of states */  2
	},
};

/* Program table (global) */
seqProgram entryOpteTest = {
	/* magic number */      2002005,
	/* program name */      "entryOpteTest",
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

#define PROG_NAME entryOpteTest
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void entryOpteTestRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&entryOpteTest);
}
epicsExportRegistrar(entryOpteTestRegistrar);
