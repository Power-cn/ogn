#pragma once

class Reference
{
public:
	Reference(void);
	virtual ~Reference(void);
	void incRef();
	void decRef();
protected:
	void deleteThis();
protected:
	int mCount;
};

