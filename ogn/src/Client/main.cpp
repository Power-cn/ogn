#include "stdafx.h"

void main()
{
	Dump dump;
	srand(time(NULL));
	INSTANCE(ConfigManager).reloadConfig();

	INSTANCE(RobotManager);
	INSTANCE(SocketHandler);

	double time = DateTime::GetNowAppUS();
	double delay = 0.0;
	double pingTime = 0.0;
	while (1)
	{
		delay = DateTime::GetNowAppUS() - time;
		time += delay;
		time = DateTime::GetNowAppUS();
		pingTime += delay;

		INSTANCE(Network).update(0.f, 0.f);
		INSTANCE(RobotManager).update(time, delay);
		Threader::sleep(1);
	}
}