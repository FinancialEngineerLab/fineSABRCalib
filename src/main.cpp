#include <iostream>
#include <ql/quantlib.hpp>

#include "lazyobject3.hpp"


int main(int argc, char *argv[])
{
	{
		try
		{
			boost::shared_ptr<SimpleVolQuote> v1(new VolQuote(0.2,90.0));
			boost::shared_ptr<SimpleVolQuote> v2(new VolQuote(0.191,95.0));
			boost::shared_ptr<SimpleVolQuote> v3(new VolQuote(0.187,100.0));
			boost::shared_ptr<SimpleVolQuote> v4(new VolQuote(0.211,105.0));
			
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
			std::cout << " Inital SABR ATM " << res << std::endl;
			res = Model.getVanillaPrice(90.0);
			res = Model.getVanillaPrice(95.0);
			res = Model.getVanillaPrice(105.0);
			
			v1->setVolatility(0.22);
			v2->setVolatility(0.20);
			v3->setVolatility(0.18);
			v4->setVolatility(0.32);
			
			res=Model.getVanillaPrice(100.0);
			std::cout << " Updated SABR ATM " << res << std::endl;
			
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