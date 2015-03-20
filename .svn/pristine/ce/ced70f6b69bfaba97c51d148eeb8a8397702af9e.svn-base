/*
 *  wiimotes.c
 *
 *  Created by Benoît Pin on 08/12/10.
 *  Copyright 2010 Ecole des mines. All rights reserved.
 *
 */


#include "m_pd.h"
#include "wiiuse.h"
#define MAX_WIIMOTES 6
#define DEFAULT_TIMEOUT 5
#define	POLLING_TIME 2 // ms

static t_class *wiimotes_class;

typedef struct _wiimotes {
	t_object x_obj;
	
	wiimote ** wiimotes;
	t_outlet ** wiimote_outlets;
	t_outlet * outlet_status;
	t_clock * x_clock;

	int maxwiimotes;
	int timeout;
	int _connected;
	int _initialized;
	
} t_wiimotes;


static void wiimotes_discover(t_wiimotes *x) {
	if (!x->_initialized) {
		x->wiimotes =  wiiuse_init(x->maxwiimotes);
		x->_initialized = 1;
	}

	if (!x->_connected) {
		int found, connected;
		
		found = wiiuse_find(x->wiimotes, x->maxwiimotes, x->timeout);
		if (!found) {
			outlet_symbol(x->outlet_status, gensym("no-wiimote-found"));
			return;
		}
		
		found = found > x->maxwiimotes ? x->maxwiimotes : found;
		
		connected = wiiuse_connect(x->wiimotes, found);
		if (connected) {
			int i;
			int ledParity, ledNumber = 2;
			t_atom ap[1];

			post("Connected to %i wiimotes (of %i found).", connected, found);
			
			for (i = 0; i < found; i++) {
				ledParity = i % 2;
				wiiuse_set_leds(x->wiimotes[i], (ledParity|ledNumber) << 4);
				if (ledParity) {
					ledNumber = ledNumber << 1;
					ledNumber = ledNumber & 0xf; 
				}
			}
			SETFLOAT(ap+0, (double)connected);
			outlet_anything(x->outlet_status, gensym("connected"), 1, ap);
			x->_connected = 1;
		}
		else {
			post("Failed to connect to any wiimote.");
			return;
		}
	}
	else {
		post("already connected");
	}

}

