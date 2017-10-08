#ifndef HASH_H
#define HASH_H

#include "ran.h"

struct Hashfn1
{
	//Example of an object encapsulating a hash function for use by the class Hashmap.
	Ranhash hasher;   //The actual hash function.
	Int n;            //Size of key in bytes.
	Hashfn1(Int nn):n(nn){} //Constructor just saves key size.
	
	inline Ullong fn( const void *key )//Function that returns hash from key.
	{
		Uint *k;
		Ullong *kk;
		switch(n)
		{
		case 4:
			k = (Uint *)key;
			return hasher.int64(*k);   //Return 64-bit hash of 32-bit key.
		case 8:
			kk = (Ullong *)key;
			return hasher.int64(*kk);  //Return 64-bit hash of 64-bit key.
		default:
			throw( "Hashfn1 is for 4 or 8 byte keys only." );

		}
	}
};

struct Hashfn2
{
	//Another example of an object encapsulating a hash function, allowing arbitrary fixed key sizes
	//or variable-length null terminated strings. The hash function algorithm is self-contained.
	static Ullong hashfn_tab[256];
	Ullong h;
	Int n;   //Size of key in bytes, when fixed size.
	
	inline Hashfn2( Int nn ) : n(nn)
	{
		if ( n == 1 )
		{
			n = 0;  //Null terminated string key signaled by n == 0
		}
		h = 0x544B2FBACAAF1684LL; //or 1.
		for ( Int j = 0; j < 256; j++ )
		{
			//Length 256 lookup table is initialized with values from 
			//a 64-bit Marsaglia generator stepped 31 times between each.
			for ( Int i = 0; i < 31; i++ )
			{
				h = ( h >> 7 ) ^ h;
				h = ( h << 11 ) ^ h;
				h = ( h >> 10 ) ^ h;
			}
			hashfn_tab[j] = h;
		}
	}

	inline Ullong fn( const void *key )
	{
		//Function that returns hash from key.
		Int j;
		char *k = (char*)key;    //Cast the key pointer to char pointer.
		h = 0xBB40E64DA205B064LL;
		j = 0;
		while ( n ? j++ < n : *k ) //Fixed length or else until null.
		{
			h = ( h * 7664345821815920749LL ) ^ hashfn_tab[(unsigned char)(*k)];
			k++;
		}
		return h;
	}
};

template<class keyT, class hfnT> struct Hashtable
{
	//Instantiate a hash table, with methods for maintaining a one-to-one correspondence between
	//arbitrary keys and unique integers in a specified range.
	Int nhash, nmax, nn, ng;
	VecInt htable, next, garbg;
	VecUllong thehash;
	hfnT hash;  //An instance of a hash function object.
	Hashtable( Int nh, Int nv );
	//Constructor. Arguments are size of hash table and max number of stored elements(keys).

	Int iget( const keyT &key ); //Return integer for a previously set key.
	Int iset( const keyT &key ); //Return unique integer for a new key.
	Int ierase( const keyT &key ); //Erase a key.
	Int ireserve();              //Reserve an integer( with no key).
	Int irelinquish( Int k );   //Un-reserve an integer.
};


template<class keyT, class hfnT >
Hashtable<keyT,hfnT>::Hashtable( Int nh, Int nv ):
//Constructor. Set nhash, the size of the hash table, and nmax, the maximum number of elements
//(keys) that can be accommodated. Allocate arrays appropriately.
hash(sizeof(keyT)), nhash(nh), nmax(nv), nn(0),ng(0),
htable(nh),next(nv),garbg(nv),thehash(nv)
{
	for ( Int j = 0; j < nh; j++ )
	{
		htable[j] = -1; //Signifies empty.
	}
}

template<class keyT, class hfnT>
Int Hashtable<keyT,hfnT>::iget( const keyT &key )
{
	//Returns integer in 0..nmax-1 corresponding to key, or -1 if no such key was previously stored.
	Int j, k;
	Ullong pp = hash.fn( &key );   //Get 64-bit hash.
	j = (Int)(pp % nhash);           //and map it into the hash table.
	for ( k = htable[j]; k != -1; k = next[k] )//Traverse linked list until an exact match is found.
	{
		if ( thehash[k] == pp )
		{
			return k;
		}
	}
	return -1;        //Key was not previously stored.
}


