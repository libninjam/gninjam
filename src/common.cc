/*
    Copyright (C) 2006 Tobias Gehrig <tobias@gehrignet.de>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "common.hh"

Glib::ustring on_hscale_volume_format_value(double value)
{
  char output[10];
  snprintf(output, sizeof(output), "%.1f dB", value);
  return output;
}

Glib::ustring on_hscale_pan_format_value(double value)
{
  if (value == 0.0) {
    return "center";
  } else {
    char output[9];
    snprintf(output, sizeof(output), "%d %%", (int)(value*100));
    return output;
  }
}
