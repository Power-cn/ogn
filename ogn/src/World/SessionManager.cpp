#include "stdafx.h"

SessionManager::SessionManager()
{

}

SessionManager::~SessionManager()
{
	for ( auto itr: mMapSession)
		delete itr.second;
	mMapSession.clear();
}

Session* SessionManager::createSession(Socket* s, SessionID sessionId)
{
	Session* session = new Session(s);
	if (sessionId != 0)
		session->setSessionId(sessionId);
	else
		session->setSessionId(++Session::sId);

	return addSession(session);
}

Session* SessionManager::getSession(SessionID sessionId)
{
	auto itr = mMapSession.find(sessionId);
	if (itr != mMapSession.end())
		return itr->second;

	return NULL;
}

Session* SessionManager::addSession(Session* session)
{
	auto itr = mMapSession.find(session->getSessionId());
	if (itr != mMapSession.end())
		return NULL;
	mMapSession.insert(std::make_pair(session->getSessionId(), session));
	return session;
}

void SessionManager::removeSession(SessionID sessionId)
{
	auto itr = mMapSession.find(sessionId);
	if (itr != mMapSession.end())
	{
		delete itr->second;
		mMapSession.erase(itr);
	}
		
}

Session* SessionManager::addSessionBySocket(uint32 socketId, Session* session)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr != mMapSocketSession.end())
		return NULL;
	mMapSocketSession.insert(std::make_pair(socketId, session));
	return session;
}

Session* SessionManager::getSessionBySocket(uint32 socketId)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr == mMapSocketSession.end())
		return NULL;

	return getSession(itr->second->getSessionId());
}

void SessionManager::removeSessionBySocket(uint32 socketId)
{
	auto itr = mMapSocketSession.find(socketId);
	if (itr == mMapSocketSession.end())
		return;
	mMapSocketSession.erase(itr);
}

Session* SessionManager::addSessionsBySocket(uint32 socketId, Session* session)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
	{
		std::set<Session*>& sset = itr->second;
		if (sset.find(session) == sset.end())
			sset.insert(session);
		else
			return NULL;
	}
	else
	{
		std::set<Session*> sset;
		sset.insert(session);
		mMapSocketSessions.insert(std::make_pair(socketId, sset));
	}
	return session;
}

std::set<Session*>* SessionManager::getSessionsBySocket(uint32 socketId)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
		return &itr->second;

	return NULL;
}

void SessionManager::removeSessionsBySocket(uint32 socketId, Session* session)
{
	auto itr = mMapSocketSessions.find(socketId);
	if (itr != mMapSocketSessions.end())
	{
		std::set<Session*>& sset = itr->second;
		sset.erase(session);
	}
}

Socket* SessionManager::addSktsBySsn(SessionID sessionId, Socket* skt)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
	{
		std::set<Socket*>& sset = itr->second;
		if (sset.find(skt) == sset.end())
			sset.insert(skt);
		else
			return NULL;
	}
	else
	{
		std::set<Socket*> sset;
		sset.insert(skt);
		mMapSessionSockets.insert(std::make_pair(sessionId, sset));
	}
	return skt;
}

std::set<Socket*>* SessionManager::getSktsBySsn(SessionID sessionId)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
		return &itr->second;

	return NULL;
}

void SessionManager::removeSktsBySsn(SessionID sessionId, Socket* skt)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
	{
		std::set<Socket*>& sset = itr->second;
		sset.erase(skt);
	}
}