template<class keyT, class hfnT>
Int Hashtable<keyT,hfnT>::iset( const keyT &key )
{
	//Returns integer in 0..nmax-1 that will henceforth correspond to key. If key was previously set,
	//return the same integer as before.
	Int j, k, kprev;
	Ullong pp = hash.fn(&key);   //Get 64-bit hash
	j = (Int)( pp % nhash );     //and map it into the hash table.

	if ( htable[j] == -1 )       //Key not in table. Find a free integer, either new or previously erased.
	{
		k = ng ? garbg[--ng] : nn++;
		htable[j] = k;
	}
	else    //Key might be in table. Traverse list.
	{
		for ( k = htable[j]; k != -1; k = next[k] )
		{
			if ( thehash[k] == pp )
			{
				return k;  //Yes. Return previous value.
			}
			kprev = k;
		}
		k = ng ? garbg[--ng] : nn++;   //No. Get new integer.
		next[kprev] = k;
	}
	if ( k >= nmax )
	{
		throw( "storing too many values" );
	}
	thehash[k] = pp;   //Store the key at the new or previous integer.
	next[k] = -1;
	return k;

}

template<class keyT, class hfnT>
Int Hashtable<keyT, hfnT>::ierase( const keyT &key)
{
	//Erase a key, returning the integer in 0..nmax-1 erased, or -1 if the key was not previously set.
	Int j, k, kprev;
	Ullong pp = hash.fn( &key );
	j = (Int)( pp % nhash );
	if ( htable[j] == -1 )
	{
		return -1; //Key not previously set.
	}
	kprev = -1;
	for ( k = htable[j]; k != -1; k = next[k] )
	{
		if ( thehash[k] == pp )  //Found key. Splice linked list around it.
		{
			if ( kprev == -1 )
			{
				htable[j] = next[k];
			}
			else next[kprev] = next[k];
			garbg[ng++] = k;     //Add k to garbage stack as an available integer.

			//next[k] = -1;   //Optional correction term.

			return k;
		}
		kprev = k;
	}
	return -1;  //Key not previously set.
}

template<class keyT, class hfnT>
Int Hashtable<keyT,hfnT>::ireserve()
{
	//Reserve an integer in 0..nmax-1 so that it will not be used by set(), and return its value.
	Int k = ng ? garbg[--ng] : nn++;
	if ( k >= nmax )
	{
		throw( "reserving too many values" );
	}
	next[k] = -2;
	return k;
}

template<class keyT, class hfnT>
Int Hashtable<keyT, hfnT>::irelinquish( Int k)
{
	//Return to the pool an index previously reserved by reserve(), and return it, or return -1 if it
	//was not previously reserved.
	if ( next[k] != -2 )
	{
		return -1;
	}
	garbg[ng++] = k;
	return k;
}


template<class keyT, class elT, class hfnT>
struct Hash: Hashtable<keyT,hfnT>
{
	//Extend the Hashtable class with storage for elements of type elT, and provide methods for
	//storing, retrieving and erasing elements. key is passed by address in all methods.
	using Hashtable<keyT,hfnT>::iget;
	using Hashtable<keyT,hfnT>::iset;
	using Hashtable<keyT,hfnT>::ierase;
	vector<elT> els;

	Hash( Int nh, Int nm ): Hashtable<keyT,hfnT>(nh,nm),els(nm){}
	//Same constructor syntax as Hashtable.

	inline void set( const keyT &key, const elT &el ) //Store an element el.
	{
		els[iset(key)] = el;
	}
	
	inline Int get( const keyT &key, elT &el )
	{
		//Retrieve an element into el. Returns 0 if no element is stored under key, or 1 for success.
		Int ll = iget(key);
		if ( ll < 0 )
		{
			return 0;
		}
		el = els[ll];
		return 1;
	}

	inline elT& operator[]( const keyT &key )
	{
		//Store or retrieve an element using subscript notation for its key. Returns a reference that
		//can be used as an I-value.
		Int ll = iget(key);
		if ( ll <0 )
		{
			ll = iset(key);
			els[ll] = elT();
		}
		return els[ll];
	}

	inline Int count( const keyT &key )
	{
		//Return the number of elements stored under key, that is, either 0 or 1.
		Int ll = iget(key);
		return ( ll < 0 ? 0 : 1 );
	}

	inline Int erase( const keyT &key )
	{
		//Erase an element. Returns 1 for success, or 0 if no element is stored under key.
		return ( ierase(key) < 0 ? 0 : 1 );
	}

};

template<class keyT, class elT, class hfnT >
struct Mhash : Hashtable<keyT,hfnT>
{
	//Extend the Hashtable class with storage for elements of type elT, allowing more than one
	//element to be stored under a single key.
	using Hashtable<keyT,hfnT>::iget;
	using Hashtable<keyT,hfnT>::iset;
	using Hashtable<keyT,hfnT>::ierase;
	using Hashtable<keyT,hfnT>::ireserve;
	using Hashtable<keyT,hfnT>::irelinquish;
	vector<elT> els;
	VecInt nextsis;  //Links to next sister element under a single key.
	Int nextget;

	Mhash( Int nh, Int nm ); //Same constructor syntax as Hashtable.
	Int store( const keyT &key, const elT &el ); //Store an element under key.
	Int erase( const keyT &key, const elT &el ); //Erase a specified element under key.
	Int count( const keyT &key ); //Count elements stored under key.
	Int getinit( const keyT &key ); //Prepare to retrieve elements from key.
	Int getnext( elT &el ); //Retrieve next element specified by getinit.
};


template<class keyT, class elT, class hfnT>
Mhash<keyT,elT,hfnT>::Mhash( Int nh, Int nm )
:Hashtable<keyT,hfnT>(nh,nm),nextget(-1),els(nm),nextsis(nm)
{
	for ( Int j = 0; j < nm; j++ )
	{
		nextsis[j] = -2;  //Initialize to "empty"
	}
}

template<class keyT, class elT, class hfnT>
Int Mhash<keyT,elT,hfnT>::store( const keyT &key, const elT &el)
{
	//Store an element el under key. Return index in 0..nmax-1, giving the storage location utilized.
	Int j, k;
	j = iset(key); //Find root index for this key.
	if ( nextsis[j] == -2 )
	{
		//It is the first object with this key.
		els[j] = el;
		nextsis[j] = -1; //-1 means it is the terminal element.
		return j;
	}
	else
	{
		while( nextsis[j] != -1 ) { j = nextsis[j]; } //Traverse the tree.
		k = ireserve(); //Get a new index and link it into the list.
		els[k] = el;
		nextsis[j] = k;
		nextsis[k] = -1;
		return k;
	}
}

template<class keyT, class elT, class hfnT>
Int Mhash<keyT,elT,hfnT>::erase( const keyT &key, const elT &el )
{
	//Erase an element el previously stored under key. Return 1 for success, or 0 if no matching
	//element is found. Note: The == operation must be defined for the type elT.
	Int j = -1, kp = -1, kpp = -1;
	Int k = iget( key );
	while ( k >= 0 )
	{
		if( j < 0 && el == els[k] ) j = k; //Save index of matching el as j.
		kpp = kp;
		kp = k;
		k = nextsis[k];
	}
	if( j < 0 ) return 0; //No matching el found.
	if ( kpp < 0 ) //The element  el was unique under key.
	{
		ierase(key);
		nextsis[j] = -2;
	}
	else      //Patch the list.
	{
		if( j != kp ) els[j] = els[kp]; //Overwrite j with the terminal element and then shorten the list.
		nextsis[kpp] = -1;
		irelinquish(kp);
		nextsis[kp] = -2;
	}
	return 1;        //Success.
}

template<class keyT, class elT, class hfnT>
Int Mhash<keyT,elT,hfnT>::count( const keyT &key )
{
	//Return the number of elements stored under key, 0 if none.
	Int next, n = 1;
	if( (next = iget(key)) < 0 ) return 0;
	while ( (next=nextsis[next]) >= 0 ) { n++; }
	return n;
}

template<class keyT, class elT, class hfnT>
Int Mhash<keyT,elT,hfnT>::getinit( const keyT &key )
{
	//Initialize nextget so that it points to the first element stored under key. Return 1 for success.
	nextget = iget(key);
	return ((nextget<0)? 0 : 1);
}

template<class keyT, class elT, class hfnT>
Int Mhash<keyT, elT, hfnT>::getnext( elT &el )
{
	//If nextget points validly, copy its element into el, update nextget to the next element with
	//the same key, and return 1. Otherwise, do not modify el, and return 0.
	if ( nextget < 0 )
	{
		return 0;
	}
	el = els[nextget];
	nextget = nextsis[nextget];
	return 1;
}


#endif