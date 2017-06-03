#ifndef QX_MEMORY_H
#define QX_MEMORY_H

#define QX_DEF_PADDING					10

/*memory*/
inline double *** qx_allocd_3(int n,int r,int c,int padding=QX_DEF_PADDING)
{
    double *a,**p,***pp;
    int rc=r*c;
    int i,j;
    a=(double*) malloc(sizeof(double)*(n*rc+padding));
    if(a==NULL) {printf("qx_allocd_3() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(double**) malloc(sizeof(double*)*n*r);
    pp=(double***) malloc(sizeof(double**)*n);
    for(i=0;i<n;i++)
        for(j=0;j<r;j++)
            p[i*r+j]=&a[i*rc+j*c];
    for(i=0;i<n;i++)
        pp[i]=&p[i*r];
    return(pp);
}
inline void qx_freed_3(double ***p)
{
    if(p!=NULL)
    {
        free(p[0][0]);
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline unsigned char** qx_allocu(int r,int c,int padding=QX_DEF_PADDING)
{
    unsigned char *a,**p;
    a=(unsigned char*) malloc(sizeof(unsigned char)*(r*c+padding));
    if(a==NULL) {printf("qx_allocu() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(unsigned char**) malloc(sizeof(unsigned char*)*r);
    for(int i=0;i<r;i++) p[i]= &a[i*c];
    return(p);
}
inline void qx_freeu(unsigned char **p)
{
    if(p!=NULL)
    {
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline unsigned char *** qx_allocu_3(int n,int r,int c,int padding=QX_DEF_PADDING)
{
    unsigned char *a,**p,***pp;
    int rc=r*c;
    int i,j;
    a=(unsigned char*) malloc(sizeof(unsigned char )*(n*rc+padding));
    if(a==NULL) {printf("qx_allocu_3() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(unsigned char**) malloc(sizeof(unsigned char*)*n*r);
    pp=(unsigned char***) malloc(sizeof(unsigned char**)*n);
    for(i=0;i<n;i++)
        for(j=0;j<r;j++)
            p[i*r+j]=&a[i*rc+j*c];
    for(i=0;i<n;i++)
        pp[i]=&p[i*r];
    return(pp);
}
inline void qx_freeu_3(unsigned char ***p)
{
    if(p!=NULL)
    {
        free(p[0][0]);
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline void qx_freeu_1(unsigned char*p)
{
    if(p!=NULL)
    {
        delete [] p;
        p=NULL;
    }
}
inline float** qx_allocf(int r,int c,int padding=QX_DEF_PADDING)
{
    float *a,**p;
    a=(float*) malloc(sizeof(float)*(r*c+padding));
    if(a==NULL) {printf("qx_allocf() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(float**) malloc(sizeof(float*)*r);
    for(int i=0;i<r;i++) p[i]= &a[i*c];
    return(p);
}
inline void qx_freef(float **p)
{
    if(p!=NULL)
    {
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline float *** qx_allocf_3(int n,int r,int c,int padding=QX_DEF_PADDING)
{
    float *a,**p,***pp;
    int rc=r*c;
    int i,j;
    a=(float*) malloc(sizeof(float)*(n*rc+padding));
    if(a==NULL) {printf("qx_allocf_3() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(float**) malloc(sizeof(float*)*n*r);
    pp=(float***) malloc(sizeof(float**)*n);
    for(i=0;i<n;i++)
        for(j=0;j<r;j++)
            p[i*r+j]=&a[i*rc+j*c];
    for(i=0;i<n;i++)
        pp[i]=&p[i*r];
    return(pp);
}
inline void qx_freef_3(float ***p)
{
    if(p!=NULL)
    {
        free(p[0][0]);
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline int** qx_alloci(int r,int c,int padding=QX_DEF_PADDING)
{
    int *a,**p;
    a=(int*) malloc(sizeof(int)*(r*c+padding));
    if(a==NULL) {printf("qx_alloci() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(int**) malloc(sizeof(int*)*r);
    for(int i=0;i<r;i++) p[i]= &a[i*c];
    return(p);
}
inline void qx_freei(int **p)
{
    if(p!=NULL)
    {
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline void qx_freei_1(int*p)
{
    if(p!=NULL)
    {
        delete [] p;
        p=NULL;
    }
}
inline double** qx_allocd(int r,int c,int padding=QX_DEF_PADDING)
{
    double *a,**p;
    a=(double*) malloc(sizeof(double)*(r*c+padding));
    if(a==NULL) {printf("qx_allocd() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(double**) malloc(sizeof(double*)*r);
    for(int i=0;i<r;i++) p[i]= &a[i*c];
    return(p);
}
inline void qx_freed(double **p)
{
    if(p!=NULL)
    {
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline unsigned char**** qx_allocu_4(int t,int n,int r,int c,int padding=QX_DEF_PADDING)
{
    unsigned char *a,**p,***pp,****ppp;
    int nrc=n*r*c,nr=n*r,rc=r*c;
    int i,j,k;
    a=(unsigned char*) malloc(sizeof(unsigned char)*(t*nrc+padding));
    if(a==NULL) {printf("qx_allocu_4() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(unsigned char**) malloc(sizeof(unsigned char*)*t*nr);
    pp=(unsigned char***) malloc(sizeof(unsigned char**)*t*n);
    ppp=(unsigned char****) malloc(sizeof(unsigned char***)*t);
    for(k=0;k<t;k++)
        for(i=0;i<n;i++)
            for(j=0;j<r;j++)
                p[k*nr+i*r+j]=&a[k*nrc+i*rc+j*c];
    for(k=0;k<t;k++)
        for(i=0;i<n;i++)
            pp[k*n+i]=&p[k*nr+i*r];
    for(k=0;k<t;k++)
        ppp[k]=&pp[k*n];
    return(ppp);
}
inline void qx_freeu_4(unsigned char ****p)
{
    if(p!=NULL)
    {
        free(p[0][0][0]);
        free(p[0][0]);
        free(p[0]);
        free(p);
        p=NULL;
    }
}
inline double**** qx_allocd_4(int t,int n,int r,int c,int padding=QX_DEF_PADDING)
{
    double *a,**p,***pp,****ppp;
    int nrc=n*r*c,nr=n*r,rc=r*c;
    int i,j,k;
    a=(double*) malloc(sizeof(double)*(t*nrc+padding));
    if(a==NULL) {printf("qx_allocd_4() fail, Memory is too huge, fail.\n"); getchar(); exit(0); }
    p=(double**) malloc(sizeof(double*)*t*nr);
    pp=(double***) malloc(sizeof(double**)*t*n);
    ppp=(double****) malloc(sizeof(double***)*t);
    for(k=0;k<t;k++)
        for(i=0;i<n;i++)
            for(j=0;j<r;j++)
                p[k*nr+i*r+j]=&a[k*nrc+i*rc+j*c];
    for(k=0;k<t;k++)
        for(i=0;i<n;i++)
            pp[k*n+i]=&p[k*nr+i*r];
    for(k=0;k<t;k++)
        ppp[k]=&pp[k*n];
    return(ppp);
}
inline void qx_freed_4(double ****p)
{

    if(p!=NULL)
    {
        free(p[0][0][0]);
        free(p[0][0]);
        free(p[0]);
        free(p);
        p=NULL;
    }
}

inline void qx_memcpy_u3(unsigned char a[3],unsigned char b[3]){*a++=*b++; *a++=*b++; *a++=*b++;}
inline void image_copy(double***out,double***in,int h,int w,int d){memcpy(out[0][0],in[0][0],sizeof(double)*h*w*d);}
inline void image_copy(unsigned char**out,unsigned char**in,int h,int w){memcpy(out[0],in[0],sizeof(unsigned char)*h*w);}

#endif // QX_MEMORY_H
