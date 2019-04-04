#pragma once

#include <vector>

namespace itl2
{

	/*
	 * http://ndevilla.free.fr/median
	 *
	 * The following routines have been built from knowledge gathered
	 * around the Web. I am not aware of any copyright problem with
	 * them, so use it as you want.
	 * N. Devillard - 1998
	 */


	#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
	#define PIX_SWAP(a,b) { pixel_t temp=(a);(a)=(b);(b)=temp; }

	/*----------------------------------------------------------------------------
	   Function :   opt_med3()
	   In       :   pointer to array of 3 pixel values
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 3 pixel values
	   Notice   :   found on sci.image.processing
					cannot go faster unless assumptions are made
					on the nature of the input signal.
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med3(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[0], p[1]); PIX_SORT(p[1], p[2]); PIX_SORT(p[0], p[1]);
		return(p[1]);
	}

	/*----------------------------------------------------------------------------
	   Function :   opt_med5()
	   In       :   pointer to array of 5 pixel values
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 5 pixel values
	   Notice   :   found on sci.image.processing
					cannot go faster unless assumptions are made
					on the nature of the input signal.
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med5(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[0], p[1]); PIX_SORT(p[3], p[4]); PIX_SORT(p[0], p[3]);
		PIX_SORT(p[1], p[4]); PIX_SORT(p[1], p[2]); PIX_SORT(p[2], p[3]);
		PIX_SORT(p[1], p[2]); return(p[2]);
	}

	/*----------------------------------------------------------------------------
	   Function :   opt_med6()
	   In       :   pointer to array of 6 pixel values
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 6 pixel values
	   Notice   :   from Christoph_John@gmx.de
					based on a selection network which was proposed in
					"FAST, EFFICIENT MEDIAN FILTERS WITH EVEN LENGTH WINDOWS"
					J.P. HAVLICEK, K.A. SAKADY, G.R.KATZ
					If you need larger even length kernels check the paper
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med6(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[1], p[2]); PIX_SORT(p[3], p[4]);
		PIX_SORT(p[0], p[1]); PIX_SORT(p[2], p[3]); PIX_SORT(p[4], p[5]);
		PIX_SORT(p[1], p[2]); PIX_SORT(p[3], p[4]);
		PIX_SORT(p[0], p[1]); PIX_SORT(p[2], p[3]); PIX_SORT(p[4], p[5]);
		PIX_SORT(p[1], p[2]); PIX_SORT(p[3], p[4]);
		return (p[2] + p[3]) * 0.5;
		/* PIX_SORT(p[2], p[3]) results in lower median in p[2] and upper median in p[3] */
	}


	/*----------------------------------------------------------------------------
	   Function :   opt_med7()
	   In       :   pointer to array of 7 pixel values
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 7 pixel values
	   Notice   :   found on sci.image.processing
					cannot go faster unless assumptions are made
					on the nature of the input signal.
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med7(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[0], p[5]); PIX_SORT(p[0], p[3]); PIX_SORT(p[1], p[6]);
		PIX_SORT(p[2], p[4]); PIX_SORT(p[0], p[1]); PIX_SORT(p[3], p[5]);
		PIX_SORT(p[2], p[6]); PIX_SORT(p[2], p[3]); PIX_SORT(p[3], p[6]);
		PIX_SORT(p[4], p[5]); PIX_SORT(p[1], p[4]); PIX_SORT(p[1], p[3]);
		PIX_SORT(p[3], p[4]);
		return (p[3]);
	}

	/*----------------------------------------------------------------------------
	   Function :   opt_med9()
	   In       :   pointer to an array of 9 pixelvalues
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 9 pixelvalues
	   Notice   :   in theory, cannot go faster without assumptions on the
					signal.
					Formula from:
					XILINX XCELL magazine, vol. 23 by John L. Smith

					The input array is modified in the process
					The result array is guaranteed to contain the median
					value
					in middle position, but other elements are NOT sorted.
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med9(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[1], p[2]); PIX_SORT(p[4], p[5]); PIX_SORT(p[7], p[8]);
		PIX_SORT(p[0], p[1]); PIX_SORT(p[3], p[4]); PIX_SORT(p[6], p[7]);
		PIX_SORT(p[1], p[2]); PIX_SORT(p[4], p[5]); PIX_SORT(p[7], p[8]);
		PIX_SORT(p[0], p[3]); PIX_SORT(p[5], p[8]); PIX_SORT(p[4], p[7]);
		PIX_SORT(p[3], p[6]); PIX_SORT(p[1], p[4]); PIX_SORT(p[2], p[5]);
		PIX_SORT(p[4], p[7]); PIX_SORT(p[4], p[2]); PIX_SORT(p[6], p[4]);
		PIX_SORT(p[4], p[2]);
		return(p[4]);
	}


	/*----------------------------------------------------------------------------
	   Function :   opt_med25()
	   In       :   pointer to an array of 25 pixelvalues
	   Out      :   a pixelvalue
	   Job      :   optimized search of the median of 25 pixelvalues
	   Notice   :   in theory, cannot go faster without assumptions on the
					signal.
					Code taken from Graphic Gems.
	 ---------------------------------------------------------------------------*/

	template<typename pixel_t> double opt_med25(std::vector<pixel_t>& p)
	{
		PIX_SORT(p[0], p[1]);   PIX_SORT(p[3], p[4]);   PIX_SORT(p[2], p[4]);
		PIX_SORT(p[2], p[3]);   PIX_SORT(p[6], p[7]);   PIX_SORT(p[5], p[7]);
		PIX_SORT(p[5], p[6]);   PIX_SORT(p[9], p[10]);  PIX_SORT(p[8], p[10]);
		PIX_SORT(p[8], p[9]);   PIX_SORT(p[12], p[13]); PIX_SORT(p[11], p[13]);
		PIX_SORT(p[11], p[12]); PIX_SORT(p[15], p[16]); PIX_SORT(p[14], p[16]);
		PIX_SORT(p[14], p[15]); PIX_SORT(p[18], p[19]); PIX_SORT(p[17], p[19]);
		PIX_SORT(p[17], p[18]); PIX_SORT(p[21], p[22]); PIX_SORT(p[20], p[22]);
		PIX_SORT(p[20], p[21]); PIX_SORT(p[23], p[24]); PIX_SORT(p[2], p[5]);
		PIX_SORT(p[3], p[6]);   PIX_SORT(p[0], p[6]);   PIX_SORT(p[0], p[3]);
		PIX_SORT(p[4], p[7]);   PIX_SORT(p[1], p[7]);   PIX_SORT(p[1], p[4]);
		PIX_SORT(p[11], p[14]); PIX_SORT(p[8], p[14]);  PIX_SORT(p[8], p[11]);
		PIX_SORT(p[12], p[15]); PIX_SORT(p[9], p[15]);  PIX_SORT(p[9], p[12]);
		PIX_SORT(p[13], p[16]); PIX_SORT(p[10], p[16]); PIX_SORT(p[10], p[13]);
		PIX_SORT(p[20], p[23]); PIX_SORT(p[17], p[23]); PIX_SORT(p[17], p[20]);
		PIX_SORT(p[21], p[24]); PIX_SORT(p[18], p[24]); PIX_SORT(p[18], p[21]);
		PIX_SORT(p[19], p[22]); PIX_SORT(p[8], p[17]);  PIX_SORT(p[9], p[18]);
		PIX_SORT(p[0], p[18]);  PIX_SORT(p[0], p[9]);   PIX_SORT(p[10], p[19]);
		PIX_SORT(p[1], p[19]);  PIX_SORT(p[1], p[10]);  PIX_SORT(p[11], p[20]);
		PIX_SORT(p[2], p[20]);  PIX_SORT(p[2], p[11]);  PIX_SORT(p[12], p[21]);
		PIX_SORT(p[3], p[21]);  PIX_SORT(p[3], p[12]);  PIX_SORT(p[13], p[22]);
		PIX_SORT(p[4], p[22]);  PIX_SORT(p[4], p[13]);  PIX_SORT(p[14], p[23]);
		PIX_SORT(p[5], p[23]);  PIX_SORT(p[5], p[14]);  PIX_SORT(p[15], p[24]);
		PIX_SORT(p[6], p[24]);  PIX_SORT(p[6], p[15]);  PIX_SORT(p[7], p[16]);
		PIX_SORT(p[7], p[19]);  PIX_SORT(p[13], p[21]); PIX_SORT(p[15], p[23]);
		PIX_SORT(p[7], p[13]);  PIX_SORT(p[7], p[15]);  PIX_SORT(p[1], p[9]);
		PIX_SORT(p[3], p[11]);  PIX_SORT(p[5], p[17]);  PIX_SORT(p[11], p[17]);
		PIX_SORT(p[9], p[17]);  PIX_SORT(p[4], p[10]);  PIX_SORT(p[6], p[12]);
		PIX_SORT(p[7], p[14]);  PIX_SORT(p[4], p[6]);   PIX_SORT(p[4], p[7]);
		PIX_SORT(p[12], p[14]); PIX_SORT(p[10], p[14]); PIX_SORT(p[6], p[7]);
		PIX_SORT(p[10], p[12]); PIX_SORT(p[6], p[10]);  PIX_SORT(p[6], p[17]);
		PIX_SORT(p[12], p[17]); PIX_SORT(p[7], p[17]);  PIX_SORT(p[7], p[10]);
		PIX_SORT(p[12], p[18]); PIX_SORT(p[7], p[12]);  PIX_SORT(p[10], p[18]);
		PIX_SORT(p[12], p[20]); PIX_SORT(p[10], p[20]); PIX_SORT(p[10], p[12]);
		return (p[12]);
	}


	#undef PIX_SORT
	#undef PIX_SWAP


	/**
	Calculates median of values in vector v.
	*/
	template<typename pixel_t> double calcMedian(vector<pixel_t>& v)
	{
		size_t i = v.size();

		if (i == 9)
		{
			return opt_med9(v);
		}
		else if (i == 25)
		{
			return opt_med25(v);
		}
		else if (i == 6)
		{
			return opt_med6(v);
		}
		else if (i == 3)
		{
			return opt_med3(v);
		}
		if (i == 0)
		{
			return pixel_t();
		}
		else if (i == 1)
		{
			return v[0];
		}
		else
		{
			size_t n = i / 2;
			nth_element(v.begin(), v.begin() + n, v.end());
			pixel_t vn = v[n];

			if (v.size() % 2 == 1)
			{
				return vn;
			}
			else
			{
				nth_element(v.begin(), v.begin() + n - 1, v.end());
				return 0.5 * (vn + v[n - 1]);
			}
		}
	}

}