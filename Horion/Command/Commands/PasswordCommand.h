#pragma once
#include "ICommand.h"
#include <sstream>

#ifdef _BETA
class PasswordCommand : public IMCCommand {
public:
	
	uint32_t matchPW[29] = {3032770428, 3274002126, 163086608, 4230224777, 2039851928, 3675653500, 3328843349, 3328843349, 3885879077, 958491549, 3061013261, 1792958942, 3143304174, 3143304174, 3143304174, 3143304174, 3143304174, 3270400800, 3270400800, 1275806891, 3306539612, 3306539612, 1446547125, 1916298011, 1916298011, 1916298011, 1916298011, 1916298011, 3438163959};

	int tries = 3;
	bool hasChecked = false;

	PasswordCommand();
	~PasswordCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#endif
