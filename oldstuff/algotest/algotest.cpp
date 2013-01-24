
#include <iostream>
#include <inttypes.h>
using namespace std;

#define STEP_DELAY_INITIAL    5000	// max delay in us
#define STEP_DELAY_MIN  1667	// min delay in us

#define CW  0
#define CCW 1

#define TRUE 1
#define FALSE 0

#define PRESCALED_TICKS_PER_MS 8	// uh, approximately.

// ramp state-machine states
#define ramp_idle 0
#define ramp_up   1
#define ramp_max  2
#define ramp_down 3
#define ramp_last 4

int
main ()
{

  cout << "ramp_sts\t motor_pos\t pos_inc\t motor_dir\t run_flg\t ";
  cout << "step_delay_us\t step_delay_us_32\t step_no\t step_down\t ";
  cout << "move\t midpt\t denom\t pos_new\t OCR0\t step_delay_ms" << endl;

  int OCR0 = 0;
  int step_delay_ms = 0;

  int ramp_sts = ramp_idle;
  int motor_pos = 0;	// absolute step number
  int pos_inc = 0;		// motor_pos increment
  int motor_dir;		// motor direction for stepping calls
  int run_flg;		// true while motor is running

  int step_delay_us = 0;	// integer delay count in us
  long int step_delay_us_32 = 0;	// 24.8 fixed point delay count in us

  int step_no = 0;		// progress of move
  int step_down = 0;		// start of down-ramp
  int move;		// total steps to move
  int midpt;		// midpoint of move
  int denom;		// 4.n+1 in ramp algo

  int pos_new = 20;

  if (pos_new < motor_pos)	// get direction & #steps
    {
//      cout << "pos_new < motor_pos" << endl;
      move = motor_pos - pos_new;
      pos_inc = -1;
      motor_dir = CCW;
    }
  else if (pos_new != motor_pos)
    {
//      cout << "pos_new != motor_pos" << endl;
      move = pos_new - motor_pos;
      pos_inc = 1;
      motor_dir = CW;
    }
  else
    return -1;

  midpt = (move - 1) >> 1;
  step_delay_us = STEP_DELAY_INITIAL;
  step_delay_us_32 = step_delay_us << 8;	// keep step_delay_us in 24.8 fixed-point format for ramp calcs
  step_no = 0;			// step counter
  denom = 1;			// 4.n+1, n=0
  ramp_sts = ramp_up;		// start ramp state-machine
  run_flg = TRUE;

//  cout << "initial conditions:" << endl;
  cout << ramp_sts << "\t " << motor_pos << "\t " << pos_inc << "\t " << motor_dir << "\t " << run_flg << "\t ";
  cout << step_delay_us << "\t " << step_delay_us_32 << "\t " << step_no << "\t " << step_down << "\t ";
  cout << move << "\t " << midpt << "\t " << denom << "\t " << pos_new << "\t " << OCR0 << "\t " << step_delay_ms << endl;

//  cout << "entering run loop" << endl;
  while (run_flg)
    {
      switch (ramp_sts)
	{
	case ramp_up:		// accel
	  if (step_no == midpt)
	    {			// midpoint: decel
	      ramp_sts = ramp_down;
	      denom = ((step_no - move) << 2) + 1;
	      if (!(move & 1))
		{		// even move: repeat last delay before decel
		  denom += 4;
		  break;
		}
	    }
	  // no break: share code for ramp algo
	case ramp_down:	// decel
	  if (step_no == move - 1)
	    {			// next irq is cleanup (no step)
	      ramp_sts = ramp_last;
	      break;
	    }
	  denom += 4;
	  step_delay_us_32 -= (step_delay_us_32 << 1) / denom;	// ramp algorithm
	  step_delay_us = (step_delay_us_32 + 128) >> 8;	// round 24.8format->int16
	  if (step_delay_us <= STEP_DELAY_MIN)
	    {			// go to constant speed
	      ramp_sts = ramp_max;
	      step_down = move - step_no;
	      step_delay_us = STEP_DELAY_MIN;
	      break;
	    }
	  break;
	case ramp_max:		// constant speed
	  if (step_no == step_down)
	    {			// start decel
	      ramp_sts = ramp_down;
	      denom = ((step_no - move) << 2) + 5;
	    }
	  break;
	default:		// last step: cleanup
	  ramp_sts = ramp_idle;
	  // reduce motor current to holding value
	  run_flg = FALSE;	// move complete
	  break;
	}			// switch (ramp_sts)

      if (ramp_sts != ramp_idle)
	{
	  motor_pos += pos_inc;
	  ++step_no;
	  OCR0 = step_delay_us >> 7;	// set timer compare to num ticks reqd for c
	  if (ramp_sts != ramp_last)	// else repeat last action: no step
	    //halfstep(motor_dir);
	    //cout << "stepping..." << endl;
	    cout;
	}			// if (ramp_sts != ramp_idle)

  cout << ramp_sts << "\t " << motor_pos << "\t " << pos_inc << "\t " << motor_dir << "\t " << run_flg << "\t ";
  cout << step_delay_us << "\t " << step_delay_us_32 << "\t " << step_no << "\t " << step_down << "\t ";
  cout << move << "\t " << midpt << "\t " << denom << "\t " << pos_new << "\t " << OCR0 << "\t " << step_delay_ms << endl;

    }				// while (run_flg)

}
