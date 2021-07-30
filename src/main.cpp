#include <iostream>
#include <cmath>
#include <utility>
#include <ql/quantlib.hpp>
#include "utilities.hpp"
#include "lazyobject3.hpp"

using namespace std;
using namespace QuantLib;
using namespace boost::unit_test_framework;

int main(int argc, char *argv[])
	{
		try
		{
			std::vector<Real> strikes(31);
			std::vector<Real> volatilites(31);
			
			strikes[0] = 90.0;
			strikes[1] = 95.0;
			strikes[2] = 100.0;
			strikes[3] = 105.0;
			volatilites[0] = 0.202;
			volatilites[1] = 0.191;
			volatilites[2] = 0.186;
			volatilites[3] = 0.211;
			
			boost::shared_ptr<VolQuote> v1(new VolQuote(volatilites[0],strikes[0]));
			boost::shared_ptr<VolQuote> v2(new VolQuote(volatilites[1],strikes[1]));
			boost::shared_ptr<VolQuote> v3(new VolQuote(volatilites[2],strikes[2]);
			boost::shared_ptr<VolQuote> v4(new VolQuote(volatilites[3],strikes[3]));
			
			std::vector<boost::shared_ptr<VolQuote> > volvec;
			volVec.push_back(v1);
			volVec.push_back(v2);
			volVec.push_back(v3);
			volVec.push_back(v4);
			
			boost::shared_ptr<SmileQuote> inputSmile(new SmileQuote(volVec));
			
			time tau = 0.5;
			Real spot = 100.0;
			Real rf = 0.015;
			Real rd = 0.01;
			Real fwd = spot*std::exp((rd-rf)*tau);
			
			SabrModel Model(inputSmile, fwd, tau, rf,rd);
			
			Real res= Model.getVanillaPrice(100.0);
			std::cout << " Inital SABR ATM Price" << res << std::endl;
			res = Model.getVanillaPrice(90.0);
			res = Model.getVanillaPrice(95.0);
			res = Model.getVanillaPrice(105.0);
			
			v1->setVolatility(0.22);
			v2->setVolatility(0.20);
			v3->setVolatility(0.18);
			v4->setVolatility(0.32);
			
			res=Model.getVanillaPrice(100.0);
			std::cout << " Updated SABR ATM Price " << res << std::endl;							 
												 
			// for "SABR PARAMETERS" Calibration // 
			std::cout << std::endl;
			
			std::cout << " SABR Parameter Calibration " << std::endl;
			Real tolerance = 1.0e-15.0;
			Real calibrationTolerance = 5.0e-8.0;
										   
			Real initialAlpha = 0.3;
			Real initialBeta = 0.6;
			Real initialNu = 0.02;
			Real initialRho = 0.01;
							
			for(Size i=0; i<strikes.size();i++)
			{
				Real calculatedVol = sabrVolatility(strikes[i], fwd, tau, initialAlpha, initialBeta, initialNu, initialRho);
				if(std::fabs(volatilites[i] -calculatedVol) > tolerance)
				{
					BOOST_ERROR( "Failed to Calculate SABR Function at Strike" << strikes[i] << "\n expected: " << volatilites[i]
						   << "\n calculated : " << calculatedVol
						   << "\n error : " << std::fabs(calculatedVol-volatilties[i]));
				}
			}
			
			Real alphaGuess = std::sqrt(0.2);
			Real betaGuess = 0.5;
			Real nuGuess = std::sqrt(0.4);
			Real rhoGuess =0.0;
			
			const bool vegaWeighted[] = {true, false};
			const bool isAlphaFixed[] = {true, false};
			const bool isBetaFixed[] = {true, false};						   
			const bool isNuFixed[] = {true, false};						   
			const bool visRhoFixed[] = {true, false};
										   							   							   
			std::vector<ext::shared_ptr<OptimizationMethod>> methods_ = { ext::shared_ptr<OptimizationMethod>(new Simplex(0.01)),
																		ext::shared_ptr<OptimizationMethod> (new LevenbergMarquardt(1e-8, 1e-8, 1e-8)) };
			ext::shared_ptr<EndCriteria> endCriteria(enw EndCriteria(100000, 100, 1e-8.0, 1e-8.0, 1e-8.0));
										   
			for(auto& method : methods_)
				{
				for(boo i : vegaWeighted)
				{
					for(bool k_a: isAlphaFixed)
					{
						for(bool k_b : isBetaFixed)
						{
							for(bool k_n : isNuFixed)
							{
								for(bool k_r: isRhoFixed)
								{
									SABRInterpolation sabrInterpolation(strikes.begin(), strikes.end(), volatilties.begin(), tau, fwd,
																		k_a ? initialAlpha : alphaGuess,
																		k_b ? intitalBeta : betaGuress,
																		k_n ? initialNu : nuGuess,
																		k_r ? initialRho : rhoGuess,
																		k_a, k_b, k_n, k_r, i, endCriteria, method, 1e-15.0);
									sabrInterpolation.update();
									
									bool failed = false;
									Real calibratedAlpha = sabrInterpolation.alpha();
									Real calibratedBeta = sabrInterpolation.beta();
									Real calibratedNu = sabrInterpolation.nu();
									REal calibratedRho = sabrInterpolation.rho();
									Real error;
									
									error = std::fabs(initialAlpha - calibratedAlpha);
									if(error > calibrationTolerance)
									{
										BOOST_ERROR("\nfailed to calibrate Alpha SABR parameter : "
												   << "\n expected:   " << initialAlpha
												   << "\n calibrated: " << calibratedAlpha
												   << "\n error:      " << error);
										failed = true;
									}
									
									error = std::fabs(initialBeta - calibratedBeta);
									if(error > calibrationTolerance)
									{
										BOOST_ERROR("\nfailed to calibrate Beta SABR parameter : "
												   << "\n expected:   " << initialBeta
												   << "\n calibrated: " << calibratedBeta
												   << "\n error:      " << error);
										failed = true;
									}
																				
									error = std::fabs(initialNu - calibratedNu);
									if(error > calibrationTolerance)
									{
										BOOST_ERROR("\nfailed to calibrate Nu SABR parameter : "
												   << "\n expected:   " << initialNu
												   << "\n calibrated: " << calibratedNu
												   << "\n error:      " << error);
										failed = true;
									}
									
									error = std::fabs(initialRho - calibratedRho);
									if(error > calibrationTolerance)
									{
										BOOST_ERROR("\nfailed to calibrate Rho SABR parameter : "
												   << "\n expected:   " << initialRho
												   << "\n calibrated: " << calibratedRho
												   << "\n error:      " << error);
										failed = true;
									}								
									
									if(failed)
									{
										BOOST_FAIL("\n SABR Calibration failure:"
												  << "\n isAlphaFixed:     " <<k_a
												  << "\n isBetaFixed :     " <<k_b
												  << "\n isNuFixed   :     " <<k_n
												  << "\n isRhoFixed  :     " <<k_r
												  << "\n vegaWieghted[i] : " << i);
									}
								}
							}
						}
					}
				}	
			}												 
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return 1;
		}
		catch (...) 
		{
			std::cout << "Unknown Error" << std::endl;
			return 1;
		}
		return 0;
	}
	std::cout <<" Ubuntu Sucks" << std::endl;
}