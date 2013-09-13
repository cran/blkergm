#include "changestats.users.h"

CHANGESTAT_FN(d_nmedges)
{
 int edgeflag, i;
  Vertex tail, head;
  
  /* *** don't forget tail -> head */    
  CHANGE_STAT[0] = 0.0;
  for (i=0; i < ntoggles; i++)
    {
      edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      CHANGE_STAT[0] += edgeflag ? - 1 : 1;
      TOGGLE_IF_MORE_TO_COME(i);
    }
/*CHANGE_STAT[0]= CHANGE_STAT[0]/((N_NODES-1)*N_NODES/2);*/
  UNDO_PREVIOUS_TOGGLES(i);
}

CHANGESTAT_FN(d_nmtriangle)
{
	Edge e;
  Vertex tail, head, change, node3;
  int i, j;
  double tailattr, edgemult;

  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    tail = TAIL(i);
    head = HEAD(i);
    edgemult = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
    change = 0;
    if(N_INPUT_PARAMS>0){ /* match on attributes */
      tailattr = INPUT_ATTRIB[tail-1];
      if(tailattr == INPUT_ATTRIB[head-1]){
        STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
          if(tailattr == INPUT_ATTRIB[node3-1]){
            if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
            else change += IS_UNDIRECTED_EDGE(node3,tail);
          }
        }
        STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
          if(tailattr == INPUT_ATTRIB[node3-1]){
            if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
            else change += IS_UNDIRECTED_EDGE(node3,tail);
          }
        }
        if(N_CHANGE_STATS>1){ /* diff = TRUE */
          for (j=0; j<N_CHANGE_STATS; j++){
            if (tailattr == INPUT_PARAM[j])
              CHANGE_STAT[j] += edgemult * change;
          }
        }else{ /* diff = FALSE */
          CHANGE_STAT[0] += edgemult * change;
        }
      }
    }else{ /* no attribute matching */
      STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
        if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
	      else change += IS_UNDIRECTED_EDGE(node3,tail);
      }
      STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
        if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
	      else change += IS_UNDIRECTED_EDGE(node3,tail);
      }
      CHANGE_STAT[0] += edgemult * change;
    }
    TOGGLE_IF_MORE_TO_COME(i);
  }
CHANGE_STAT[0]=CHANGE_STAT[0]/N_NODES;       /*((N_NODES-1)*(N_NODES-2)*N_NODES/6);*/
  UNDO_PREVIOUS_TOGGLES(i);
}


CHANGESTAT_FN(d_nmkstar)
{
	double change, taild, headd=0.0;
  int edgeflag, i, j, kmo;
  Edge e;
  Vertex tail, head, node3;
  int ninputs, nstats;
  double tailattr;
    
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  if(ninputs>nstats){
    /* match on attributes */
    for (i=0; i < ntoggles; i++) {
      /* edgeflag is 1 if edge exists and will disappear
      edgeflag is 0 if edge DNE and will appear */

      edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      tailattr = INPUT_ATTRIB[tail-1];
      if(tailattr == INPUT_ATTRIB[head-1]){
        taild = - edgeflag;
        STEP_THROUGH_OUTEDGES(tail, e, node3) {
          if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
        }
        STEP_THROUGH_INEDGES(tail, e, node3) {
          if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
        }
        headd = - edgeflag;
        STEP_THROUGH_OUTEDGES(head, e, node3) {
          if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
        }
        STEP_THROUGH_INEDGES(head, e, node3) {
          if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
        }
        
        for(j=0; j < N_CHANGE_STATS; j++) {
          kmo = ((int)INPUT_PARAM[j]) - 1;
/*          if (kmo==0) {
            change=1;
          } else { */
            change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
/*          } uncomment these few lines to define 1-stars as equivalent to 
              edges (currently, each edge is counted as two 1-stars) */
	if((int)INPUT_PARAM[j]==2)
        {
          CHANGE_STAT[j] += (edgeflag ? - change/N_NODES : change/N_NODES); 
	}
	else
	{
		CHANGE_STAT[j] += (edgeflag ? -change:change);
	}
        }
      }
      TOGGLE_IF_MORE_TO_COME(i);
    }
  }else{
    /* *** don't forget tail -> head */    
    for (i=0; i < ntoggles; i++)
    {
      /* edgeflag is 1 if edge exists and will disappear
      edgeflag is 0 if edge DNE and will appear */
      edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      taild = OUT_DEG[tail] + IN_DEG[tail] - edgeflag; 
      headd = OUT_DEG[head] + IN_DEG[head] - edgeflag;
      for(j=0; j < N_CHANGE_STATS; j++) 
      {
        kmo = ((int)INPUT_PARAM[j]) - 1;
/*        if (kmo==0) {
          change=1;
        } else { */
          change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
/*      } uncomment these few lines to define 1-stars as equivalent to 
          edges (currently, each edge is counted as two 1-stars) */
	if((int)INPUT_PARAM[j]==2){
        CHANGE_STAT[j] += (edgeflag ? - change/N_NODES : change/N_NODES); 
	}
	else
	{
		CHANGE_STAT[j] += (edgeflag ? -change:change);
	}
      }
      TOGGLE_IF_MORE_TO_COME(i);
    }
  }
  UNDO_PREVIOUS_TOGGLES(i);
}

