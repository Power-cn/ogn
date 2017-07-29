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

Session* SessionManager::createSession(Socket* s, uint64 sessionId)
{
	Session* session = newSession(s, sessionId);
	return addSession(session);
}

Session* SessionManager::newSession(Socket* s, uint64 ssnId /*= 0*/)
{
	Session* session = new Session(s);
	if (ssnId != 0)
		session->setSessionId(ssnId);
	else
		session->setSessionId(++Session::sId);
	return session;
}

Session* SessionManager::getSession(uint64 sessionId)
{
	auto itr = mMapSession.find(sessionId);
	if (itr != mMapSession.end())
		return itr->second;

	return NULL;
}

Session* SessionManager::addSession(Session* session)
{
	auto itr = mMapSession.find(session->sessionId);
	if (itr != mMapSession.end())
		return NULL;
	mMapSession.insert(std::make_pair(session->sessionId, session));
	return session;
}

void SessionManager::removeSession(uint64 sessionId)
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

Entity* SessionManager::getEntity(uint32 unitId)
{
	auto itr = mMapEntity.find(unitId);
	if (itr != mMapEntity.end())
		return itr->second;

	return NULL;
}

Entity* SessionManager::addEntity(Entity* unit)
{
	auto itr = mMapEntity.find(unit->unitId);
	if (itr != mMapEntity.end())
		return NULL;

	mMapEntity.insert(std::make_pair(unit->unitId, unit));
	return unit;
}

void SessionManager::removeEntity(uint32 unitId)
{
	auto itr = mMapEntity.find(unitId);
	if (itr != mMapEntity.end())
	{
		delete itr->second;
		mMapEntity.erase(itr);
	}
}

Player* SessionManager::getPlayer(uint32 playerId)
{
	auto itr = mMapPlayer.find(playerId);
	if (itr != mMapPlayer.end())
		return itr->second;

	return NULL;
}

Player* SessionManager::addPlayer(Player* player)
{
	addEntity(player);
	auto itr = mMapPlayer.find(player->playerId);
	if (itr != mMapPlayer.end())
		return NULL;

	mMapPlayer.insert(std::make_pair(player->playerId, player));
	return player;
}

void SessionManager::removePlayer(uint32 playerId)
{
	auto itr = mMapPlayer.find(playerId);
	if (itr != mMapPlayer.end())
	{
		Player* plr = itr->second;
		if (plr)
			removeEntity(plr->unitId);
		else
			delete itr->second;
		mMapPlayer.erase(itr);
	}
}

Session* SessionManager::addSessionsBySocket(uint32 socketId, Session* session)
{
	addSession(session);
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
		if (sset.size() <= 0)
			mMapSocketSessions.erase(itr);
	}
	removeSession(session->getSessionId());
}

Socket* SessionManager::addSktsBySsn(uint64 sessionId, Socket* skt)
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

std::set<Socket*>* SessionManager::getSktsBySsn(uint64 sessionId)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
		return &itr->second;

	return NULL;
}

void SessionManager::removeSktsBySsn(uint64 sessionId, Socket* skt)
{
	auto itr = mMapSessionSockets.find(sessionId);
	if (itr != mMapSessionSockets.end())
	{
		std::set<Socket*>& sset = itr->second;
		sset.erase(skt);
		if (sset.size() <= 0)
			mMapSessionSockets.erase(itr);
	}
}

