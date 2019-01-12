// Constant threshold values used

// Raw Input: Distances
	// max is 1250, seems like output of ToF is pretty much millimeters
	// range: 0 to 1250
	// low = far, high = close
const int high1 = 0;
const int high2 = 140;
const int high3 = 280;
const int high4 = 420;

const int med1 = 430;
const int med2 = 570;
const int med3 = 610;
const int med4 = 850;

const int low1 = 860;
const int low2 = 1000;
const int low3 = 1140;
const int low4 = 8192;

// Output: Speeds
	// For turns, the inner wheel is slower than the outer wheel
const int v_small_slow = 135;
const int v_small_fast = 145;

const int v_full_slow = 130;
const int v_full_fast = 150;

const int v_center = 140;

// Line sensor thresholds
const int line_fwd = 160;		// max fwd

const int line_rev = 300;		// max rev
const int nudge = 100;			// duration of reverse maneuver (ms)

const int line_180 = 300;
const int degrees180 = 250; 	// duration of 180 deg turn (ms)

const int degrees45 = 30;
const int ram = 500;

const int stall = 100;

const int push = 3000;
const int smash = 1000;

const int boost = 180;

// 		Consult with Mason, TBD