CHANGESTAT_FN(d_blkedges)
{
  int edgeflag, i,j,bi,bj,blockid,lblk,lwb;
  Vertex tail, head;
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
	//Rprintf("INPUTPARAMS,%d\n",N_INPUT_PARAMS);
  lblk=INPUT_PARAM[0];
  lwb =INPUT_PARAM[lblk+1];
  for (i=0; i < ntoggles; i++)
    {
	edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
	for(j=1; j < lblk; j++)
	{
		if(TAIL(i) > INPUT_PARAM[j] && TAIL(i) <= INPUT_PARAM[j+1])
			bi = j+1-1;
		else if(TAIL(i) <= INPUT_PARAM[1])
			bi = 0;
		if(HEAD(i) > INPUT_PARAM[j] && HEAD(i) <= INPUT_PARAM[j+1])
			bj = j+1-1;
		else if(HEAD(i) <= INPUT_PARAM[1])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;
	//Rprintf("tail %d, head %d\n",TAIL(i),HEAD(i));
	//Rprintf("blockid %d\n",blockid); 
	for(j=0;j<lwb;j++)
	{
		if(blockid==INPUT_PARAM[j+lblk+2]-1)
		CHANGE_STAT[j]+= edgeflag ? - 1 : 1;
	}
//        CHANGE_STAT[blockid] += edgeflag ? - 1 : 1;
        TOGGLE_IF_MORE_TO_COME(i);
    }
  UNDO_PREVIOUS_TOGGLES(i);
}

CHANGESTAT_FN(d_blktriangle)
{
  Edge e;
  Vertex tail, head, change, node3;
  int i, j,bi,bj,blockid,ninputs,nstats,lblk,lwb;
  double tailattr, edgemult;
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;
//Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */ 
  lblk=INPUT_PARAM[0];
  lwb =INPUT_PARAM[lblk+1];   
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
    	tail = TAIL(i);
	head = HEAD(i);
	for(j=1; j < lblk; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j+1-1;
		else if(tail <= INPUT_PARAM[1])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j+1-1;
		else if(head <= INPUT_PARAM[1])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;
//	blockid=calblockid(N_INPUT_PARAMS,INPUT_PARAM,tail,head,0);
	if(bi == bj){ /*the edge is within a block*/
		//Rprintf("bi %d, bj %d\n",bi,bj);
		edgemult = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
    		change = 0;
		if(lblk+lwb+2 < ninputs){ /* match on attributes */
        		tailattr = INPUT_ATTRIB[tail-1];
		        if(tailattr == INPUT_ATTRIB[head-1]){
        			STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
          				if(tailattr == INPUT_ATTRIB[node3-1]){
						/*test if node3 is within block "blocked"*/
						if((bi>0 && INPUT_PARAM[bi+1]>=node3 && INPUT_PARAM[bi+1-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+1])) 					
						{/*the node is within the block*/
            						if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
			                		else change += IS_UNDIRECTED_EDGE(node3,tail);
						}
          				}
        			}

	        		STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				        if(tailattr == INPUT_ATTRIB[node3-1]){
						if((bi>0 && INPUT_PARAM[bi+1]>=node3 && INPUT_PARAM[bi+1-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+1])) /*the node is within the block*/		{
			    			        if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
					        else change += IS_UNDIRECTED_EDGE(node3,tail);
						}
          				}
       				}
				for(j=0;j<lwb;j++)
				{
					if(blockid == INPUT_PARAM[j+lblk+2]-1)
					CHANGE_STAT[j] += edgemult * change;
				}
				//CHANGE_STAT[bi] += edgemult * change;
       		}
	 }else{ /* no attribute matching */
      		  STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
			//Rprintf("node3 %d\n",node3);
			if((bi>0 && INPUT_PARAM[bi+1]>=node3 && INPUT_PARAM[bi+1-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+1])) /*the node is within the block*/{
          	  	if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
		        else change += IS_UNDIRECTED_EDGE(node3,tail);
			}
            	  }
	          STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
			if((bi>0 && INPUT_PARAM[bi+1]>=node3 && INPUT_PARAM[bi+1-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+1])) /*the node is within the block*/{
        		if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
		        else change += IS_UNDIRECTED_EDGE(node3,tail);
			}
            	  }
		for(j=0;j<lwb;j++)
		{
			if(bi == INPUT_PARAM[j+lblk+2]-1)
				CHANGE_STAT[j] += edgemult * change;
		}
//	      	  CHANGE_STAT[bi] += edgemult * change;
    	 }
	}
        TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);	
}

