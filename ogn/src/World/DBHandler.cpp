#include "stdafx.h"

DBHandler::DBHandler()
{

}

DBHandler::~DBHandler()
{

}

void DBHandler::doRegister()
{
	RegDBEvent(ID_NetLoginRes, &DBHandler::onNetNetLoginRes, this);
	RegDBEvent(ID_NetCreateRoleRes, &DBHandler::onNetCreateRoleRes, this);
	RegDBEvent(ID_NetSelectRoleRes, &DBHandler::onNetSelectRoleRes, this);
	RegDBEvent(ID_NetQueryRoleRes, &DBHandler::onNetQueryRoleRes, this);
	RegDBEvent(ID_NetSelectRoleRes, &DBHandler::onNetSelectRoleRes, this);
}

int32 DBHandler::onNetNetLoginRes(Session* session, NetLoginRes* res)
{
	uint32 accId = res->accInfo.id;
	if (res->result != NResultSuccess)
	{
		session->sendPacketToWorld(*res);
		return 0;
	}

	Player* player = sWorld.FindPlrByAccId(accId);
	do 
	{
		if (player) {
			Session* oldSession = player->getSession();
			if (oldSession)
			{
				player->sendRespnoseMsg(MC_LoginTheNumberInTheRemote);
				Dictionary dict;
				sApp.onLeaveWorld(player, dict);

				NetLoginRes res;
				res.result = 3;
				oldSession->sendPacketToWorld(res);
				player->unbindSession();

				sApp.doSessionLeaveWorld(oldSession);
			}
			player->SetOnline(true);
			player->bindSession(session);
			break;
		}

		player = new Player;
		player->SetOnline(true);
		player->bindSession(session);
		player->setAccId(res->accInfo.id);
		player->setUser(res->accInfo.user);
		sWorld.addPlayerByAccId(player);
	} while (false);
	res->guid = player->getGuid();
	player->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetCreateRoleRes(Player* aPlr, NetCreateRoleRes* res)
{
	//////////////////////////////////////////////////////////////////////////
	/// 角色创建成功 ///
	aPlr->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetSelectRoleRes(Player* aPlr, NetSelectRoleRes* res)
{
	if (res->result == NResultFail) {
		aPlr->sendPacket(*res);
		return 0;
	}

	aPlr->sendPacket(*res);

	aPlr->SetOfflineTimer(DateTime::Now());

	DBRoleInfo& info = res->roleInfo;
	aPlr->setUserId(info.id);
	aPlr->setName(info.name);
	aPlr->SetOnlineTimer(DateTime::Now());
	sWorld.addPlayerToUserId(aPlr);
	sWorld.addPlayerByName(aPlr);

	Dictionary dict;
	if (info.property.wpos() <= 0)
		aPlr->DoCreateCharacter(dict, info);
	else
		info.property >> dict;

	sApp.onLoad(aPlr, dict);
	sApp.onEnterWorld(aPlr, dict);
	return 0;
}

int32 DBHandler::onNetQueryRoleRes(Player* aPlr, NetQueryRoleRes* res)
{
	aPlr->sendPacket(*res);
	return 0;
}

int32 DBHandler::onNetSellProductRes(Player* aPlr, NetSellProductRes* res)
{
	if (res->result == NResultFail)
	{
		aPlr->sendPacket(*res);
		return 0;
	}
	Product* product = new Product;
	product->mInsId = res->productInfo.productInsId;
	product->mProductId = res->productInfo.productId;
	product->mUserId = res->productInfo.sellUserId;
	product->mBuyUserId = res->productInfo.buyUserId;
	product->mShelvesTime = res->productInfo.shelvesTime;
	product->mUnshelvesTime = res->productInfo.unShelvesTime;
	if (sShop.AddProduct(product) == NULL)
		delete product;
	aPlr->sendPacket(*res);
	return 0;
}
