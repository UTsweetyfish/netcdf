#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "netcdf.h"

#undef DEBUG

static void
nccheck(int ret)
{
    if(ret == NC_NOERR) return;
    fprintf(stderr,"err=%s\n",nc_strerror(ret));
    exit(1);
}

#define NCCHECK(err) nccheck(err)

int
main(int argc, char *argv[] )
{
    int   err, ncid, varid, dimid[1];
    size_t  dimlen[1];
    float   *fdat;
    int     *idat;
    const char* filename = "tmp_fillonly.nc";
    const char* varname = "f";
    const char* dimname = "x";
    size_t i;

    NCCHECK(err = nc_open(filename,NC_NETCDF4,&ncid));
    NCCHECK(err = nc_inq_varid(ncid, varname, &varid));
    NCCHECK(err = nc_inq_dimid(ncid, dimname, dimid));
    NCCHECK(err = nc_inq_dim(ncid, dimid[0], NULL, dimlen));

    /* Make room for both double and floating dat */
    fdat = (float  *)calloc(1,sizeof(float) * dimlen[0]);
    idat = (int  *)calloc(1,sizeof(int) * dimlen[0]);

    NCCHECK(err = nc_get_var_int(ncid, varid, idat));
    NCCHECK(err = nc_get_var_float(ncid, varid, fdat));

#ifdef DEBUG
    printf("int[0..%d]:",(int)dimlen[0]);
    for(i=0; i<dimlen[0]; i++ ) printf(" %i", idat[i]);
    printf("\n");

    printf("float[0..%d]:",(int)dimlen[0]);
    for(i=0; i<dimlen[0]; i++ ) printf(" %f", fdat[i]);
    printf("\n");
#endif

    /* Do the comparisons */
    for(i=0; i<dimlen[0]; i++ ) {
	if(fdat[i] != (float)(idat[i])) {
	    fprintf(stderr,"data mismatch [%d]: float=%f (float)int=%f int=%i\n",(int)i,fdat[i],(float)idat[i],idat[i]);
	    return 1;
	}
    }

    if(fdat) free(fdat);
    if(idat) free(idat);
    return 0;

}