CHANGESTAT_FN(d_blkkstar)
{
  double change, taild, headd=0.0;
  int edgeflag, i, j,k, kmo;
  Edge e;
  Vertex tail, head, node3;
  int ninputs, nstats;
  int bi,bj,blockid,lk,lblk,lwb;
  double tailattr;
    
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;
 // nbblk = (int)N_BBLK_PARAMS;
 // nother=ninputs-nbblk;
  lk=INPUT_PARAM[0];
  lblk=INPUT_PARAM[lk+1];
  lwb =INPUT_PARAM[lk+lblk+2];
 // Rprintf("ninputs: %d,lk:%d,lblk:%d\n",ninputs,lk,lblk);
//  Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
	tail = TAIL(i);
  	head = HEAD(i);
	for(j=lk+2; j < lk+lblk+1; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j-lk-1;
		else if(tail <= INPUT_PARAM[lk+2])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j-lk-1;
		else if(head <= INPUT_PARAM[lk+2])
			bj=0;
	}
	int ifact=0;
	for(int m=1;m<=bi+1;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;

//	blockid = calblockid(lblk,INPUT_PARAM,tail,head,lk+2);
	//Rprintf("blockid kstar %d\n",blockid);
	if(bi==bj){
//	Rprintf("bi %d, bj %d\n",bi,bj);
	
	if(ninputs>lk+lblk+lwb+3){/* match on attributes */
		edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      		tailattr = INPUT_ATTRIB[tail-1];
	        if(tailattr == INPUT_ATTRIB[head-1]){
		      	taild = - edgeflag;
		        STEP_THROUGH_OUTEDGES(tail, e, node3) {
				if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
		        		if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
	  			}
        		}
		        STEP_THROUGH_INEDGES(tail, e, node3) {
				if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
			        	if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
	  			}
			}
	        	headd = - edgeflag;
	        	STEP_THROUGH_OUTEDGES(head, e, node3) {
				if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
				          if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
				}
			}
		        STEP_THROUGH_INEDGES(head, e, node3) {
				if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
			        	if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
	  			}
			}
	        }
		for(j=1; j < lk+1; j++) {
			kmo = ((int)INPUT_PARAM[j]) - 1;
			change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
			CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
		}
	}else{ /*doesn't have to match the attribute*/
		  edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
		//Rprintf("tail %d, head %d\n",tail,head);
		//Rprintf("edgeflag %d\n",edgeflag);
		  taild = - edgeflag;
		  STEP_THROUGH_OUTEDGES(tail, e, node3) {
			if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
		        	++taild;
	  		}
        	  }
		  STEP_THROUGH_INEDGES(tail, e, node3) {
		  	if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
			        ++taild;
	  		}
		  }
	          headd = - edgeflag;
	          STEP_THROUGH_OUTEDGES(head, e, node3) {
			if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
				++headd;
			}
		  } 
		  STEP_THROUGH_INEDGES(head, e, node3) {
			if((bi>0 && INPUT_PARAM[bi+lk+2]>=node3 && INPUT_PARAM[bi+lk+2-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi+lk+2])){
			        ++headd;
	  		}
		  }
	//	Rprintf("outd tail %d, ind tail %d, outdo head %d, ind head %d\n",OUT_DEG[tail],IN_DEG[tail],OUT_DEG[head],IN_DEG[head]);	
      	//	  taild = OUT_DEG[tail] + IN_DEG[tail] - edgeflag; 
	//        headd = OUT_DEG[head] + IN_DEG[head] - edgeflag;
	          for(j=1; j < lk+1; j++) {
		  	kmo = ((int)INPUT_PARAM[j]) - 1;
			change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
			for(int jj = 0;jj<lwb;jj++)
			{
				if(bi==INPUT_PARAM[jj+lk+lblk+3]-1)
				{
					CHANGE_STAT[jj*lk+j-1] += (edgeflag ? -change:change);  
				}
			}
		//	CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
        	 }

	}
	}
	TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);
}


