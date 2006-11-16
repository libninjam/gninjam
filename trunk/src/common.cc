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
