#include "Shared.hpp"
#include "Application.h"

int main(){
	Dump dmp;
	INSTANCE(Application).Initialize();
	while (1)
	{
		INSTANCE(Application).Update();
		Threader::sleep(1);
	}
	INSTANCE(Application).Destroy();
	return 0;
}