CHANGESTAT_FN(d_blktriangle2)
{
  Edge e;
  Vertex tail, head, change, change1,change2,node3;
  int i, j,bi,bj,bii,blockid,ninputs,nstats,lblk,lwb;
  double tailattr, edgemult;
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;
  lblk=INPUT_PARAM[0];
  lwb=INPUT_PARAM[lblk+1];
//Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
//	Rprintf("ntoggles %d\n",ntoggles);
  FOR_EACH_TOGGLE(i) {
    	tail = TAIL(i);
	head = HEAD(i);
	for(j=1; j < lblk; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j+1-1;
		else if(tail <= INPUT_PARAM[1])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j+1-1;
		else if(head <= INPUT_PARAM[1])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;
//	blockid=calblockid(ninputs,INPUT_PARAM,tail,head,0);
	change = 0;
	change1 = 0;
	change2 = 0;
//	Rprintf("bi %d, bj %d\n",bi,bj);
//	Rprintf("triangle blockid %d\n",blockid);
			
	edgemult = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
    		
	if(ninputs>lblk+lwb+2){ /* match on attributes */
       		tailattr = INPUT_ATTRIB[tail-1];
	        if(tailattr == INPUT_ATTRIB[head-1]){
			if(bi == bj){ /*the edge is within a block*/
        			STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
          				if(tailattr == INPUT_ATTRIB[node3-1]){
					/*test if node3 is within block "blocked"*/
        	    				if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
	        	        		else change += IS_UNDIRECTED_EDGE(node3,tail);
          				}
        			}

			        STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				        if(tailattr == INPUT_ATTRIB[node3-1]){
					    	if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
					        else change += IS_UNDIRECTED_EDGE(node3,tail);
          				}
        			}
				CHANGE_STAT[bi] += edgemult * change;
			}
			else{
				STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(ninputs,INPUT_PARAM,node3,0);
					if(bii==bi){
						if (DIRECTED) change1 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
					        else change1 += IS_UNDIRECTED_EDGE(node3,tail);
					}else if(bii==bj){
						if (DIRECTED) change2 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
					        else change2 += IS_UNDIRECTED_EDGE(node3,tail);			
					}
					}
				}
				STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(ninputs,INPUT_PARAM,node3,0);
					if(bii==bi){
						if (DIRECTED) change1 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
				       		else change1 += IS_UNDIRECTED_EDGE(node3,tail);
					}else if(bii==bj){
						if (DIRECTED) change2 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
			       			else change2 += IS_UNDIRECTED_EDGE(node3,tail);			
					}
					}
				}
				CHANGE_STAT[bi] += edgemult * change1;
				CHANGE_STAT[bj] += edgemult * change2;
			}
       		}
	 }else{ /* no attribute matching */
		if(bi==bj){
	      		STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
				if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
			        else change += IS_UNDIRECTED_EDGE(node3,tail);
            		}
		        STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				if (DIRECTED) change += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
			        else change += IS_UNDIRECTED_EDGE(node3,tail);
        	    	}
			for(j = 0; j<lwb;j++)
			{
				if(bi==INPUT_PARAM[j+lblk+2]-1)
				{
					CHANGE_STAT[j] += edgemult * change;
				}
			}
	//	      	CHANGE_STAT[bi] += edgemult * change;
		}
		else{ /*edge is between blocks*/
			STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
				bii=calblock(lblk,INPUT_PARAM,node3,1);
				if(bii==bi){
					if (DIRECTED) change1 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
				        else change1 += IS_UNDIRECTED_EDGE(node3,tail);
				}else if(bii==bj){
					if (DIRECTED) change2 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
				        else change2 += IS_UNDIRECTED_EDGE(node3,tail);			
				}
			}
			STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				bii=calblock(lblk,INPUT_PARAM,node3,1);
				if(bii==bi){
					if (DIRECTED) change1 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
		        		else change1 += IS_UNDIRECTED_EDGE(node3,tail);
				}else if(bii==bj){
					if (DIRECTED) change2 += IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail);
		        		else change2 += IS_UNDIRECTED_EDGE(node3,tail);			
				}
			}
			for(j = 0; j<lwb;j++)
			{
				if(bi==INPUT_PARAM[j+lblk+2]-1)
				{
					CHANGE_STAT[j] += edgemult * change1;
				}
				else if(bj==INPUT_PARAM[j+lblk+2]-1)
				{
					CHANGE_STAT[j] += edgemult * change2;
				}
			}
		//	CHANGE_STAT[bi] += edgemult * change1;
		//	CHANGE_STAT[bj] += edgemult * change2;		
		}
	}	
        TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);	
}

