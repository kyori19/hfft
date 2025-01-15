#include <hls_stream.h>
#include <ap_axi_sdata.h>

const size_t N = 4;
const size_t MAX_LEN = 16 * 1024;

typedef ap_fixed<32, 4> real_t;

typedef real_t data_in_t[N];
typedef real_t data_out_t[N * 2];

typedef hls::axis<data_in_t, 0, 0, 0> axis_in_t;
typedef hls::axis<data_out_t, 0, 0, 0> axis_out_t;

void hfft(hls::stream<axis_in_t> &stream_in, hls::stream<axis_out_t> &stream_out);
