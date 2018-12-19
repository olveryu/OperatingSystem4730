/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _AVG_H_RPCGEN
#define _AVG_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAXAVGSIZE 100

struct input_data {
	struct {
		u_int input_data_len;
		double *input_data_val;
	} input_data;
};
typedef struct input_data input_data;

struct stat {
	double average;
	double mode;
	double median;
};
typedef struct stat stat;


#define STAT_PROGS 21500
#define STAT_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define STAT 1
extern  stat * stat_1(input_data *, CLIENT *);
extern  stat * stat_1_svc(input_data *, struct svc_req *);
extern int stat_progs_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define STAT 1
extern  stat * stat_1();
extern  stat * stat_1_svc();
extern int stat_progs_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_input_data (XDR *, input_data*);
extern  bool_t xdr_stat (XDR *, stat*);
extern  bool_t xdr_input_data (XDR *, input_data*);

#else /* K&R C */
extern bool_t xdr_input_data ();
extern bool_t xdr_stat ();
extern bool_t xdr_input_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_AVG_H_RPCGEN */
