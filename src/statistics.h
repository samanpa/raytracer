#ifndef BPRAY_STATISTICS_H
#define BPRAY_STATISTICS_H

struct statistic {
	uint64_t val;
	uint64_t cnt;
	statistic() {
                reset();
        }
        void reset() {
                val = 0;
                cnt = 0;
        }
	void inc(uint64_t val) {
		this->val += val;
		++this->cnt;
	}
	void inc(uint64_t val, int cnt) {
		this->val += val;
		this->cnt += cnt;
	}
};


#endif
