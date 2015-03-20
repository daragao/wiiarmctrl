/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Michal Wiedenbauer	< shagkur >
 *		Dave Murphy			< WinterMute >
 *		Hector Martin		< marcan >
 *		William Alozy		< wiill >
 *
 *	Copyright 2009
 *
 *
 *	This file is part of wiiuse and fWIIne.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Handles device I/O (non-OS specific).
 */

#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "wiiuse_internal.h"
#include "nunchuk.h"
#include "classic.h"
#include "guitar_hero_3.h"
//#include ".h"
#include "motion_plus.h"
#include "io.h"


 /**
 *	@brief Get initialization data from the wiimote.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param data		unused
 *	@param len		unused
 *
 *	When first called for a wiimote_t structure, a request
 *	is sent to the wiimote for initialization information.
 *	This includes factory set accelerometer data.
 *	The handshake will be concluded when the wiimote responds
 *	with this data.
 */
void wiiuse_handshake(struct wiimote_t* wm, byte* data, unsigned short len) {
	if (!wm)	return;

	switch (wm->handshake_state) {
		case 0:
		{
			/* send request to wiimote for accelerometer calibration */
			byte* buf;

			WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_HANDSHAKE);
			wiiuse_set_leds(wm, WIIMOTE_LED_NONE);

			buf = (byte*)malloc(sizeof(byte) * 8);
			wiiuse_read_data_cb(wm, wiiuse_handshake, buf, WM_MEM_OFFSET_CALIBRATION, 7);
			wm->handshake_state++;
			
			wiiuse_set_leds(wm, WIIMOTE_LED_NONE);

			break;
		}
		case 1:
		{
			struct read_req_t* req = wm->read_req;
			struct accel_t* accel = &wm->accel_calib;

			accel->cal_zero.x = ((data[0]<<2)|((data[3]>>4)&3));
			accel->cal_zero.y = ((data[1]<<2)|((data[3]>>2)&3));
			accel->cal_zero.z = ((data[2]<<2)|(data[3]&3));

			accel->cal_g.x = (((data[4]<<2)|((data[7]>>4)&3)) - accel->cal_zero.x);
			accel->cal_g.y = (((data[5]<<2)|((data[7]>>2)&3)) - accel->cal_zero.y);
			accel->cal_g.z = (((data[6]<<2)|(data[7]&3)) - accel->cal_zero.z);
			/* received read data */
			/*accel->cal_zero.x = req->buf[0];
			accel->cal_zero.y = req->buf[1];
			accel->cal_zero.z = req->buf[2];

			accel->cal_g.x = req->buf[4] - accel->cal_zero.x;
			accel->cal_g.y = req->buf[5] - accel->cal_zero.y;
			accel->cal_g.z = req->buf[6] - accel->cal_zero.z;*/

			/* done with the buffer */
			free(req->buf);

			/* handshake is done */
			WIIUSE_DEBUG("Handshake finished. Calibration: Idle: X=%x Y=%x Z=%x\t+1g: X=%x Y=%x Z=%x",
					accel->cal_zero.x, accel->cal_zero.y, accel->cal_zero.z,
					accel->cal_g.x, accel->cal_g.y, accel->cal_g.z);


			/* request the status of the wiimote to see if there is an expansion */
			wiiuse_status(wm);
			
			WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_HANDSHAKE);
			WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_HANDSHAKE_COMPLETE);
			wm->handshake_state++;

			/* now enable IR if it was set before the handshake completed */
			if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_IR)) {
				WIIUSE_DEBUG("Handshake finished, enabling IR.");
				WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_IR);
				wiiuse_set_ir(wm, 1);
			}
			wm->event = WIIUSE_CONNECT;
			wiiuse_status(wm);

			break;
		}
		default:
		{
			break;
		}
	}
}
void handshake_expansion_start(struct wiimote_t *wm)
{
	if(WIIMOTE_IS_SET(wm,WIIMOTE_STATE_EXP) || WIIMOTE_IS_SET(wm,WIIMOTE_STATE_EXP_FAILED) || WIIMOTE_IS_SET(wm,WIIMOTE_STATE_EXP_HANDSHAKE))
		return;

	wm->expansion_state = 0;
	WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP_HANDSHAKE);
	handshake_expansion(wm, NULL, 0);
}

