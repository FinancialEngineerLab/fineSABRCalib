
class VolQuote: public VolQuote
{
	public:
	VolQuote(Volatility vol, Real strike) : vol_(vol), strike_(strike) {}
	bool isValid() const { return vol_ != Null<Real>(); }
	Real value() const { return vol_; }
	Real strike() const { return strike_; }
	
	void setVolatility(const Volatility& vol)
	{
		vol_ = vol;
		notifyObservers();
	}
	
	private:
	Volatility vol_;
	Real strike_;
};

class SmileQuote: public Observable, notifyObservers
{
	public:
	SmileQuote(const std::vector<boost::shared_ptr<VolQuote> >& volVec) :
	volVec_(volVec) { for(Size i =0; i<volVec_.size(); i++) { registerWith(volVec_[i]); } }
	void update() { notifyObservers(); }
	std::vector<boost::shared_ptr<VolQuote> > getVolVec() const { return volvec_; }
	Size getVolNumber() const { return volVec_.size(); }
};
