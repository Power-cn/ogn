#include "Shared.hpp"



using std::string;
using std::vector;
using std::sregex_iterator;
using std::regex;
using std::sub_match;
using std::cin;
using std::cout;
using std::endl;


class CmdThreadProcessor : public ThreadProcessor
{
public:
	CmdThreadProcessor(CmdDispatcher* cmd);
protected:
	unsigned int ThreadProcess(Threader* pThread);
private:
	CmdDispatcher*				mCmdDispatcher;
};

CmdThreadProcessor::CmdThreadProcessor(CmdDispatcher* cmd)
{
	mCmdDispatcher = cmd;
}


void SplitParams(std::string& cmdline, std::string& cmd, std::vector<std::string>& list) {
	// splits the commandline into cmd & paramlist
	std::regex splitPattern("(\")(.*)(\\1)|([^,;\\s]+)");
	std::match_results<string::const_iterator> results;
	sregex_iterator itor(cmdline.begin(), cmdline.end(), splitPattern);
	sregex_iterator itor_end;
	string param;
	bool isParam = false;
	cmd = cmdline;
	/* 由于c++ regex库是DFA模型, 不支持前向断言预判, 只能使用子串匹配模式 */
	for (; itor != itor_end; itor++) {
		// index 2 means find quoted string 
		const sub_match<string::const_iterator> &submatch = (*itor)[2];
		param = (submatch.matched) ? submatch.str() : itor->str(0);
		if (isParam)
			list.push_back(param);
		else
		{
			cmd = param;
			isParam = true;
		}
	}
}


unsigned int CmdThreadProcessor::ThreadProcess(Threader* pThread)
{
	while (pThread->isActive())
	{
		std::string cmdline;
		getline(std::cin, cmdline, '\n');
		CmdExecute* cmdExecute = new CmdExecute;
		SplitParams(cmdline, cmdExecute->cmd, cmdExecute->params);
		mCmdDispatcher->mQueueCmd.push(cmdExecute);
		Threader::sleep(1);
	}
	return 0;
}

CmdDispatcher::CmdDispatcher()
{
	m_pThreadProcessor = new CmdThreadProcessor(this);
	m_pThread = Threader::createThread(m_pThreadProcessor);
}


CmdDispatcher::~CmdDispatcher()
{
	m_pThread->eixt();
	SAFE_DELETE(m_pThread);
	SAFE_DELETE(m_pThreadProcessor);

	while (mQueueCmd.size() > 0)
	{
		delete mQueueCmd.front();
		mQueueCmd.pop();
	}
}

void CmdDispatcher::update(float32 time, float32 delay)
{
	CmdExecute* cmd = NULL;
	m_mutex.lock();
	if (mQueueCmd.size() <= 0)
	{
		m_mutex.unlock();
		return;
	}

	cmd = mQueueCmd.front();
	mQueueCmd.pop();
	m_mutex.unlock();
	
	CmdEvent e;
	e.event = cmd->cmd;
	e.cmdExecute = cmd;
	this->dispatch(e);
	delete cmd;
}
