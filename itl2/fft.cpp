#include "fft.h"

#include <random>
#include <cmath>
#include <iomanip>

#include "io/raw.h"
#include "pointprocess.h"
#include "projections.h"
#include "test.h"
#include "misc.h"
#include "conversions.h"
#include "transform.h"
#include "noise.h"
#include "math/mathutils.h"

using namespace std;

namespace itl2
{
	/*
	Initializes FFTW if it has not been initialized.
	*/
	void initFFTW()
	{
		static bool isFFTWInit = false;

		#pragma omp critical
		{
			if (!isFFTWInit)
			{
				isFFTWInit = true;
				// NOTE: Enabling threading here makes things REALLY slow.
				//fftwf_init_threads();
				fftwf_import_system_wisdom();
			}
		}
	}

	void setThreads()
	{
		//if (omp_in_parallel())
		//	fftwf_plan_with_nthreads(1);
		//else
		//	fftwf_plan_with_nthreads(omp_get_num_threads());
	}

	void dct(Image<float32_t>& img)
	{
		initFFTW();

		if (img.dimensionality() == 1)
		{
			coord_t w = img.dimension(0);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_1d((int)w, img.getData(), img.getData(), FFTW_REDFT10, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 2)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_2d((int)h, (int)w, img.getData(), img.getData(), FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 3)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);
			coord_t d = img.dimension(2);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_3d((int)d, (int)h, (int)w, img.getData(), img.getData(), FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else
		{
			throw ITLException("Unsupported dimensionality.");
		}

		// Normalize the output image
		multiply(img, 1 / sqrt(::pow(2, img.dimensionality()) * img.pixelCount()));
	}

	void idct(Image<float32_t>& img)
	{
		initFFTW();

		if (img.dimensionality() == 1)
		{
			coord_t w = img.dimension(0);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_1d((int)w, img.getData(), img.getData(), FFTW_REDFT01, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 2)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_2d((int)h, (int)w, img.getData(), img.getData(), FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 3)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);
			coord_t d = img.dimension(2);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_r2r_3d((int)d, (int)h, (int)w, img.getData(), img.getData(), FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else
		{
			throw ITLException("Unsupported dimensionality.");
		}

		// Normalize the output image
		multiply(img, 1 / sqrt(::pow(2, img.dimensionality()) * img.pixelCount()));
	}


	void fft(Image<float32_t>& img, Image<complex32_t>& out)
	{
		initFFTW();

		if (img.dimensionality() == 1)
		{
			coord_t w = img.dimension(0);

			out.ensureSize(w / 2 + 1);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_r2c_1d((int)w, img.getData(), (fftwf_complex*)out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 2)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);

			out.ensureSize(w / 2 + 1, h);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_r2c_2d((int)h, (int)w, img.getData(), (fftwf_complex*)out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 3)
		{
			coord_t w = img.dimension(0);
			coord_t h = img.dimension(1);
			coord_t d = img.dimension(2);

			out.ensureSize(w / 2 + 1, h, d);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_r2c_3d((int)d, (int)h, (int)w, img.getData(), (fftwf_complex*)out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else
		{
			throw ITLException("Unsupported dimensionality.");
		}
	}

	void ifft(Image<complex32_t>& img, Image<float32_t>& out)
	{
		initFFTW();

		if (img.dimensionality() == 1)
		{
			coord_t w = (img.dimension(0) - 1) * 2;

			out.ensureSize(w);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_c2r_1d((int)w, (fftwf_complex*)img.getData(), out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 2)
		{
			coord_t w = (img.dimension(0) - 1) * 2;
			coord_t h = img.dimension(1);

			out.ensureSize(w, h);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_c2r_2d((int)h, (int)w, (fftwf_complex*)img.getData(), out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else if (img.dimensionality() == 3)
		{
			coord_t w = (img.dimension(0) - 1) * 2;
			coord_t h = img.dimension(1);
			coord_t d = img.dimension(2);

			out.ensureSize(w, h, d);

			fftwf_plan p;
			#pragma omp critical
			{
				setThreads();
				p = fftwf_plan_dft_c2r_3d((int)d, (int)h, (int)w, (fftwf_complex*)img.getData(), out.getData(), FFTW_ESTIMATE);
			}
			fftwf_execute(p);
			#pragma omp critical
			{
				fftwf_destroy_plan(p);
			}
		}
		else
		{
			throw ITLException("Unsupported dimensionality.");
		}

		// Normalize the output image
		divide(out, (double)out.pixelCount());
	}


	void bandpassFilter(Image<float32_t>& img, double min_size, double max_size, bool zeroEdges)
	{
		// This method is tuned to be consistent with gauss function, and to mimic ImageJ's 2D bandpass as well as possible.

		double min_sigma = min_size / 2;
		double max_sigma = max_size / 2;

		min_sigma = math::max(min_sigma, 0.0);
		max_sigma = math::max(max_sigma, 0.0);

		// See gauss method
		if (zeroEdges)
			setEdges(img, 0, (coord_t)ceil(1.5 * min_sigma));

		Image<complex32_t > ft;
		fft(img, ft);

		Vec3c size = img.dimensions();

		coord_t fw = ft.width();
		coord_t fh = ft.height();
		coord_t fd = ft.depth();

		// r = 0 are at (0, 0, 0), (0, fh, 0), (0, 0, fd) and (0, fh, fd)
		#pragma omp parallel for if(ft.pixelCount() > PARALLELIZATION_THRESHOLD && !omp_in_parallel())
		for (coord_t z = 0; z < fd; z++)
		{
			for (coord_t y = 0; y < fh; y++)
			{
				for (coord_t x = 0; x < fw; x++)
				{
					double dx = (double)(x - 0);
					double dy = (double)math::min(y - 0, fh - y);
					double dz = (double)math::min(z - 0, fd - z);

					double w1x = sqrt(2) * math::PI * max_sigma / size.x;
					double w1y = sqrt(2) * math::PI * max_sigma / size.y;
					double w1z = sqrt(2) * math::PI * max_sigma / size.z;

					double w2x = sqrt(2) * math::PI * min_sigma / size.x;
					double w2y = sqrt(2) * math::PI * min_sigma / size.y;
					double w2z = sqrt(2) * math::PI * min_sigma / size.z;

					double trans = (1.0 - ::exp(-(w1x * w1x * dx * dx + w1y * w1y * dy * dy + w1z * w1z * dz * dz)))
										* ::exp(-(w2x * w2x * dx * dx + w2y * w2y * dy * dy + w2z * w2z * dz * dz));
					ft(x, y, z) *= (float)trans;
				}
			}
		}

		ifft(ft, img);
	}

	void gaussFilter(Image<float32_t>& img, Vec3d sigma, bool zeroEdges)
	{
		sigma = componentwiseMax(sigma, Vec3d(0, 0, 0));

		// This zeroing seems to just about enough for many purposes.
		// Padding would be better but we don't want to start creating new images here.
		if (zeroEdges)
			setEdges(img, 0, math::componentwiseCeil(1.5 * sigma));

		Image<complex32_t > ft;
		fft(img, ft);

		Vec3c size = img.dimensions();

		coord_t fw = ft.width();
		coord_t fh = ft.height();
		coord_t fd = ft.depth();

		// r = 0 are at (0, 0, 0), (0, fh, 0), (0, 0, fd) and (0, fh, fd)
		#pragma omp parallel for if(ft.pixelCount() > PARALLELIZATION_THRESHOLD && !omp_in_parallel())
		for (coord_t z = 0; z < fd; z++)
		{
			for (coord_t y = 0; y < fh; y++)
			{
				for (coord_t x = 0; x < fw; x++)
				{
					double dx = (double)(x - 0);
					double dy = (double)math::min(y - 0, fh - y);
					double dz = (double)math::min(z - 0, fd - z);

					double wx = sqrt(2) * math::PI * sigma.x / size.x;
					double wy = sqrt(2) * math::PI * sigma.y / size.y;
					double wz = sqrt(2) * math::PI * sigma.z / size.z;

					double trans = ::exp( -(wx * wx * dx * dx + wy * wy * dy * dy + wz * wz * dz * dz));
					ft(x, y, z) *= (float)trans;
				}
			}
		}

		ifft(ft, img);
	}

	void powerSpectrum(const Image<complex32_t>& img, Image<float32_t>& pwr)
	{
		pwr.ensureSize(img);
		#pragma omp parallel for if(img.pixelCount() > PARALLELIZATION_THRESHOLD && !omp_in_parallel())
		for (coord_t n = 0; n < img.pixelCount(); n++)
			pwr(n) = norm(img(n));
	}


	namespace internals
	{
		/**
		Calculates x mod period as positive number.
		*/
		template<typename T> T modulo(T x, T period)
		{
			while (x > period)
				x -= period;
			while (x < 0)
				x += period;

			return x;
		}

		/*
		Helper function for Foroosh method.
		*/
		//inline double forooshHelper(double C0, double C1, double x0, double x1)
		//{
		//	double dx1 = C1 / (C1 + C0);
		//	double dx2 = C1 / (C1 - C0);
		//	double dx = dx1;

		//	bool dx1Good = -1 < dx1 && dx1 < 1 && copysign(1.0, dx1) == copysign(1.0, x1 - x0);
		//	bool dx2Good = -1 < dx2 && dx2 < 1 && copysign(1.0, dx2) == copysign(1.0, x1 - x0);
		//	
		//	if(dx1Good && dx2Good)
		//	{
		//		cout << "Logic :(" << endl;
		//		return dx1;
		//	}

		//	if (dx1Good)
		//		return dx1;

		//	return dx2;
		//}

		/**
		Finds location of peak in correlation image.
		*/
		Vec3d findPeak(Image<float32_t>& img, const Vec3c& maxShift, float32_t& maxVal)
		{
			// Find position of maximum
			Vec3c maxPos(0, 0, 0);
			maxVal = img(maxPos);
			for (coord_t z = -maxShift.z; z <= maxShift.z; z++)
			{
				coord_t zz = modulo(z, img.depth());
				for (coord_t y = -maxShift.y; y <= maxShift.y; y++)
				{
					coord_t yy = modulo(y, img.height());
					for (coord_t x = -maxShift.x; x <= maxShift.x; x++)
					{
						coord_t xx = modulo(x, img.width());

						//if(zz < 0 || zz >= img.depth() || yy < 0 || yy >= img.height() || xx < 0 || xx >= img.width())
						//	throw runtime_error("logical error");

						float32_t p = img(xx, yy, zz);
						if (p > maxVal)
						{
							maxVal = p;
							maxPos = Vec3c(x, y, z);
						}
					}
				}
			}

			// No subpixel accuracy

			return Vec3d((double)maxPos.x, (double)maxPos.y, (double)maxPos.z);
			
			/*
			// This approach is wrong: each coordinate direction must be "moduloed" independently
			Vec3c maxPos(0, 0, 0);
			maxVal = img(maxPos);
			for (coord_t z = 0; z < img.depth(); z++)
			{
				for (coord_t y = 0; y < img.width(); y++)
				{
					for (coord_t x = 0; x < img.depth(); x++)
					{
						float32_t p = img(x, y, z);
						if (p > maxVal)
						{
							maxVal = p;
							maxPos = Vec3c(x, y, z);
						}
					}
				}
			}

			Vec3c s1 = maxPos - Vec3c(0, 0, 0);
			Vec3c s2 = maxPos - Vec3c(0, img.height() - 1, 0);
			Vec3c s3 = maxPos - Vec3c(0, 0, img.depth() - 1);
			Vec3c s4 = maxPos - Vec3c(0, img.height() - 1, img.depth() - 1);

			Vec3c mins = s1;
			if (s2.normSquared() < mins.normSquared())
				mins = s2;
			if (s3.normSquared() < mins.normSquared())
				mins = s3;
			if (s4.normSquared() < mins.normSquared())
				mins = s4;

			return Vec3d(mins);
			*/

			
			// Centroid method for subpixel accuracy
			/*
			// Get data around the maximum (periodically) and calculate mean position of the peak.
			// This sould work as the peak should resemble a delta-peak. The center of mass of the peak should lie
			// at the exact location of the shift.
			const coord_t r = 8;
			coord_t x0 = maxPos.x;
			coord_t y0 = maxPos.y;
			coord_t z0 = maxPos.z;

			double xsum = 0;
			double ysum = 0;
			double zsum = 0;
			double wsum = 0;

			for (coord_t z = z0 - r; z <= z0 + r; z++)
			{
				coord_t zz = modulo(z, img.depth());
				for (coord_t y = y0 - r; y <= y0 + r; y++)
				{
					coord_t yy = modulo(y, img.height());

					for (coord_t x = x0 - r; x <= x0 + r; x++)
					{
						coord_t xx = modulo(x, img.width());

						// Make weight positive
						// Taking absolute value may not be as good as clamping to zero. As there should not be
						// any negative values it may be better to clamp, because abs would promote negative noise values
						// to positive weight values.
						//float p = abs(img[yy * w + xx]);
						float32_t p = img(xx, yy, zz);
						if (p < 0)
							p = 0;

						xsum += p * x;
						ysum += p * y;
						zsum += p * z;
						wsum += p;
					}
				}
			}

			if (wsum > 0.00001)
			{
				Vec3d shift = Vec3d(xsum, ysum, zsum) / wsum;// -Vec3d(0.5, 0.5, 0.5);
				return shift;
			}
			else
			{
				if (maxVal > 0)
					return Vec3d(maxPos.x, maxPos.y, maxPos.z);// -Vec3d(0.5, 0.5, 0.5);
				else
					return Vec3d(maxPos.x, maxPos.y, maxPos.z);
			}
			*/

			/*
			// Foroosh method for subpixel accuracy
			// Centroid method seems to give similar results but without a couple of outliers
			// Additionally, this method has bias towards small x, y, and z values.
			// There is probably some problem in the implementation.
			coord_t x0 = maxPos.x;
			coord_t y0 = maxPos.y;
			coord_t z0 = maxPos.z;
			float32_t C000 = img(modulo(x0, img.width()), modulo(y0, img.height()), modulo(z0, img.depth()));
			float32_t C100 = img(modulo(x0 + 1, img.width()), modulo(y0, img.height()), modulo(z0, img.depth()));
			float32_t C010 = img(modulo(x0, img.width()), modulo(y0 + 1, img.height()), modulo(z0, img.depth()));
			float32_t C001 = img(modulo(x0, img.width()), modulo(y0, img.height()), modulo(z0 + 1, img.depth()));
			double dx = forooshHelper(C000, C100, modulo(x0, img.width()), modulo(x0 + 1, img.width()));
			double dy = forooshHelper(C000, C010, modulo(y0, img.height()), modulo(y0 + 1, img.height()));
			double dz = forooshHelper(C000, C001, modulo(z0, img.depth()), modulo(z0 + 1, img.depth()));

			return Vec3d(x0 + dx, y0 + dy, z0 + dz);
			*/
		}

		
	}

	/*
	Calculates shift between img1 and img2 using phase correlation.
	@param maxShift Maximal shift that is considered.
	*/
	Vec3d phaseCorrelation(Image<float32_t>& img1, Image<float32_t>& img2, const Vec3c& maxShift, double& goodness)
	{
		Image<complex32_t> img1FFT;
		Image<complex32_t> img2FFT;

		fft(img1, img1FFT);

		//Image<complex32_t> tmp;
		//set(tmp, img1FFT);
		//normSquared(tmp);
		//complex32_t rg00 = sum<complex32_t, complex32_t>(tmp);


		fft(img2, img2FFT);

		//set(tmp, img2FFT);
		//normSquared(tmp);
		//complex32_t rf00 = sum<complex32_t, complex32_t>(tmp);

		// This works but does not (possibly) produce correct goodness of fit estimate
		conjugate(img2FFT);
		multiply(img1FFT, img2FFT);
		normalize(img1FFT);
		ifft(img1FFT, img1);


		// TODO: Should this be here or not? (Probably not!)
		//abs(img1);

		// For testing
		//raw::writed(img1, "./correlation");

		// Now img1 contains a peak at the location of the shift.
		float32_t maxVal;
		Vec3d shift = internals::findPeak(img1, maxShift, maxVal);

		// This uncertainty estimation is from MatLab code corresponding to paper
		// Manuel Guizar-Sicairos, Samuel T. Thurman, and James R. Fienup, "Efficient subpixel image registration algorithms," Opt.Lett. 33, 156 - 158 (2008).
		// See https://ch.mathworks.com/matlabcentral/fileexchange/18401-efficient-subpixel-image-registration-by-cross-correlation?focused=6461816&tab=function
		// TODO: ...but something goes wrong in this implementation and I'm not able to see the error right now. One difference is that
		// G-S algorithm does not normalize before inverse FFT, but if that step is removed, the shifts are not recognized at correct locations anymore...
		// TODO: The paper contains a sub-pixel registration algorithm, too.
		//CCmax = CC(row_shift, col_shift)*nr*nc;
		//rg00 = sum(abs(buf1ft(:)). ^ 2);
		//rf00 = sum(abs(buf2ft(:)). ^ 2);
		//error = sqrt(1.0 - abs(CCmax). ^ 2 / (rg00*rf00));

		if (maxVal > 0)
		{
			//float32_t CCmax = maxVal * img1.pixelCount();
			//goodness = 1.0 - CCmax * CCmax / (rg00.real() * rf00.real());
			goodness = maxVal;
		}
		else
		{
			goodness = 0;
		}

		return shift;
	}


	namespace tests
	{

		void phaseCorrelation2()
		{
			// NOTE: No asserts!

			Image<uint16_t> tmp;
			raw::readd(tmp, "./t1-head_256x256x129.raw");

			Image<float32_t> reference;
			convert(tmp, reference);

			raw::readd(tmp, "./t1-head_rot_trans_256x256x129.raw");

			Image<float32_t> deformed;
			convert(tmp, deformed);

			
			double goodness;
			Vec3d shift = phaseCorrelation(reference, deformed, 30 * Vec3c(1, 1, 1), goodness);

			cout << shift << endl;


		}

		void phaseCorrelation()
		{
			// NOTE: No asserts!

			Image<uint16_t> head16;
			raw::readd(head16, "./t1-head_256x256x129.raw");

			Image<float32_t> head(head16.dimensions());
			convert(head16, head);
			
			std::default_random_engine generator;

			int maxShift = 64;
			std::uniform_real_distribution<double> distribution(-maxShift, maxShift);

			ofstream out;
			out.open("./fourier/shift_errors.txt");

			cout << "true x,\tmeas x,\tdelta" << endl;
			cout << "true y,\tmeas y,\tdelta" << endl; 
			cout << "true z,\tmeas z,\tdelta" << endl;
			cout << "goodness" << endl;
			for (coord_t n = 0; n < 100; n++)
			{
				Vec3d shiftGT(distribution(generator), distribution(generator), distribution(generator));

				Image<float32_t> headShifted(head.dimensions());
				translate(head, headShifted, shiftGT);

				noise(headShifted, 0, 25);

				raw::writed(headShifted, "./fourier/shifted_noisy_head");

				double goodness;
				Vec3d shift = phaseCorrelation(headShifted, head, maxShift * Vec3c(1, 1, 1), goodness);


				cout << (int)math::round(shiftGT.x) << ",\t" << shift.x << ",\t" << (shift.x - shiftGT.x)  << endl;
				cout << (int)math::round(shiftGT.y) << ",\t" << shift.y << ",\t" << (shift.y - shiftGT.y) << endl;
				cout << (int)math::round(shiftGT.z) << ",\t" << shift.z << ",\t" << (shift.z - shiftGT.z) << endl;
				cout << goodness << endl;
				
				out << shiftGT.x << ", " << shiftGT.y << ", " << shiftGT.z << ", " << shift.x << ", " << shift.y << ", " << shift.z << ", " << goodness << endl;

				showProgress(n, 100);
			}

		}

		void fourierTransformPair()
		{
			Image<uint16_t> head16(256, 256, 129);
			raw::read(head16, "./t1-head_noisy_256x256x129.raw");
			
			Image<float32_t> head(head16.dimensions());
			convert(head16, head);


			Image<complex32_t > ft;
			fft(head, ft);

			Image<float32_t> comp;
			ifft(ft, comp);

			raw::writed(comp, "./fourier/ifft");

			subtract(comp, head);
			abs(comp);
			testAssert(max(comp) < 1e-3, "FFT - inverse FFT pair");
		}

		void dctPair()
		{
			Image<uint16_t> head16(256, 256, 129);
			raw::read(head16, "./t1-head_noisy_256x256x129.raw");

			Image<float32_t> head(head16.dimensions());
			convert(head16, head);

			Image<float32_t> headGT(head16.dimensions());
			convert(head16, headGT);


			dct(head);
			idct(head);

			raw::writed(head, "./fourier/idct");

			subtract(head, headGT);
			abs(head);
			testAssert(max(head) < 1e-3, "DCT - inverse DCT pair");
		}

		void bandpass()
		{
			// NOTE: No asserts

			Image<uint16_t> head16(256, 256, 129);
			raw::read(head16, "./t1-head_noisy_256x256x129.raw");

			Image<float32_t> head(head16.dimensions());
			convert(head16, head);
			
			
			bandpassFilter(head, 4, 40);
			raw::writed(head, "./fourier/bandpass");


			convert(head16, head);
			gaussFilter(head, 4);
			raw::writed(head, "./fourier/gauss");

			//head.init(128, 128, 64);
			//raw::read(head, "./t1-head_noisy_32_128x128x64.raw");

			//gauss(head, 4);
			//raw::writed(head, "./fourier/gauss");
		}
	}
}