static void wiimotes_tick(t_wiimotes *x) {
	if (wiiuse_poll(x->wiimotes, x->maxwiimotes)) {
		int i;
		for (i = 0; i < x->maxwiimotes; i++) {
			switch (x->wiimotes[i]->event) {
				case WIIUSE_EVENT:
					
					/* buttons */
					/*
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_A)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_a"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_B)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_b"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_UP)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_up"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_down"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_left"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_right"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_MINUS)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_minus"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_PLUS)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_plus"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_ONE)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_one"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_TWO)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_two"));
					}
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_HOME)) {
						outlet_symbol(x->wiimote_outlets[i], gensym("wiimote_button_home"));
					}
					 */
					
					if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_A)     ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_B)     ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_UP)    ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_DOWN)  ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_LEFT)  ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_RIGHT) ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_MINUS) ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_PLUS)  ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_ONE)   ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_TWO)   ||
						IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_HOME)) {
						t_atom btn[1];
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_A)) {
							SETSYMBOL(btn, gensym("a"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_B)) {
							SETSYMBOL(btn, gensym("b"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_UP)) {
							SETSYMBOL(btn, gensym("up"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
							SETSYMBOL(btn, gensym("down"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
							SETSYMBOL(btn, gensym("left"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
							SETSYMBOL(btn, gensym("right"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_MINUS)) {
							SETSYMBOL(btn, gensym("minus"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_PLUS)) {
							SETSYMBOL(btn, gensym("plus"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_ONE)) {
							SETSYMBOL(btn, gensym("one"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_TWO)) {
							SETSYMBOL(btn, gensym("two"));
						}
						if (IS_JUST_PRESSED(x->wiimotes[i], WIIMOTE_BUTTON_HOME)) {
							SETSYMBOL(btn, gensym("home"));
						}
						outlet_anything(x->wiimote_outlets[i], gensym("wiimote_button_down"), 1, btn);
					}
					
					if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_A)     ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_B)     ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_UP)    ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_DOWN)  ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_LEFT)  ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_RIGHT) ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_MINUS) ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_PLUS)  ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_ONE)   ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_TWO)   ||
						IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_HOME)) {
						t_atom btn[1];
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_A)) {
							SETSYMBOL(btn, gensym("a"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_B)) {
							SETSYMBOL(btn, gensym("b"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_UP)) {
							SETSYMBOL(btn, gensym("up"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
							SETSYMBOL(btn, gensym("down"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
							SETSYMBOL(btn, gensym("left"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
							SETSYMBOL(btn, gensym("right"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_MINUS)) {
							SETSYMBOL(btn, gensym("minus"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_PLUS)) {
							SETSYMBOL(btn, gensym("plus"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_ONE)) {
							SETSYMBOL(btn, gensym("one"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_TWO)) {
							SETSYMBOL(btn, gensym("two"));
						}
						if (IS_RELEASED(x->wiimotes[i], WIIMOTE_BUTTON_HOME)) {
							SETSYMBOL(btn, gensym("home"));
						}
						outlet_anything(x->wiimote_outlets[i], gensym("wiimote_button_up"), 1, btn);
					}
					
					
					/* acceleration */
					if (WIIUSE_USING_ACC(x->wiimotes[i])) {
						t_atom ac[3];
						t_atom gf[3];
						t_atom or[3];

						SETFLOAT(ac+0, (double)x->wiimotes[i]->accel.x);
						SETFLOAT(ac+1, (double)x->wiimotes[i]->accel.y);
						SETFLOAT(ac+2, (double)x->wiimotes[i]->accel.z);
						outlet_anything(x->wiimote_outlets[i], gensym("accel"), 3, ac);

						
						SETFLOAT(gf+0, (double)x->wiimotes[i]->gforce.x);
						SETFLOAT(gf+1, (double)x->wiimotes[i]->gforce.y);
						SETFLOAT(gf+2, (double)x->wiimotes[i]->gforce.z);
						outlet_anything(x->wiimote_outlets[i], gensym("gforce"), 3, gf);

						SETFLOAT(or+0, (double)x->wiimotes[i]->orient.pitch);
						SETFLOAT(or+1, (double)x->wiimotes[i]->orient.yaw);
						SETFLOAT(or+2, (double)x->wiimotes[i]->orient.roll);
						outlet_anything(x->wiimote_outlets[i], gensym("orient"), 3, or);
						
						if (x->wiimotes[i]->exp.type == EXP_MOTION_PLUS) {
							t_atom gy[3];
							struct motion_plus_t * mp = (motion_plus_t*)&x->wiimotes[i]->exp.mp;

							SETFLOAT(gy+0, (double)mp->rx);
							SETFLOAT(gy+1, (double)mp->ry);
							SETFLOAT(gy+2, (double)mp->rz);

							outlet_anything(x->wiimote_outlets[i], gensym("gyro"), 3, gy);
						}
					}
					
					/* ir */
					if (WIIUSE_USING_IR(x->wiimotes[i])) {
						t_atom air[3];
						SETFLOAT(air+0, (double)x->wiimotes[i]->ir.x);
						SETFLOAT(air+1, (double)x->wiimotes[i]->ir.y);
						SETFLOAT(air+2, (double)x->wiimotes[i]->ir.z);
						outlet_anything(x->wiimote_outlets[i], gensym("ir"), 3, air);
					}
					
					break;
				case WIIUSE_DISCONNECT:
				case WIIUSE_UNEXPECTED_DISCONNECT:
					outlet_symbol(x->outlet_status, gensym("wiimote-disconnected"));
					break;

				default:
					break;
			}
		}
	}
	clock_delay(x->x_clock, POLLING_TIME);
}

static void wiimotes_motion_sensing(t_wiimotes * x, t_floatarg on) {
	int i;
	for (i = 0; i<x->maxwiimotes; i++) {
		wiiuse_motion_sensing(x->wiimotes[i], (int)on);
	}
}

static void wiimotes_motion_plus_sensing(t_wiimotes * x, t_floatarg on) {
	int i;
	for (i = 0; i<x->maxwiimotes; i++) {
		wiiuse_motion_sensing(x->wiimotes[i], (int)on);
		wiiuse_set_motion_plus(x->wiimotes[i], (int)on);
	}
}

static void wiimotes_ir_sensing(t_wiimotes * x, t_floatarg on) {
	int i;
	for (i = 0; i<x->maxwiimotes; i++) {
		wiiuse_set_ir(x->wiimotes[i], (int)on);
	}
}

static void* wiimotes_new(t_symbol *s, int argc, t_atom *argv)
{
	t_wiimotes *x = (t_wiimotes *)pd_new(wiimotes_class);
	t_outlet ** outs;
	int i;

	switch (argc) {
		case 1:
			x->maxwiimotes = atom_getint(argv);
			x->timeout = DEFAULT_TIMEOUT;
			break;

		case 2:
			x->maxwiimotes = atom_getint(argv);
			x->timeout = atom_getint(argv+1);
			break;

		default:
			x->maxwiimotes = MAX_WIIMOTES;
			x->timeout = DEFAULT_TIMEOUT;
			break;
	}
	
	
	if(!(outs = (t_outlet **)getbytes(x->maxwiimotes * sizeof(*outs))))
	   return (0);
	
	x->wiimote_outlets = outs;
	
	for (i=0; i<x->maxwiimotes; i++) {
		x->wiimote_outlets[i] = outlet_new(&x->x_obj, NULL);
	}
	x->outlet_status = outlet_new(&x->x_obj, NULL);
	x->_connected = 0;
	x->x_clock = clock_new(x, (t_method)wiimotes_tick);
	clock_delay(x->x_clock, POLLING_TIME);
	return (void *)x;
}

static void wiimotes_free(t_wiimotes* x) {
	//wiiuse_cleanup(x->wiimotes, MAX_WIIMOTES);
	clock_free(x->x_clock);
}


void wiimotes_setup(void) {
	wiimotes_class = class_new(gensym("wiimotes"),
							   (t_newmethod)wiimotes_new,
							   (t_method)wiimotes_free,
							   sizeof(t_wiimotes),
							   CLASS_DEFAULT,
							   A_GIMME,
							   0);
	//class_addbang(wiimotes_class, wiimotes_bang);
	class_addmethod(wiimotes_class, (t_method) wiimotes_discover, gensym("discover"), 0);
	class_addmethod(wiimotes_class, (t_method)wiimotes_motion_sensing, gensym("motion_sensing"), A_DEFFLOAT, 0);
	class_addmethod(wiimotes_class, (t_method)wiimotes_motion_plus_sensing, gensym("motion_plus_sensing"), A_DEFFLOAT, 0);
	class_addmethod(wiimotes_class, (t_method)wiimotes_ir_sensing, gensym("ir_sensing"), A_DEFFLOAT, 0);
	
}