CHANGESTAT_FN(d_blkkstar2)
{
  double change, taild, headd,taild1,taild2,headd1,headd2;
  int edgeflag, i, j,k, kmo;
  Edge e;
  Vertex tail, head, node3;
  int ninputs, nstats;
  int bi,bj,blockid,lk,lblk,bii,lwb;
  double tailattr;
    
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;

  lk=INPUT_PARAM[0];
  lblk=INPUT_PARAM[lk+1];
  lwb= INPUT_PARAM[lk+lblk+2];
//  Rprintf("ninputs: %d,lk:%d,lblk:%d\n",ninputs,lk,lblk);
//  Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
	tail = TAIL(i);
  	head = HEAD(i);
	//blockid=calblockid(lblk,INPUT_PARAM,tail,head,lk+2);
	for(j=lk+2; j < lk+lblk+1; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j-lk-1;
		else if(tail <= INPUT_PARAM[lk+2])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j-lk-1;
		else if(head <= INPUT_PARAM[lk+2])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;
	
//	Rprintf("blockid kstar %d\n",blockid);
	
	if(ninputs>lk+lblk+lwb+3){/* match on attributes */
		edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      		tailattr = INPUT_ATTRIB[tail-1];
	        if(tailattr == INPUT_ATTRIB[head-1]){
			if(bi==bj){
		      	taild = - edgeflag;
		        STEP_THROUGH_OUTEDGES(tail, e, node3) {
				if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
        		}
		        STEP_THROUGH_INEDGES(tail, e, node3) {
				if(tailattr == INPUT_ATTRIB[node3-1]){++taild;}
			}
	        	headd = - edgeflag;
	        	STEP_THROUGH_OUTEDGES(head, e, node3) {
				if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
			}
		        STEP_THROUGH_INEDGES(head, e, node3) {
				if(tailattr == INPUT_ATTRIB[node3-1]){++headd;}
			}
			for(j=1; j < lk+1; j++) {
				kmo = ((int)INPUT_PARAM[j]) - 1;
				change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
				for(int jj=0;jj<lwb;j++)
				{
					if(bi == INPUT_PARAM[jj+lk+lblk+3]-1)
					CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
				}
			//	CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
			}
			}
			else{
				taild1 = - edgeflag;
				taild2 = - edgeflag;
				STEP_THROUGH_OUTEDGES(tail, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++taild1;
					else if(bii==bj) ++taild2;
					}
				}
				STEP_THROUGH_INEDGES(tail, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++taild1;
					else if(bii==bj) ++taild2;
					}
				}
				headd1 = -edgeflag;
				headd2 = -edgeflag;
				STEP_THROUGH_OUTEDGES(head, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++headd1;
					else if(bii==bj) ++headd2;
					}
				}
				STEP_THROUGH_INEDGES(head, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++headd1;
					else if(bii==bj) ++headd2;
					}
				}
				for(j=1; j < lk+1; j++) {
			  		kmo = ((int)INPUT_PARAM[j]) - 1;
					for(int jj=0;jj<lwb;jj++)
					{
						if(bi == INPUT_PARAM[jj+lk+lblk+3]-1){
							change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
							CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
						}
						else if(bj == INPUT_PARAM[jj+lk+lblk+3]-1){
							change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
							CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
						}
						
					}
			//		change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
			//		CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
			//		change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
			//		CHANGE_STAT[bj*lk+(j-1)] += (edgeflag ? -change:change);
        		 	}					
			}/*else*/
		}/*if(tailattr == INPUT_ATTRIB[head-1])*/
	}else{ /*doesn't have to match the attribute*/
		  edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
	//	Rprintf("edgeflag %d\n",edgeflag);
		if(bi==bj){
		  taild = - edgeflag;
		  STEP_THROUGH_OUTEDGES(tail, e, node3) {
			++taild;
        	  }
		  STEP_THROUGH_INEDGES(tail, e, node3) {
		  	++taild;
		  }
	          headd = - edgeflag;
	          STEP_THROUGH_OUTEDGES(head, e, node3) {
			++headd;
		  } 
		  STEP_THROUGH_INEDGES(head, e, node3) {
			++headd;
		  }
	//	Rprintf("taild %d\n",taild);
	          for(j=1; j < lk+1; j++) {
		  	kmo = ((int)INPUT_PARAM[j]) - 1;
			change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
			for(int jj=0;jj<lwb;jj++)
			{
				if(bi == INPUT_PARAM[jj+lk+lblk+3]-1)
				CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
			}
			//change = CHOOSE(taild, kmo) + CHOOSE(headd, kmo); 
			//CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
        	 }
		}/*(bi==bj)*/
		else{ /*bi != bj*/
			taild1 = - edgeflag;
			taild2 = - edgeflag;
			STEP_THROUGH_OUTEDGES(tail, e, node3) {
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
				if(bii==bi) ++taild1;
				else if(bii==bj) ++taild2;
			}
			STEP_THROUGH_INEDGES(tail, e, node3) {
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
				if(bii==bi) ++taild1;
				else if(bii==bj) ++taild2;
			}
			headd1 = -edgeflag;
			headd2 = -edgeflag;
			STEP_THROUGH_OUTEDGES(head, e, node3) {
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
				if(bii==bi) ++headd1;
				else if(bii==bj) ++headd2;
			}
			STEP_THROUGH_INEDGES(head, e, node3) {
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
				if(bii==bi) ++headd1;
				else if(bii==bj) ++headd2;
			}
			for(j=1; j < lk+1; j++) {
		  		kmo = ((int)INPUT_PARAM[j]) - 1;
				for(int jj=0;jj<lwb;jj++)
				{
					if(bi == INPUT_PARAM[jj+lk+lblk+3]-1){
						change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
						CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
					}
					else if(bj == INPUT_PARAM[jj+lk+lblk+3]-1){
						change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
						CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
					}
					
				}
		//		change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
		//		CHANGE_STAT[bi*lk+(j-1)] += (edgeflag ? -change:change);
		//		change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
		//		CHANGE_STAT[bj*lk+(j-1)] += (edgeflag ? -change:change);
        	 	}
		}/*bi != bj*/
	}/*match on attributes*/
	TOGGLE_IF_MORE_TO_COME(i);
  }/*for*/
  UNDO_PREVIOUS_TOGGLES(i);
}


CHANGESTAT_FN(d_blktriangle3)
{
  Edge e;
  Vertex tail, head, node3;
  int i, j,bi,bj,bii,blockid,blockid1,ninputs,nstats;
  double tailattr, edgemult;
  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;
//Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
//	Rprintf("ntoggles %d\n",ntoggles);
  FOR_EACH_TOGGLE(i) {
    	tail = TAIL(i);
	head = HEAD(i);

	for(j=0; j < N_INPUT_PARAMS-1; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j+1;
		else if(tail <= INPUT_PARAM[0])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j+1;
		else if(head <= INPUT_PARAM[0])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*N_INPUT_PARAMS + bj - ifact;
//	Rprintf("bi %d, bj %d\n",bi,bj);
//	Rprintf("triangle blockid %d\n",blockid);
			
	edgemult = IS_OUTEDGE(tail, head) ? -1.0 : 1.0;
    		
	if(ninputs>nstats){ /* match on attributes */
       		tailattr = INPUT_ATTRIB[tail-1];
	        if(tailattr == INPUT_ATTRIB[head-1]){
			if(bi==bj){
	      			STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					if((bi>0 && INPUT_PARAM[bi]>=node3 && INPUT_PARAM[bi-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi])){
						if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        		else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
					else{
						/*calculate it is in which block*/
						blockid1 = calblockid(ninputs,INPUT_PARAM,node3,tail,0);
						if (DIRECTED) CHANGE_STAT[blockid1] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        		else CHANGE_STAT[blockid1] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
					}
	            		}
			        STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					if((bi>0 && INPUT_PARAM[bi]>=node3 && INPUT_PARAM[bi-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi])){
						if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        		else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
					else{
						/*calculate it is in which block*/
						blockid1 = calblockid(ninputs,INPUT_PARAM,node3,tail,0);
						if (DIRECTED) CHANGE_STAT[blockid1] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        		else CHANGE_STAT[blockid1] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
					}
        	    		}
			}
			else{ /*edge is between blocks*/
				
				STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        	else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
				}
				STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
					if(tailattr == INPUT_ATTRIB[node3-1]){
					if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
				        else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
					}
				}				
			}
       		}
	 }else{ /* no attribute matching */
		if(bi==bj){
	      		STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
				if((bi>0 && INPUT_PARAM[bi]>=node3 && INPUT_PARAM[bi-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi])){
					if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        	else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
				}
				else{
					/*calculate it is in which block*/
					blockid1 = calblockid(ninputs,INPUT_PARAM,node3,tail,0);
					if (DIRECTED) CHANGE_STAT[blockid1] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        	else CHANGE_STAT[blockid1] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
				}
            		}
		        STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				if((bi>0 && INPUT_PARAM[bi]>=node3 && INPUT_PARAM[bi-1]<node3) ||(bi==0 && node3 <= INPUT_PARAM[bi])){
					if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        	else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
				}
				else{
					/*calculate it is in which block*/
					blockid1 = calblockid(ninputs,INPUT_PARAM,node3,tail,0);
					if (DIRECTED) CHANGE_STAT[blockid1] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        	else CHANGE_STAT[blockid1] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
				}
        	    	}
		}
		else{ /*edge is between blocks*/
			STEP_THROUGH_OUTEDGES(head, e, node3) { /* step through outedges of head */
				if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
			}
			STEP_THROUGH_INEDGES(head, e, node3) { /* step through inedges of head */
				if (DIRECTED) CHANGE_STAT[blockid] += edgemult*(IS_OUTEDGE(node3, tail) + IS_INEDGE(node3, tail));
			        else CHANGE_STAT[blockid] += edgemult*IS_UNDIRECTED_EDGE(node3,tail);
			}		
		}
	}	
        TOGGLE_IF_MORE_TO_COME(i);
  }
  UNDO_PREVIOUS_TOGGLES(i);	
}

