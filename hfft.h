#include <math.h>
#include <stdlib.h>
#include <ap_axi_sdata.h>

const size_t N = 8;
const size_t MAX_LEN = 16 * 1024;

typedef ap_fixed<32, 4> real_t;

void hfft(real_t *mem_in, real_t *mem_out);

template <size_t WIDTH>
class sin_cos_table_c {
    real_t cos_table[WIDTH / 4 + 1];

  public:
    constexpr sin_cos_table_c() : cos_table() {
        for (size_t i = 0; i <= WIDTH / 4; i++) {
            cos_table[i] = cos(-2.0 * M_PI * i / WIDTH);
        }
    }

    inline real_t cos_at(size_t i) const {
        if (i <= WIDTH / 4) {
            return get_cos_table(i);
        } else {
            return -get_cos_table(WIDTH / 2 - i);
        }
    }

    inline real_t sin_at(size_t i) const {
        if (i <= WIDTH / 4) {
            return -cos_at(WIDTH / 4 - i);
        } else {
            return -cos_at(i - WIDTH / 4);
        }
    }

  private:
    inline real_t get_cos_table(size_t i) const {
        return cos_table[i];
    }
};

template <size_t WIDTH>
const sin_cos_table_c<WIDTH> sin_cos_table;
