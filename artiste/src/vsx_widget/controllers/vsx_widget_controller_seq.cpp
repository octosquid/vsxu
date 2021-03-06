/**
* Project: VSXu: Realtime modular visual programming language, music/audio visualizer.
*
* This file is part of Vovoid VSXu.
*
* @author Jonatan Wallmander, Robert Wenzel, Vovoid Media Technologies AB Copyright (C) 2003-2013
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef VSX_NO_CLIENT
#include "vsx_gl_global.h"
//#include <string>
//#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <vector>
#include <math.h>
#include "vsx_math_3d.h"
#include "vsx_texture_info.h"
#include "vsx_texture.h"
#include "vsx_command.h"
#include "vsx_font.h"
#include "vsxfst.h"
#include "vsx_mouse.h"
#include "vsx_param.h"
#include "vsx_module.h"

#include "vsx_widget_base.h"
#include "vsx_widget_base_controller.h"
#include "vsx_widget_controller.h"
#include "vsx_widget_controller_seq.h"
#include "sequencer/vsx_widget_sequence.h"
#include "sequencer/vsx_widget_seq_chan.h"

void vsx_widget_controller_sequence::init()
{
  if (init_run) return;
  init_children();
  init_run = true;
  set_size(vsx_vector(0.12f,0.1f));
  seq_chan = add(new vsx_widget_seq_channel,"chan");
  if (size_controlled_from_outside)
  {
  	seq_chan->size = size;
  } else
  {
  	seq_chan->size = size*0.95f;
  }
  seq_chan->target_size = seq_chan->size;
  seq_chan->init();
  seq_chan->visible = 1;
  ((vsx_widget_seq_channel*)seq_chan)->set_view_time(-0.05f, 1.05f);
  ((vsx_widget_seq_channel*)seq_chan)->hidden_by_sequencer = false;
  ((vsx_widget_seq_channel*)seq_chan)->y_start = -0.15f;
  ((vsx_widget_seq_channel*)seq_chan)->y_end = 1.1f;
  ((vsx_widget_seq_channel*)seq_chan)->is_controller = true;
  get_value();
}

void vsx_widget_controller_sequence::set_size(vsx_vector new_size)
{
	if (seq_chan)
	seq_chan->set_size(new_size);
  vsx_widget::set_size(new_size);
}

void vsx_widget_controller_sequence::vsx_command_process_b(vsx_command_s *t) {
  if (t->cmd == "update")
  {
    //printf("command gotten from chan: %s\n",t->raw.c_str());
    command_q_b.add_raw("param_set "+(target_param!= ""?(target_param+" "):"")+t->parts[2]+command_suffix);
    parent->vsx_command_queue_b(this);
  } else
  if (t->cmd == "pg_ok") {
    command_q_b.add_raw("pseq_p_ok inject_get foo bar "+t->parts[3]);
    seq_chan->vsx_command_queue_b(this);
  }
  else
  if (t->cmd == "pg64_ok") {
#ifdef VSXU_DEBUG
    printf("command gotten from server: %s\n",t->raw.c_str());
#endif
    command_q_b.add_raw("pseq_p_ok inject_get foo bar "+base64_decode(t->parts[3]));
    seq_chan->vsx_command_queue_b(this);
  }
  else
  if (t->cmd == "menu_close" || t->cmd == "remove_chan")
  {
    //if (!parent_removal)
    //{
			_delete();
//		} else
//		{
//			command_q_b.add_raw("controller_sequence_close "+i2s(id)+command_suffix);
//			parent->vsx_command_queue_b(this);
//		}
  }
}

void vsx_widget_controller_sequence::i_draw()
{
	if (draw_base)
	{
		vsx_widget_base_controller::i_draw();
	}
}

void vsx_widget_controller_sequence::set_span(float new_y_start, float new_y_end)
{
  ((vsx_widget_seq_channel*)seq_chan)->y_start = new_y_start;
  ((vsx_widget_seq_channel*)seq_chan)->y_end = new_y_end;
}

void vsx_widget_controller_sequence::set_view_time(float start, float end)
{
	((vsx_widget_seq_channel*)seq_chan)->set_view_time(start, end);
}

#endif
