#ifndef CHANGESTATS_H
#define CHANGESTATS_H

#include "edgetree.h"
#include "changestat.h"



/* Add your function names here */

CHANGESTAT_FN(d_nmkstar);
CHANGESTAT_FN(d_nmtriangle);
CHANGESTAT_FN(d_nmedges);
CHANGESTAT_FN(d_blkedges);
CHANGESTAT_FN(d_blktriangle);
CHANGESTAT_FN(d_blkkstar);
CHANGESTAT_FN(d_blktriangle2);
CHANGESTAT_FN(d_blkkstar2);
CHANGESTAT_FN(d_blktriangle3);
CHANGESTAT_FN(d_blkkstar3);
CHANGESTAT_FN(d_degseq);
CHANGESTAT_FN(d_blkdegseq);
int calblock(int npara,double *para,int node,int offset);
int calblockid(int npara,double *para,int node1,int node2,int offset);
#endif
