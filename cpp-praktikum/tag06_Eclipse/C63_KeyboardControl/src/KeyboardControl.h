#ifndef KEYBOARDCONTROL_H_
#define KEYBOARDCONTROL_H_

#include <ncurses.h>
#include "InterfaceHW.h"

class KeyboardControl {
public:
	KeyboardControl();
	virtual ~KeyboardControl();
	void Communicate();
	void Step();
	static KeyboardControl* transferPointer;
	static void transferFunction ();

private:
	InterfaceHW interface;


	double dVSoll[2];
	double dVIst[2];
	int iMicros[2];
	char cEingabe;
};

#endif /* KEYBOARDCONTROL_H_ */
