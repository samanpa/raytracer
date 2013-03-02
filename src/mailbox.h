#ifndef MAILBOX_H
#define MAILBOX_H

#if MBOX == 1
struct mailboxentry { unsigned int ray; unsigned int tri;};

template <int N>
struct mailbox {
        mailboxentry _entries[N];
        mailbox() {
                for (int i = 0; i < N; ++i) {
                        _entries[i].ray = -1;
                        _entries[i].tri = -1;
                }
        }

        inline void add(scene& s, unsigned int ray, unsigned int tri) {
                unsigned i = tri % N;
                _entries[i].ray = ray;
                _entries[i].tri = tri;
        }

        inline bool find(scene& s, unsigned int ray, unsigned int tri) {
                unsigned i = tri % N;
                return (_entries[i].ray == ray)  & (_entries[i].tri == tri);
        }
};
#elif MBOX == 2
template <int N>
struct mailbox {
        union {
                ssef m128[2];
                int mint[8];
        };
        int curr;
        
        mailbox() {
                ssef FLOAT4_FF = _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF));
                m128[0] = FLOAT4_FF;
                m128[1] = FLOAT4_FF;
        }

        inline void add(scene& s, unsigned int ray, unsigned int tri) {
                mint[++curr % 8] = tri;
        }

        inline bool find(scene& s, unsigned int ray, unsigned int tri) {
                ssef trissef = _mm_castsi128_ps(_mm_set1_epi32(tri));
                ssef tmp1 = m128[0] == trissef;
                ssef tmp2 = m128[1] == trissef;
                int res = (_mm_movemask_ps(tmp1) | _mm_movemask_ps(tmp2));
                return res;
        }
};
#elif MBOX == 3
template <int N>
struct mailbox {
        mailbox() {
        }

        inline void add(scene& s, unsigned int ray, unsigned int tri) {
                s._accels[tri].pad1 == ray;
        }

        inline bool find(scene& s, unsigned int ray, unsigned int tri) {
                return s._accels[tri].pad1 == ray;
        }
};
#else
         #error "no mailbox type defined" 
#endif

#endif