CHANGESTAT_FN(d_blkkstar3)
{
  double change, taild, headd,taild1,taild2,headd1,headd2;
  int edgeflag, i, j,k, kmo;
  Edge e;
  Vertex tail, head, node3;
  int ninputs, nstats;
  int bi,bj,blockid,lk,lblk,bii,lwb;
  double tailattr;
  //int *blockdegreet,*blockdegreeh;

  ninputs = (int)N_INPUT_PARAMS;
  nstats  = (int)N_CHANGE_STATS;

  lk=INPUT_PARAM[0];
  lblk=INPUT_PARAM[lk+1];
  lwb = INPUT_PARAM[lk+lblk+2];
 // Rprintf("ninputs: %d,lk:%d,lblk:%d\n",ninputs,lk,lblk);
//  Rprintf("input[0]:%f,input[1]:%f,input[2]:%f,input[3]:%f,input[4]:%f,input[5]:%f\n",INPUT_PARAM[0],INPUT_PARAM[1],INPUT_PARAM[2],INPUT_PARAM[3],INPUT_PARAM[4],INPUT_PARAM[5]);
  /* *** don't forget tail -> head */    
 // blockdegreet = (int *) malloc(sizeof(int) * lblk*(lblk-1)/2);
 // blockdegreeh = (int *) malloc(sizeof(int) * lblk*(lblk-1)/2);
  ZERO_ALL_CHANGESTATS(i);
  FOR_EACH_TOGGLE(i) {
	tail = TAIL(i);
  	head = HEAD(i);

	for(j=lk+2; j < lk+lblk+1; j++)
	{
		if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
			bi = j-lk-1;
		else if(tail <= INPUT_PARAM[lk+2])
			bi = 0;
		if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
			bj = j-lk-1;
		else if(tail <= INPUT_PARAM[lk+2])
			bj=0;
	}
//	int ifact=0;
//	for(int m=1;m<=bi+1;m++)
//	{
//		ifact+=m;
//	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*lblk + bj - ifact;
//	blockid=calblockid(lblk,INPUT_PARAM,tail,head,lk+2);
//	Rprintf("blockid kstar %d\n",blockid);
	
	if(ninputs>lk+lblk+lwb+3){/* match on attributes */
		edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
      		tailattr = INPUT_ATTRIB[tail-1];
	        if(tailattr == INPUT_ATTRIB[head-1]){
			if(bi!=bj){
				taild1 = - edgeflag;
				STEP_THROUGH_OUTEDGES(tail, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bj) ++taild1;
					}
				}
				STEP_THROUGH_INEDGES(tail, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bj) ++taild1;
					}
				}
				headd1 = -edgeflag;
				STEP_THROUGH_OUTEDGES(head, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++headd1;
					}
				}
				STEP_THROUGH_INEDGES(head, e, node3) {
					if(tailattr == INPUT_ATTRIB[node3-1]){
					bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
					if(bii==bi) ++headd1;
					}
				}
				for(j=1; j < lk+1; j++) {
			  		kmo = ((int)INPUT_PARAM[j]) - 1;
					change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
					for(int jj=0;jj<lwb;jj++)
					{
						if(blockid == INPUT_PARAM[jj+lk+lblk+3]-1)
							CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
					}	
				//	CHANGE_STAT[blockid*lk+(j-1)] += (edgeflag ? -change:change);
					//change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
					//CHANGE_STAT[bj*lk+(j-1)] += (edgeflag ? -change:change);
        		 	}					
			}/*else*/
		}/*if(tailattr == INPUT_ATTRIB[head-1])*/
	}else{ /*doesn't have to match the attribute*/
		  edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
	//	Rprintf("tail %d, head %d\n",tail,head);
		if(bi!=bj){ /*bi != bj*/
		//	Rprintf("edgeflag %d\n",edgeflag);
			taild1 = - edgeflag;
			taild2 = - edgeflag;
			STEP_THROUGH_OUTEDGES(tail, e, node3) {
			//	Rprintf("out-tail node3 %d->%d\n",tail,node3);
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
			//	Rprintf("bii %d\n",bii);
				if(bii==bj) {++taild1;
			//	Rprintf("node3 %d,tail %d\n",node3,tail);
				}
			}
			STEP_THROUGH_INEDGES(tail, e, node3) {
			//	Rprintf("in-tail node3 %d->%d\n",node3,tail);
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
			//	Rprintf("bii %d\n",bii);
				if(bii==bj) {++taild1;
			//		Rprintf("node3 %d,tail %d\n",node3,tail);
				}
			}
			headd1 = -edgeflag;
			headd2 = -edgeflag;
			STEP_THROUGH_OUTEDGES(head, e, node3) {
			//	Rprintf("out-head node3 %d->%d\n",head,node3);
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
			//	Rprintf("bii %d\n",bii);
				if(bii==bi) {++headd1;
			//		Rprintf("node3 %d,tail %d\n",node3,tail);
				}
			}
			STEP_THROUGH_INEDGES(head, e, node3) {
			//	Rprintf("in-head node3 %d->%d\n",node3,head);
				bii=calblock(lblk,INPUT_PARAM,node3,lk+2);
			//	Rprintf("bii %d\n",bii);
				if(bii==bi) {++headd1;
			//		Rprintf("node3 %d,tail %d\n",node3,tail);
				}
			}
			//Rprintf("headd1 %f, taild1 %f\n",headd1,taild1);
			for(j=1; j < lk+1; j++) {
		  		kmo = ((int)INPUT_PARAM[j]) - 1;
				change = CHOOSE(taild1, kmo) + CHOOSE(headd1, kmo); 
				for(int jj=0;jj<lwb;jj++)
				{
					if(blockid == INPUT_PARAM[jj+lk+lblk+3]-1)
						CHANGE_STAT[jj*lk+(j-1)] += (edgeflag ? -change:change);
				}				
			//	CHANGE_STAT[blockid*lk+(j-1)] += (edgeflag ? -change:change);
				//change = CHOOSE(taild2, kmo) + CHOOSE(headd2, kmo); 
				//CHANGE_STAT[blockid*lk+(j-1)] += (edgeflag ? -change:change);
        	 	}
		}/*bi != bj*/
	}/*match on attributes*/
	TOGGLE_IF_MORE_TO_COME(i);
  }/*for*/
  UNDO_PREVIOUS_TOGGLES(i);
}

