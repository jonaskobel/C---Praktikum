#ifndef MANEUVER_H_
#define MANEUVER_H_

class Maneuver {
public:
	Maneuver();
	virtual ~Maneuver();

	void SetManeuver(int type, double radius, double speed);
	void Start();
	void Stop();
	void Proceed();
	bool isRunning();
	
	void Step(double dt); 
	double* GetManeuverSpeed();

    
    double GetTimer() { return dTimer; }
    double GetTargetTime() { 
        return iManeuverType == 2 ? (dTargetTimeLoop1 + dTargetTimeLoop2) : dTargetTimeLoop1; 
    }

private:
	int iManeuverType; 
	bool bIsRunning;
	double dTimer;
    
    
	double dTargetTimeLoop1; 
	double dTargetTimeLoop2; 
	
	double dSpeedL_Loop1, dSpeedR_Loop1; 
	double dSpeedL_Loop2, dSpeedR_Loop2; 
	double adWishSpeed[2];
};

#endif /* MANEUVER_H_ */
