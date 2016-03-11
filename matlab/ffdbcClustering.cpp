#include "mex.h"
#include <queue>
#include <cstring>
#include <chrono>
#include <cstdint>

#include <FFDBC.h>
#include <common.h>
#include <QList>


using namespace std;
using namespace chrono;

void entryPoint(const mxArray *x, double *classes, double *types, double k, double gamma, size_t numPoints, size_t dim)
{
	// load input points
	double *inPoints = mxGetPr(x);

	QList<Point*>* points = new QList<Point*>();
	int idx = 0;
	for(int k=0; k < numPoints; k++)
	{
		Point* p = new Point();
        p->id = k;
        p->clusterID = -1;
        p->desiredClusterID = "";

        for(int i=0; i < dim; i++)
        {
            p->coords.append(inPoints[idx + i]);
        }
		idx += dim;
		points->append(p);
	}

	// run FFDBC
	FFDBC* ffdbc = new FFDBC(k, gamma, points);
	ffdbc->doWorkSynch();
	
	// copy output classes
	for(int k=0; k < points->size(); k++)
	{
		classes[k] = points->at(k)->clusterID;

		delete points->at(k);
	}

	// cleanup
	delete ffdbc;
	delete points;
}


void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  double *classes, *types;
  double  k,gamma;
  size_t dim,numPoints;
  
  if(nrhs!=3) 
    mexErrMsgIdAndTxt( "MATLAB:xtimesy:invalidNumInputs",
            "Three inputs required.");
  if(nlhs!=2) 
    mexErrMsgIdAndTxt( "MATLAB:xtimesy:invalidNumOutputs",
            "Two output required.");
  
  /* check to make sure the second input argument is a scalar */
  if( !mxIsDouble(prhs[1]) || mxGetN(prhs[1])*mxGetM(prhs[1])!=1 ) {
    mexErrMsgIdAndTxt( "MATLAB:xtimesy:xNotScalar",
            "Input k must be a scalar.");
  }
  
  /* check to make sure the third input argument is a scalar */
  if( !mxIsDouble(prhs[2]) || mxGetN(prhs[2])*mxGetM(prhs[2])!=1 ) {
    mexErrMsgIdAndTxt( "MATLAB:xtimesy:xNotScalar",
            "Input k must be a scalar.");
  }
  
  /*  get the scalar input k */
  k = mxGetScalar(prhs[1]);
  
  /*  get the scalar input gamma */
  gamma = mxGetScalar(prhs[2]);
  
  /*  get the dimensions of the matrix input x */
  numPoints = mxGetN(prhs[0]);
  dim = mxGetM(prhs[0]);
  
  /*  set the output pointer to the output matrix */
  plhs[0] = mxCreateDoubleMatrix( (mwSize)numPoints, (mwSize)1, mxREAL);
  
  /*  create a C pointer to a copy of the output matrix */
  classes = mxGetPr(plhs[0]);
  
  /*  set the output pointer to the output matrix */
  plhs[1] = mxCreateDoubleMatrix( (mwSize)numPoints, (mwSize)1, mxREAL);
  
  /*  create a C pointer to a copy of the output matrix */
  types = mxGetPr(plhs[1]);
  
  /*  call the C subroutine */
  entryPoint(prhs[0],classes,types,k,gamma,numPoints,dim);  
}
