/*****************************************************************************
   Major portions of this software are copyrighted by the Medical College
   of Wisconsin, 1994-2000, and are released under the Gnu General Public
   License, Version 2.  See the file README.Copyright for details.
******************************************************************************/

#include "mrilib.h"

/*** Not 7D safe ***/

/*-------------------------------------------------------------------*/
/*! Cut out sub-image [xa..xb,ya..yb] from the input.  Returns NULL
   if inputs are incoherent.  RWCox -- 13 April 1999.
---------------------------------------------------------------------*/

MRI_IMAGE * mri_cut_2D( MRI_IMAGE * im , int xa, int xb, int ya, int yb )
{
   char * par , * qar , * irow , * orow ;
   MRI_IMAGE * qim ;
   int qx,qy , ps , xx,yy , nx, xps ;

   /*-- sanity checks --*/

   if( im == NULL || xa < 0 || xb >= im->nx || xb < xa ||
                     ya < 0 || yb >= im->ny || yb < ya   ) return NULL ;

   /*-- pointer to input data --*/

   par = (char *) mri_data_pointer( im ) ;
   if( par == NULL ) return NULL ;          /* sanity check */

   /*-- make output image --*/

   qx = xb - xa + 1 ; qy = yb - ya + 1 ;    /* dimensions */
   qim = mri_new( qx , qy , im->kind ) ;    /* creation */

   ps = im->pixel_size ; xps = qx * ps ;    /* xps = size of output row */
   qar = (char *) mri_data_pointer( qim ) ; /* ptr to output data */
   nx = im->nx ;

   for( yy=ya ; yy <= yb ; yy++ ){          /* for each output row */
      irow = par + (yy*nx+xa)*ps ;          /* ptr to input */
      orow = qar + (yy-ya)*xps ;            /* ptr to output */
      memcpy( orow , irow , xps ) ;         /* copy input to output */
   }

   return qim ;
}

/*-------------------------------------------------------------------*/
/*! Cut out sub-image [xa..xb,ya..yb,za..zb] from the input.
   Returns NULL if inputs are incoherent.  RWCox -- 06 Jun 2002.
---------------------------------------------------------------------*/

MRI_IMAGE * mri_cut_3D( MRI_IMAGE * im ,
                        int xa, int xb, int ya, int yb, int za, int zb )
{
   char * par , * qar , * irow , * orow ;
   MRI_IMAGE * qim ;
   int qx,qy,qz, ps , xx,yy,zz , nx,nxy , xps ;

   /*-- sanity checks --*/

   if( im == NULL || xa < 0 || xb >= im->nx || xb < xa ||
                     ya < 0 || yb >= im->ny || yb < ya ||
                     za < 0 || zb >= im->nz || zb < za   ) return NULL ;

   /*-- pointer to input data --*/

   par = (char *) mri_data_pointer( im ) ;
   if( par == NULL ) return NULL ;          /* sanity check */

   /*-- make output image --*/

   qx = xb - xa + 1; qy = yb - ya + 1; qz = zb - za + 1; /* dimensions */
   qim = mri_new_vol( qx , qy , qz , im->kind ) ;        /* creation */

   ps  = im->pixel_size ; xps = qx * ps ;    /* xps = size of output row */
   qar = (char *) mri_data_pointer( qim ) ;  /* ptr to output data */
   nx  = im->nx ; nxy = nx * im->ny ;

   for( zz=za ; zz <= zb ; zz++ ){             /* for each ouput plane */
     for( yy=ya ; yy <= yb ; yy++ ){           /* for each output row */
       irow = par + (zz*nxy+yy*nx+xa)*ps ;     /* ptr to input row */
       orow = qar + ((zz-za)*qy+(yy-ya))*xps ; /* ptr to output row */
       memcpy( orow , irow , xps ) ;           /* copy input to output */
     }
   }

   return qim ;
}