/**
 *	@brief Handle the handshake data from the expansion device.
 *
 *	@param wm		A pointer to a wiimote_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 *
 *	Tries to determine what kind of expansion was attached
 *	and invoke the correct handshake function.
 *
 *	If the data is NULL then this function will try to start
 *	a handshake with the expansion.
 */
void handshake_expansion(struct wiimote_t* wm, byte* data, unsigned short len) {
	int id;
	byte val = 0;//12; //pour tester
	byte buf = 0x00;
	byte* handshake_buf;

if (val==0) { // pour tester
	switch(wm->expansion_state) {
		/* These two initialization writes disable the encryption */
		case 0:
			wm->expansion_state = 1;
			/* increase the timeout until the handshake completes */
			#ifdef WIN32
			WIIUSE_DEBUG("write 0x55 - Setting timeout to expansion %i ms.", wm->exp_timeout);
			wm->timeout = wm->exp_timeout;
			#endif
			buf = 0x55;
			wiiuse_write_data_cb(wm, WM_EXP_MEM_ENABLE1, &buf, 1, handshake_expansion);
			break;
		case 1:
			wm->expansion_state = 2;
			/* increase the timeout until the handshake completes */
			#ifdef WIN32
			WIIUSE_DEBUG("write 0x00 - Setting timeout to expansion %i ms.", wm->exp_timeout);
			wm->timeout = wm->exp_timeout;
			#endif
			val = 0x00;
			wiiuse_write_data_cb(wm, WM_EXP_MEM_ENABLE2, &buf, 1, handshake_expansion);
			break;
		case 2:
			wm->expansion_state = 3;
			/* get the calibration data */
			if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP))
				disable_expansion(wm);
			handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));
			wiiuse_read_data_cb(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);

			/* tell the wiimote to send expansion data */
			WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP);
			break;
		case 3:
			if(!data || !len) return;
			id = BIG_ENDIAN_LONG(*(int*)(data + 220));
			switch(id) {
				case EXP_ID_CODE_NUNCHUK:
					if (nunchuk_handshake(wm, &wm->exp.nunchuk, data, len)) 
						wm->event = WIIUSE_NUNCHUK_INSERTED;
					break;
				case EXP_ID_CODE_CLASSIC_CONTROLLER:
					if (classic_ctrl_handshake(wm, &wm->exp.classic, data, len)) 
						wm->event = WIIUSE_CLASSIC_CTRL_INSERTED;
					break;
				case EXP_ID_CODE_GUITAR:
					if (guitar_hero_3_handshake(wm, &wm->exp.gh3, data, len))
						wm->event = WIIUSE_GUITAR_HERO_3_CTRL_INSERTED;
					break;
 				default:
					WIIUSE_WARNING("Unknown expansion type. Code: 0x%x", id);
					break;
			}
			free(data);
			WIIMOTE_DISABLE_STATE(wm,WIIMOTE_STATE_EXP_HANDSHAKE);
			WIIMOTE_ENABLE_STATE(wm,WIIMOTE_STATE_EXP);
			wiiuse_set_ir_mode(wm);
			wiiuse_status(wm);
			break;
	}
}//pour tester
else if (val==12) {//pour tester
		
		switch(wm->expansion_state) {		
				case 0:
					wm->expansion_state = 1;
					/* increase the timeout until the handshake completes */
					#ifdef WIN32
					WIIUSE_DEBUG("Setting timeout to expansion %i ms.", wm->exp_timeout);
					wm->timeout = wm->exp_timeout;
					#endif
					if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP))
						disable_expansion(wm);
					wiiuse_write_data_cb(wm, WM_EXP_MEM_ENABLE, &buf, 1, handshake_expansion);

					break;
				case 1:
					wm->expansion_state = 2;
					#ifdef WIN32
					WIIUSE_DEBUG("Setting timeout to expansion %i ms.", wm->exp_timeout);
					wm->timeout = wm->exp_timeout;
					#endif

					if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP))
						disable_expansion(wm);
					wiiuse_write_data_cb(wm, WM_EXP_MEM_ENABLE, &buf, 1, NULL);
					/* get the calibration data */
					handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));
					wiiuse_read_data_cb(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);

					/* tell the wiimote to send expansion data */
					WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP);
					break;
				case 2:
					if(!data || !len) return;
					id = BIG_ENDIAN_LONG(*(int*)(&data[220]));
					//id = BIG_ENDIAN_LONG(*(int*)(data + 220));

						switch(id) {
							case EXP_ID_CODE_NUNCHUK:
								if (nunchuk_handshake(wm, &wm->exp.nunchuk, data, len)) 
									wm->event = WIIUSE_NUNCHUK_INSERTED;
								break;
							case EXP_ID_CODE_CLASSIC_CONTROLLER:
								if (classic_ctrl_handshake(wm, &wm->exp.classic, data, len)) 
									wm->event = WIIUSE_CLASSIC_CTRL_INSERTED;
								break;
							case EXP_ID_CODE_GUITAR:
								if (guitar_hero_3_handshake(wm, &wm->exp.gh3, data, len))
									wm->event = WIIUSE_GUITAR_HERO_3_CTRL_INSERTED;
								break;
			 				default:
								WIIUSE_WARNING("Unknown expansion type. Code: 0x%x", id);
								break;
						}
					free(data);
					WIIMOTE_DISABLE_STATE(wm,WIIMOTE_STATE_EXP_HANDSHAKE);
					WIIMOTE_ENABLE_STATE(wm,WIIMOTE_STATE_EXP);
					wiiuse_set_ir_mode(wm);
					wiiuse_status(wm);
					break;
		}

		
}//pour tester
else //pour tester
{//pour tester

	if (!data) {
		byte* handshake_buf;
		byte buf = 0x00;

		if (WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP))
			disable_expansion(wm);

		/* increase the timeout until the handshake completes */
		#ifdef WIN32
		WIIUSE_DEBUG("Setting timeout to expansion %i ms.", wm->exp_timeout);
		wm->timeout = wm->exp_timeout;
		#endif

		wiiuse_write_data(wm, WM_EXP_MEM_ENABLE, &buf, 1);

		/* get the calibration data */
		handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));
		wiiuse_read_data_cb(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);

		/* tell the wiimote to send expansion data */
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP);

		return;
	}

	id = BIG_ENDIAN_LONG(*(int*)(data + 220));

	/* call the corresponding handshake function for this expansion */
	switch (id) {
		case EXP_ID_CODE_NUNCHUK:
		{
			if (nunchuk_handshake(wm, &wm->exp.nunchuk, data, len))
				wm->event = WIIUSE_NUNCHUK_INSERTED;
			break;
		}
		case EXP_ID_CODE_CLASSIC_CONTROLLER:
		{
			if (classic_ctrl_handshake(wm, &wm->exp.classic, data, len))
				wm->event = WIIUSE_CLASSIC_CTRL_INSERTED;
			break;
		}
		case EXP_ID_CODE_GUITAR:
		{
			if (guitar_hero_3_handshake(wm, &wm->exp.gh3, data, len))
				wm->event = WIIUSE_GUITAR_HERO_3_CTRL_INSERTED;
			break;
		}
		/*case EXP_ID_CODE_MOTION_PLUS:
		{
			if (motion_plus_handshake(wm, &wm->exp.mp, data, len))
				wm->event = WIIUSE_MOTION_PLUS_INSERTED;
			break;
		}*/
		default:
		{
			WIIUSE_WARNING("Unknown expansion type. Code: 0x%x", id);
			break;
		}
	}

	free(data);

}//pour tester

}


/**
 *	@brief Disable the expansion device if it was enabled.
 *
 *	@param wm		A pointer to a wiimote_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 *
 *	If the data is NULL then this function will try to start
 *	a handshake with the expansion.
 */
void disable_expansion(struct wiimote_t* wm) {
	if (!WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP))
		return;

	/* tell the assoicated module the expansion was removed */
	switch (wm->exp.type) {
		case EXP_NUNCHUK:
			nunchuk_disconnected(&wm->exp.nunchuk);
			wm->event = WIIUSE_NUNCHUK_REMOVED;
			break;
		case EXP_CLASSIC:
			classic_ctrl_disconnected(&wm->exp.classic);
			wm->event = WIIUSE_CLASSIC_CTRL_REMOVED;
			break;
		case EXP_GUITAR_HERO_3:
			guitar_hero_3_disconnected(&wm->exp.gh3);
			wm->event = WIIUSE_GUITAR_HERO_3_CTRL_REMOVED;
			break;
		case EXP_MOTION_PLUS:
 			motion_plus_disconnected(&wm->exp.mp);
 			wm->event = WIIUSE_MOTION_PLUS_REMOVED;
 			break;

		default:
			break;
	}

	WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP);
	wm->exp.type = EXP_NONE;
}

