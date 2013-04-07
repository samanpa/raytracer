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
		__sync_add_and_fetch(&this->cnt, 1);
		__sync_add_and_fetch(&this->val, val);
	}
	void inc(uint64_t val, int cnt) {
		__sync_add_and_fetch(&this->cnt, cnt);
		__sync_add_and_fetch(&this->val, val);
	}
};


#endif