CHANGESTAT_FN(d_degseq)
{
  int edgeflag,i;
  Vertex tail, head;
  
  /* *** don't forget tail -> head */    
  ZERO_ALL_CHANGESTATS(i);
	//Rprintf("INPUTPARAMS,%d\n",N_INPUT_PARAMS);
  for (i=0; i < ntoggles; i++)
    {
	edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));	
	CHANGE_STAT[tail-1] += (edgeflag ? - 1 : 1); 
	CHANGE_STAT[head-1] += (edgeflag ? - 1 : 1); 
        TOGGLE_IF_MORE_TO_COME(i);
    }
  UNDO_PREVIOUS_TOGGLES(i);
}

CHANGESTAT_FN(d_blkdegseq)
{
	int edgeflag, i,j,bi,bj,blockid,lblk,lwb,firste;
  	Vertex tail, head;
	  /* *** don't forget tail -> head */    
	lblk=INPUT_PARAM[0];
	lwb = INPUT_PARAM[lblk+1];
	firste = INPUT_PARAM[(int)INPUT_PARAM[lblk+2]]+1;
	//laste = INPUT_PARAM[(int)INPUT_PARAM[lblk+2]+1];
	  ZERO_ALL_CHANGESTATS(i);
	//Rprintf("INPUTPARAMS,%d\n",N_INPUT_PARAMS);
	for (i=0; i < ntoggles; i++)
    	{
		edgeflag = IS_OUTEDGE(tail = TAIL(i), head = HEAD(i));
		tail=TAIL(i);
		head=HEAD(i);
		for(j=2; j < lblk; j++)
		{
			if(tail > INPUT_PARAM[j] && tail <= INPUT_PARAM[j+1])
				bi = j-2+1;
			else if(tail <= INPUT_PARAM[2])
				bi = 0;
			if(head > INPUT_PARAM[j] && head <= INPUT_PARAM[j+1])
				bj = j-2+1;
			else if(head <= INPUT_PARAM[2])
				bj=0;
		}
		int ifact=0;
		for(int m=0;m<=bi;m++)
		{
			ifact+=m;
		}
		blockid = bi*lblk + bj - ifact;
		//Rprintf("tail %d, head %d\n",TAIL(i),HEAD(i));
		//Rprintf("blockid %d\n",blockid); 
		if(bi==bj && bi == (INPUT_PARAM[lblk+2]-1))
		{
			CHANGE_STAT[tail-firste] += edgeflag ? - 1 : 1;
			CHANGE_STAT[head-firste] += edgeflag ? - 1 : 1;		
		}
        	TOGGLE_IF_MORE_TO_COME(i);
    	}
  UNDO_PREVIOUS_TOGGLES(i);
}


/*given two nodes, calculate the blocked that the edge is within*/
int calblockid(int npara,double *para,int node1,int node2,int offset)
{
	int j,bi,bj,blockid;
	//Rprintf("para[0] %f,[1] %f,[2] %f,[3] %f,[4] %f,[5] %f,[6] %f,[7] %f\n",para[0],para[1],para[2],para[3],para[4],para[5],para[6]);
	for(j=offset; j < offset+npara-1; j++)
	{
		if(node1 > para[j] && node1 <= para[j+1])
			bi = j-offset+1;
		else if(node1 <= para[offset])
			bi = 0;
		if(node2 > para[j] && node2 <= para[j+1])
			bj = j-offset+1;
		else if(node2 <= para[offset])
			bj=0;
	}
	int ifact=0;
	for(int m=0;m<=bi;m++)
	{
		ifact+=m;
	}
	blockid = bi*npara + bj - ifact;
	return(blockid);
}
/*calculate which block this node is belonging to*/
int calblock(int npara,double *para,int node,int offset)
{
	int j,bi;
	for(j=offset; j < offset+npara-1; j++)
	{
		if(node > para[j] && node <= para[j+1])
			bi = j-offset+1;
		else if(node <= para[offset])
			bi = 0;
	}
	return(bi);
}



double my_choose(double n, int r) {
  const double recip_factorial[21] = {1.0, 1.0, 0.5,
	 1.66666666666667e-01, 4.16666666666667e-02, 8.33333333333333e-03,
         1.38888888888889e-03, 1.98412698412698e-04, 2.48015873015873e-05,
         2.75573192239859e-06, 2.75573192239859e-07, 2.50521083854417e-08,
         2.08767569878681e-09, 1.60590438368216e-10, 1.14707455977297e-11,
         7.64716373181982e-13, 4.77947733238739e-14, 2.81145725434552e-15,
         1.56192069685862e-16, 8.22063524662433e-18, 4.11031762331216e-19};
  double ans;

  if (r>20)
    return choose (n, (double)r); /* Use complicated function for large r */
  for(ans=recip_factorial[r]; r>0; r--)
    ans*=(n--);
  return ans;
}

