// Locale support -*- C++ -*-

// Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
// 2006, 2007, 2008, 2009, 2010, 2011
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/locale_facets.tcc
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{locale}
 */

#ifndef _LOCALE_FACETS_TCC
#define _LOCALE_FACETS_TCC 1

#pragma GCC system_header

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Routine to access a cache for the facet.  If the cache didn't
  // exist before, it gets constructed on the fly.
  template<typename _Facet>
    struct __use_cache
    {
      const _Facet*
      operator() (const locale& __loc) const;
    };

  // Specializations.
  template<typename _CharT>
    struct __use_cache<__numpunct_cache<_CharT> >
    {
      const __numpunct_cache<_CharT>*
      operator() (const locale& __loc) const
      {
	const size_t __i = numpunct<_CharT>::id._M_id();
	const locale::facet** __caches = __loc._M_impl->_M_caches;
	if (!__caches[__i])
	  {
	    __numpunct_cache<_CharT>* __tmp = 0;
	    __try
	      {
		__tmp = new __numpunct_cache<_CharT>;
		__tmp->_M_cache(__loc);
	      }
	    __catch(...)
	      {
		delete __tmp;
		__throw_exception_again;
	      }
	    __loc._M_impl->_M_install_cache(__tmp, __i);
	  }
	return static_cast<const __numpunct_cache<_CharT>*>(__caches[__i]);
      }
    };

  template<typename _CharT>
    void
    __numpunct_cache<_CharT>::_M_cache(const locale& __loc)
    {
      _M_allocated = true;

      const numpunct<_CharT>& __np = use_facet<numpunct<_CharT> >(__loc);

      char* __grouping = 0;
      _CharT* __truename = 0;
      _CharT* __falsename = 0;
      __try
	{
	  _M_grouping_size = __np.grouping().size();
	  __grouping = new char[_M_grouping_size];
	  __np.grouping().copy(__grouping, _M_grouping_size);
	  _M_grouping = __grouping;
	  _M_use_grouping = (_M_grouping_size
			     && static_cast<signed char>(_M_grouping[0]) > 0
			     && (_M_grouping[0]
				 != __gnu_cxx::__numeric_traits<char>::__max));

	  _M_truename_size = __np.truename().size();
	  __truename = new _CharT[_M_truename_size];
	  __np.truename().copy(__truename, _M_truename_size);
	  _M_truename = __truename;

	  _M_falsename_size = __np.falsename().size();
	  __falsename = new _CharT[_M_falsename_size];
	  __np.falsename().copy(__falsename, _M_falsename_size);
	  _M_falsename = __falsename;

	  _M_decimal_point = __np.decimal_point();
	  _M_thousands_sep = __np.thousands_sep();

	  const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__loc);
	  __ct.widen(__num_base::_S_atoms_out,
		     __num_base::_S_atoms_out
		     + __num_base::_S_oend, _M_atoms_out);
	  __ct.widen(__num_base::_S_atoms_in,
		     __num_base::_S_atoms_in
		     + __num_base::_S_iend, _M_atoms_in);
	}
      __catch(...)
	{
	  delete [] __grouping;
	  delete [] __truename;
	  delete [] __falsename;
	  __throw_exception_again;
	}
    }

  // Used by both numeric and monetary facets.
  // Check to make sure that the __grouping_tmp string constructed in
  // money_get or num_get matches the canonical grouping for a given
  // locale.
  // __grouping_tmp is parsed L to R
  // 1,222,444 == __grouping_tmp of "\1\3\3"
  // __grouping is parsed R to L
  // 1,222,444 == __grouping of "\3" == "\3\3\3"
  _GLIBCXX_PURE bool
  __verify_grouping(const char* __grouping, size_t __grouping_size,
		    const string& __grouping_tmp) throw ();

