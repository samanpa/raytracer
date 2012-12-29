#ifndef BPRAY_STATISTICS_H
#define BPRAY_STATISTICS_H

struct statistic {
	uint64_t val;
	uint64_t cnt;
	statistic() 
		: val (0), cnt(0) {}      
	void inc(uint64_t val) {
		this->val += val;
		++this->cnt;
	}
};


#endif
