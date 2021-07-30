#include "lazyobject.hpp"
#include "BSMobject.hpp"

class SabrModel : public LazyObject
{
	public:
	SabrModel(const boost::shared_ptr<smileQuote>& smile,
	const Real& fwd, const Time& tau, const Real& df, const Real& rd) :
	smile_(smile), fwd_(fwd), tau_(tau), rd_(rd), rf_(rf),
	strikeVec_(std::vector<Real>(smile->getVolNumber())),
	volVec_(std::vector<Real>(smile->getVolNumber()))
	{
	registerWith(smile_);
	}
	
	Real getVanillaPrice(const Real& strike)
	{
		calculate();
		return blackScholesMertonFwd(fwd_,s strike, (*intp_)(strike), rf_,rd_,tau_,1);
	}
	
	private:
	void performCalculations() const
	{
		volQuoteVec_ = smile_->getVolVec();
		for(Size i = 0; i < volQuoteVec_.size(); ++i)
		{
			strikeVec_[i] = (*volQuoteVec_[i]).strike();
			volVec_[i] = (*voQuoteVec_[i]).value();
		}
		
		if(intp_==NULL)
		{
			intp_.reset(new SABRInterpolation(strikeVec_.begin(), strikeVec_.end(),
			volVec_.begin(), tau_, fwd_, 0.1,1.0,0.1,0.1, false, false, false, false));
		}
		intp_->update();
		std::cout << "Recalibration Performed!" <<std::endl;
	}
	
	Real fwd_, rf_, rd_;
	Time tau_;
	boost::shared_ptr<SmileQuote> smile_;
	
	mutable boost::shared_ptr<SABRInterpolation> intp_;
	mutable std::vector<Real> strikeVec_, volVec_;
	mutable std::vector<boost::shared_ptr<VolQuote> > volQuoteVec_;
};