_GLIBCXX_BEGIN_NAMESPACE_LDBL

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    _M_extract_float(_InIter __beg, _InIter __end, ios_base& __io,
		     ios_base::iostate& __err, string& __xtrc) const
    {
      typedef char_traits<_CharT>			__traits_type;
      typedef __numpunct_cache<_CharT>                  __cache_type;
      __use_cache<__cache_type> __uc;
      const locale& __loc = __io._M_getloc();
      const __cache_type* __lc = __uc(__loc);
      const _CharT* __lit = __lc->_M_atoms_in;
      char_type __c = char_type();

      // True if __beg becomes equal to __end.
      bool __testeof = __beg == __end;

      // First check for sign.
      if (!__testeof)
	{
	  __c = *__beg;
	  const bool __plus = __c == __lit[__num_base::_S_iplus];
	  if ((__plus || __c == __lit[__num_base::_S_iminus])
	      && !(__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
	      && !(__c == __lc->_M_decimal_point))
	    {
	      __xtrc += __plus ? '+' : '-';
	      if (++__beg != __end)
		__c = *__beg;
	      else
		__testeof = true;
	    }
	}

      // Next, look for leading zeros.
      bool __found_mantissa = false;
      int __sep_pos = 0;
      while (!__testeof)
	{
	  if ((__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
	      || __c == __lc->_M_decimal_point)
	    break;
	  else if (__c == __lit[__num_base::_S_izero])
	    {
	      if (!__found_mantissa)
		{
		  __xtrc += '0';
		  __found_mantissa = true;
		}
	      ++__sep_pos;

	      if (++__beg != __end)
		__c = *__beg;
	      else
		__testeof = true;
	    }
	  else
	    break;
	}

      // Only need acceptable digits for floating point numbers.
      bool __found_dec = false;
      bool __found_sci = false;
      string __found_grouping;
      if (__lc->_M_use_grouping)
	__found_grouping.reserve(32);
      const char_type* __lit_zero = __lit + __num_base::_S_izero;

      if (!__lc->_M_allocated)
	// "C" locale
	while (!__testeof)
	  {
	    const int __digit = _M_find(__lit_zero, 10, __c);
	    if (__digit != -1)
	      {
		__xtrc += '0' + __digit;
		__found_mantissa = true;
	      }
	    else if (__c == __lc->_M_decimal_point
		     && !__found_dec && !__found_sci)
	      {
		__xtrc += '.';
		__found_dec = true;
	      }
	    else if ((__c == __lit[__num_base::_S_ie]
		      || __c == __lit[__num_base::_S_iE])
		     && !__found_sci && __found_mantissa)
	      {
		// Scientific notation.
		__xtrc += 'e';
		__found_sci = true;

		// Remove optional plus or minus sign, if they exist.
		if (++__beg != __end)
		  {
		    __c = *__beg;
		    const bool __plus = __c == __lit[__num_base::_S_iplus];
		    if (__plus || __c == __lit[__num_base::_S_iminus])
		      __xtrc += __plus ? '+' : '-';
		    else
		      continue;
		  }
		else
		  {
		    __testeof = true;
		    break;
		  }
	      }
	    else
	      break;

	    if (++__beg != __end)
	      __c = *__beg;
	    else
	      __testeof = true;
	  }
      else
	while (!__testeof)
	  {
	    // According to 22.2.2.1.2, p8-9, first look for thousands_sep
	    // and decimal_point.
	    if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
	      {
		if (!__found_dec && !__found_sci)
		  {
		    // NB: Thousands separator at the beginning of a string
		    // is a no-no, as is two consecutive thousands separators.
		    if (__sep_pos)
		      {
			__found_grouping += static_cast<char>(__sep_pos);
			__sep_pos = 0;
		      }
		    else
		      {
			// NB: __convert_to_v will not assign __v and will
			// set the failbit.
			__xtrc.clear();
			break;
		      }
		  }
		else
		  break;
	      }
	    else if (__c == __lc->_M_decimal_point)
	      {
		if (!__found_dec && !__found_sci)
		  {
		    // If no grouping chars are seen, no grouping check
		    // is applied. Therefore __found_grouping is adjusted
		    // only if decimal_point comes after some thousands_sep.
		    if (__found_grouping.size())
		      __found_grouping += static_cast<char>(__sep_pos);
		    __xtrc += '.';
		    __found_dec = true;
		  }
		else
		  break;
	      }
	    else
	      {
		const char_type* __q =
		  __traits_type::find(__lit_zero, 10, __c);
		if (__q)
		  {
		    __xtrc += '0' + (__q - __lit_zero);
		    __found_mantissa = true;
		    ++__sep_pos;
		  }
		else if ((__c == __lit[__num_base::_S_ie]
			  || __c == __lit[__num_base::_S_iE])
			 && !__found_sci && __found_mantissa)
		  {
		    // Scientific notation.
		    if (__found_grouping.size() && !__found_dec)
		      __found_grouping += static_cast<char>(__sep_pos);
		    __xtrc += 'e';
		    __found_sci = true;

		    // Remove optional plus or minus sign, if they exist.
		    if (++__beg != __end)
		      {
			__c = *__beg;
			const bool __plus = __c == __lit[__num_base::_S_iplus];
			if ((__plus || __c == __lit[__num_base::_S_iminus])
			    && !(__lc->_M_use_grouping
				 && __c == __lc->_M_thousands_sep)
			    && !(__c == __lc->_M_decimal_point))
		      __xtrc += __plus ? '+' : '-';
			else
			  continue;
		      }
		    else
		      {
			__testeof = true;
			break;
		      }
		  }
		else
		  break;
	      }

	    if (++__beg != __end)
	      __c = *__beg;
	    else
	      __testeof = true;
	  }

      // Digit grouping is checked. If grouping and found_grouping don't
      // match, then get very very upset, and set failbit.
      if (__found_grouping.size())
        {
          // Add the ending grouping if a decimal or 'e'/'E' wasn't found.
	  if (!__found_dec && !__found_sci)
	    __found_grouping += static_cast<char>(__sep_pos);

          if (!std::__verify_grouping(__lc->_M_grouping,
				      __lc->_M_grouping_size,
				      __found_grouping))
	    __err = ios_base::failbit;
        }

      return __beg;
    }

  template<typename _CharT, typename _InIter>
    template<typename _ValueT>
      _InIter
      num_get<_CharT, _InIter>::
      _M_extract_int(_InIter __beg, _InIter __end, ios_base& __io,
		     ios_base::iostate& __err, _ValueT& __v) const
      {
        typedef char_traits<_CharT>			     __traits_type;
	using __gnu_cxx::__add_unsigned;
	typedef typename __add_unsigned<_ValueT>::__type __unsigned_type;
	typedef __numpunct_cache<_CharT>                     __cache_type;
	__use_cache<__cache_type> __uc;
	const locale& __loc = __io._M_getloc();
	const __cache_type* __lc = __uc(__loc);
	const _CharT* __lit = __lc->_M_atoms_in;
	char_type __c = char_type();

	// NB: Iff __basefield == 0, __base can change based on contents.
	const ios_base::fmtflags __basefield = __io.flags()
	                                       & ios_base::basefield;
	const bool __oct = __basefield == ios_base::oct;
	int __base = __oct ? 8 : (__basefield == ios_base::hex ? 16 : 10);

	// True if __beg becomes equal to __end.
	bool __testeof = __beg == __end;

	// First check for sign.
	bool __negative = false;
	if (!__testeof)
	  {
	    __c = *__beg;
	    __negative = __c == __lit[__num_base::_S_iminus];
	    if ((__negative || __c == __lit[__num_base::_S_iplus])
		&& !(__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
		&& !(__c == __lc->_M_decimal_point))
	      {
		if (++__beg != __end)
		  __c = *__beg;
		else
		  __testeof = true;
	      }
	  }

	// Next, look for leading zeros and check required digits
	// for base formats.
	bool __found_zero = false;
	int __sep_pos = 0;
	while (!__testeof)
	  {
	    if ((__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
		|| __c == __lc->_M_decimal_point)
	      break;
	    else if (__c == __lit[__num_base::_S_izero]
		     && (!__found_zero || __base == 10))
	      {
		__found_zero = true;
		++__sep_pos;
		if (__basefield == 0)
		  __base = 8;
		if (__base == 8)
		  __sep_pos = 0;
	      }
	    else if (__found_zero
		     && (__c == __lit[__num_base::_S_ix]
			 || __c == __lit[__num_base::_S_iX]))
	      {
		if (__basefield == 0)
		  __base = 16;
		if (__base == 16)
		  {
		    __found_zero = false;
		    __sep_pos = 0;
		  }
		else
		  break;
	      }
	    else
	      break;

	    if (++__beg != __end)
	      {
		__c = *__beg;
		if (!__found_zero)
		  break;
	      }
	    else
	      __testeof = true;
	  }

	// At this point, base is determined. If not hex, only allow
	// base digits as valid input.
	const size_t __len = (__base == 16 ? __num_base::_S_iend
			      - __num_base::_S_izero : __base);

	// Extract.
	string __found_grouping;
	if (__lc->_M_use_grouping)
	  __found_grouping.reserve(32);
	bool __testfail = false;
	bool __testoverflow = false;
	const __unsigned_type __max =
	  (__negative && __gnu_cxx::__numeric_traits<_ValueT>::__is_signed)
	  ? -__gnu_cxx::__numeric_traits<_ValueT>::__min
	  : __gnu_cxx::__numeric_traits<_ValueT>::__max;
	const __unsigned_type __smax = __max / __base;
	__unsigned_type __result = 0;
	int __digit = 0;
	const char_type* __lit_zero = __lit + __num_base::_S_izero;

	if (!__lc->_M_allocated)
	  // "C" locale
	  while (!__testeof)
	    {
	      __digit = _M_find(__lit_zero, __len, __c);
	      if (__digit == -1)
		break;

	      if (__result > __smax)
		__testoverflow = true;
	      else
		{
		  __result *= __base;
		  __testoverflow |= __result > __max - __digit;
		  __result += __digit;
		  ++__sep_pos;
		}

	      if (++__beg != __end)
		__c = *__beg;
	      else
		__testeof = true;
	    }
	else
	  while (!__testeof)
	    {
	      // According to 22.2.2.1.2, p8-9, first look for thousands_sep
	      // and decimal_point.
	      if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
		{
		  // NB: Thousands separator at the beginning of a string
		  // is a no-no, as is two consecutive thousands separators.
		  if (__sep_pos)
		    {
		      __found_grouping += static_cast<char>(__sep_pos);
		      __sep_pos = 0;
		    }
		  else
		    {
		      __testfail = true;
		      break;
		    }
		}
	      else if (__c == __lc->_M_decimal_point)
		break;
	      else
		{
		  const char_type* __q =
		    __traits_type::find(__lit_zero, __len, __c);
		  if (!__q)
		    break;

		  __digit = __q - __lit_zero;
		  if (__digit > 15)
		    __digit -= 6;
		  if (__result > __smax)
		    __testoverflow = true;
		  else
		    {
		      __result *= __base;
		      __testoverflow |= __result > __max - __digit;
		      __result += __digit;
		      ++__sep_pos;
		    }
		}

	      if (++__beg != __end)
		__c = *__beg;
	      else
		__testeof = true;
	    }

	// Digit grouping is checked. If grouping and found_grouping don't
	// match, then get very very upset, and set failbit.
	if (__found_grouping.size())
	  {
	    // Add the ending grouping.
	    __found_grouping += static_cast<char>(__sep_pos);

	    if (!std::__verify_grouping(__lc->_M_grouping,
					__lc->_M_grouping_size,
					__found_grouping))
	      __err = ios_base::failbit;
	  }

	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 23. Num_get overflow result.
	if ((!__sep_pos && !__found_zero && !__found_grouping.size())
	    || __testfail)
	  {
	    __v = 0;
	    __err = ios_base::failbit;
	  }
	else if (__testoverflow)
	  {
	    if (__negative
		&& __gnu_cxx::__numeric_traits<_ValueT>::__is_signed)
	      __v = __gnu_cxx::__numeric_traits<_ValueT>::__min;
	    else
	      __v = __gnu_cxx::__numeric_traits<_ValueT>::__max;
	    __err = ios_base::failbit;
	  }
	else
	  __v = __negative ? -__result : __result;

	if (__testeof)
	  __err |= ios_base::eofbit;
	return __beg;
      }



_GLIBCXX_END_NAMESPACE_LDBL

  // Construct correctly padded string, as per 22.2.2.2.2
  // Assumes
  // __newlen > __oldlen
  // __news is allocated for __newlen size

  // NB: Of the two parameters, _CharT can be deduced from the
  // function arguments. The other (_Traits) has to be explicitly specified.
  template<typename _CharT, typename _Traits>
    void
    __pad<_CharT, _Traits>::_S_pad(ios_base& __io, _CharT __fill,
				   _CharT* __news, const _CharT* __olds,
				   streamsize __newlen, streamsize __oldlen)
    {
      const size_t __plen = static_cast<size_t>(__newlen - __oldlen);
      const ios_base::fmtflags __adjust = __io.flags() & ios_base::adjustfield;

      // Padding last.
      if (__adjust == ios_base::left)
	{
	  _Traits::copy(__news, __olds, __oldlen);
	  _Traits::assign(__news + __oldlen, __plen, __fill);
	  return;
	}

      size_t __mod = 0;
      if (__adjust == ios_base::internal)
	{
	  // Pad after the sign, if there is one.
	  // Pad after 0[xX], if there is one.
	  // Who came up with these rules, anyway? Jeeze.
          const locale& __loc = __io._M_getloc();
	  const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

	  if (__ctype.widen('-') == __olds[0]
	      || __ctype.widen('+') == __olds[0])
	    {
	      __news[0] = __olds[0];
	      __mod = 1;
	      ++__news;
	    }
	  else if (__ctype.widen('0') == __olds[0]
		   && __oldlen > 1
		   && (__ctype.widen('x') == __olds[1]
		       || __ctype.widen('X') == __olds[1]))
	    {
	      __news[0] = __olds[0];
	      __news[1] = __olds[1];
	      __mod = 2;
	      __news += 2;
	    }
	  // else Padding first.
	}
      _Traits::assign(__news, __plen, __fill);
      _Traits::copy(__news + __plen, __olds + __mod, __oldlen - __mod);
    }

  template<typename _CharT>
    _CharT*
    __add_grouping(_CharT* __s, _CharT __sep,
		   const char* __gbeg, size_t __gsize,
		   const _CharT* __first, const _CharT* __last)
    {
      size_t __idx = 0;
      size_t __ctr = 0;

      while (__last - __first > __gbeg[__idx]
	     && static_cast<signed char>(__gbeg[__idx]) > 0
	     && __gbeg[__idx] != __gnu_cxx::__numeric_traits<char>::__max)
	{
	  __last -= __gbeg[__idx];
	  __idx < __gsize - 1 ? ++__idx : ++__ctr;
	}

      while (__first != __last)
	*__s++ = *__first++;

      while (__ctr--)
	{
	  *__s++ = __sep;
	  for (char __i = __gbeg[__idx]; __i > 0; --__i)
	    *__s++ = *__first++;
	}

      while (__idx--)
	{
	  *__s++ = __sep;
	  for (char __i = __gbeg[__idx]; __i > 0; --__i)
	    *__s++ = *__first++;
	}

      return __s;
    }

  // Inhibit implicit instantiations for required instantiations,
  // which are defined via explicit instantiations elsewhere.
#if _GLIBCXX_EXTERN_TEMPLATE
  extern template class numpunct<char>;
  extern template class numpunct_byname<char>;
  extern template class ctype_byname<char>;

  extern template
    const ctype<char>&
    use_facet<ctype<char> >(const locale&);

  extern template
    const numpunct<char>&
    use_facet<numpunct<char> >(const locale&);


  extern template
    bool
    has_facet<ctype<char> >(const locale&);

  extern template
    bool
    has_facet<numpunct<char> >(const locale&);



#ifdef _GLIBCXX_USE_WCHAR_T
  extern template class numpunct<wchar_t>;
  extern template class numpunct_byname<wchar_t>;
  extern template class _GLIBCXX_NAMESPACE_LDBL num_get<wchar_t>;
  extern template class _GLIBCXX_NAMESPACE_LDBL num_put<wchar_t>;
  extern template class ctype_byname<wchar_t>;

  extern template
    const ctype<wchar_t>&
    use_facet<ctype<wchar_t> >(const locale&);

  extern template
    const numpunct<wchar_t>&
    use_facet<numpunct<wchar_t> >(const locale&);

  extern template
    const num_put<wchar_t>&
    use_facet<num_put<wchar_t> >(const locale&);

  extern template
    const num_get<wchar_t>&
    use_facet<num_get<wchar_t> >(const locale&);

 extern template
    bool
    has_facet<ctype<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<numpunct<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<num_put<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<num_get<wchar_t> >(const locale&);
#endif
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif
