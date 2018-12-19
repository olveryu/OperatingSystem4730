#include <rpc/rpc.h>
#include "avg.h"
#include <stdio.h>


input_data_echo * echo_1_svc(input_data* input, struct svc_req *svc) {
  static input_data_echo out;
  strncpy(out.text_echo, input->text, strlen(input->text));
  out.text_echo[strlen(input->text)] = 0;
 
  return (&out);
}
