#include "ran.h"

void psdes( Uint &lword, Uint &rword )
{
	//Pseudo-DES hashing of the 64-bit word ( lword, rword). Both 32-bit arguments are returned
	//hashed on all bits.
	const int NITER = 2;
	static const Uint c1[4] = { 0xbaa96887L, 0x1e17d32cL, 0x03bcdc3cL, 0x0f33d1b2L };
	static const Uint c2[4] = { 0x4b0f3b58L, 0xe874f0c3L, 0x6955c5a6L, 0x55a7ca46L };
	Uint i, ia, ib, iswap, itmph = 0, itmpl = 0;
	for ( i  = 0; i < NITER; i++ )
	{
		//Perform niter iterations of DES logic, using a simpler (noncryptographic) nonlinear function
		//instead of DES's.
		ia = ( iswap = rword ) ^ c1[i];  //The bit-rich constants c1 and (below) c2 guarantee lots of nonlinear mixing.
		itmpl = ia & 0xffff;
		itmph = ia >> 16;
		ib = itmpl * itmpl + ~(itmph*itmph);
		rword = lword ^ ((( ia = (ib>>16) | ((ib&0xffff) << 16))^c2[i])+itmpl*itmph);
		lword = iswap;
	}

}

void hashall( VecUint &arr )
{
	//Replace the array arr by a same-sized hash, all of whose bits depend on all of whose bits depend on all of the bits in arr.
	//Use psdes for the mutual hash of two 32-bit words.
	Int m = arr.size(), n = m - 1;
	n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16; n++;
	//Incredibly, n is now the next power of 2 >= m.
	Int nb = n, nb2 = n >> 1, j, jb;
	if ( n < 2 )
	{
		throw("size must be > 1");
	}
	while ( nb > 1 )
	{
		for ( jb = 0; jb < n - nb + 1; jb += nb )
		{
			for ( j = 0; j < nb2; j++ )
			{
				if ( jb + j + nb2 < m )
				{
					psdes( arr[jb+j], arr[jb+j+nb2] );
				}
			}
		}
		nb = nb2;
		nb2 >>= 1;
	}
	nb2 = n >> 1;
	if ( m != n )
	{
		for ( j = nb2; j < m; j++ )
		{
			psdes( arr[j], arr[j-nb2] );
		}
	}
	//Final mix needed only if m is not a power of 2.

}
