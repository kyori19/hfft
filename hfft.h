#include <ap_axi_sdata.h>

const size_t N = 4;
const size_t MAX_LEN = 16 * 1024;

typedef ap_fixed<32, 4> real_t;

void hfft(real_t *mem_in, real_t *mem_out);
