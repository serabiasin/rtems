/*
 *  3.3.4 Examine and Change Signal Action, P1003.1b-1993, p. 70
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */


#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>
#include <rtems/posix/seterr.h>
#include <rtems/score/isr.h>

/*
 * PARAMETERS_PASSING_S is defined in ptimer.c
 */

extern void PARAMETERS_PASSING_S (int num_signal, const struct sigaction inf);

int sigaction(
  int                     sig,
  const struct sigaction *act,
  struct sigaction       *oact
)
{
  ISR_Level     level;

  if ( oact )
    *oact = _POSIX_signals_Vectors[ sig ];

  if ( !sig )
    return 0;

  if ( !is_valid_signo(sig) )
    set_errno_and_return_minus_one( EINVAL );
  
  /* 
   *  Some signals cannot be ignored (P1003.1b-1993, pp. 70-72 and references.
   *
   *  NOTE: Solaris documentation claims to "silently enforce" this which
   *        contradicts the POSIX specification.
   */

  if ( sig == SIGKILL )
    set_errno_and_return_minus_one( EINVAL );
  
  /*
   *  Evaluate the new action structure and set the global signal vector
   *  appropriately.
   */

  if ( act ) {

    /*
     *  Unless the user is installing the default signal actions, then
     *  we can just copy the provided sigaction structure into the vectors.
     */

    _ISR_Disable( level );
      if ( act->sa_handler == SIG_DFL ) {
        _POSIX_signals_Vectors[ sig ] = _POSIX_signals_Default_vectors[ sig ];
      } else {
         _POSIX_signals_Clear_process_signals( signo_to_mask(sig) );
         _POSIX_signals_Vectors[ sig ] = *act;
      }
    _ISR_Enable( level );
  }

  /*
   *  No need to evaluate or dispatch because:
   *
   *    + If we were ignoring the signal before, none could be pending 
   *      now (signals not posted when SIG_IGN).
   *    + If we are now ignoring a signal that was previously pending,
   *      we clear the pending signal indicator.
   */

  return 0;
}

