#include "profile.h"

profile_internal_use_only_timer_t profile_internal_use_only[] = {
# define PROFILE_TIMER_INIT( timer ) { #timer, 0., 0., 0, 0 },
  PROFILE_TIMERS( PROFILE_TIMER_INIT )
# undef PROFILE_TIMER_INIT
  { NULL, 0., 0., 0, 0 }
};

void
update_profile( int dump ) {
  profile_internal_use_only_timer_t * p;
  double sum = 0, sum_total = 0;

  for( p=profile_internal_use_only; p->name!=NULL; p++ ) {
    p->t_total += p->t;
    p->n_total += p->n;
    sum        += p->t;
    sum_total  += p->t_total;
  }

  if( dump ) {
    print_log( "\n" // 890123456 | xxx% x.xe+xx x.xe+xx x.xe+xx | xxx% x.xe+xx x.xe+xx x.xe+xx 
               "                 |      Since Last Update       |          Cumulative\n"
               "    Operation    | Pct   Time    Count    Per   | Pct   Time    Count    Per\n"
               "-----------------+------------------------------+------------------------------\n" );

    for( p=profile_internal_use_only; p->name!=NULL; p++ ) {
      if( p->n==0 && p->n_total==0 ) continue;
      print_log( "%16.16s | % 3d%% %.1e %.1e %.1e | % 3d%% %.1e %.1e %.1e\n",
                 p->name,
                 (int)( 100.*p->t/sum + 0.5 ), p->t,
                 (double)p->n,
                 p->t/(DBL_EPSILON+(double)p->n ),
                 (int)( 100.*p->t_total/sum_total + 0.5 ), p->t_total,
                 (double)p->n_total,
                 p->t_total/(DBL_EPSILON+(double)p->n_total) );
    }
    
    print_log( "\n" );
  }

  for( p=profile_internal_use_only; p->name!=NULL; p++ ) {
    p->t = 0;
    p->n = 0;
  }
}