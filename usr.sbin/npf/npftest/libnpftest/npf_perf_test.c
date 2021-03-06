/*
 * NPF benchmarking.
 *
 * Public Domain.
 */

#ifdef _KERNEL
#include <sys/types.h>
#include <sys/param.h>

#include <sys/kernel.h>
#include <sys/kmem.h>
#include <sys/kthread.h>
#endif

#include "npf_impl.h"
#include "npf_test.h"

#define	NSECS		10 /* seconds */

static volatile int	run;
static volatile int	done;

static uint64_t *	npackets;
static bool		stateful;

static struct mbuf *
fill_packet(unsigned i)
{
	struct mbuf *m;
	struct ip *ip;
	struct udphdr *uh;
	char buf[32];

	m = mbuf_construct(IPPROTO_UDP);
	uh = mbuf_return_hdrs(m, false, &ip);

	snprintf(buf, sizeof(buf), "192.0.2.%u", i + i);
	ip->ip_src.s_addr = inet_addr(PUB_IP1);
	ip->ip_dst.s_addr = inet_addr(stateful ? LOCAL_IP2 : LOCAL_IP3);
	uh->uh_sport = htons(80);
	uh->uh_dport = htons(15000 + i);
	return m;
}

__dead static void
worker(void *arg)
{
	npf_t *npf = npf_getkernctx();
	ifnet_t *ifp = npf_test_getif(IFNAME_INT);
	unsigned int i = (uintptr_t)arg;
	struct mbuf *m = fill_packet(i);
	uint64_t n = 0;

	while (!run)
		/* spin-wait */;
	while (!done) {
		int error;

		error = npf_packet_handler(npf, &m, ifp, PFIL_OUT);
		KASSERT(error == 0); (void)error;
		n++;
	}
	npackets[i] = n;
	kthread_exit(0);
}

void
npf_test_conc(bool st, unsigned nthreads)
{
	uint64_t total = 0;
	int error;
	lwp_t **l;

	printf("THREADS\tPKTS\n");
	stateful = st;
	done = false;
	run = false;

	npackets = kmem_zalloc(sizeof(uint64_t) * nthreads, KM_SLEEP);
	l = kmem_zalloc(sizeof(lwp_t *) * nthreads, KM_SLEEP);

	for (unsigned i = 0; i < nthreads; i++) {
		error = kthread_create(PRI_NONE, KTHREAD_MUSTJOIN |
		    KTHREAD_MPSAFE, NULL, worker, (void *)(uintptr_t)i,
		    &l[i], "npfperf");
		KASSERT(error == 0); (void)error;
	}

	/* Let them spin! */
	run = true;
	kpause("perf", false, mstohz(NSECS * 1000), NULL);
	done = true;

	/* Wait until all threads exit and sum the counts. */
	for (unsigned i = 0; i < nthreads; i++) {
		kthread_join(l[i]);
		total += npackets[i];
	}
	kmem_free(npackets, sizeof(uint64_t) * nthreads);
	kmem_free(l, sizeof(lwp_t *) * nthreads);

	printf("%u\t%" PRIu64 "\n", nthreads, total / NSECS);
}
