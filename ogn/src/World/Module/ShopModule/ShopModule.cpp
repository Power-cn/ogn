#include "stdafx.h"

IMPLEMENT_CLASS(ShopModule, Module);
ShopModule::ShopModule()
{

}

ShopModule::~ShopModule()
{

}

Product* ShopModule::AddProduct(Product* product)
{
	if (FindProduct(product->mInsId)) return NULL;
	mMapProduct[product->mInsId] = product;
	return NULL;
}

Product* ShopModule::FindProduct(uint32 insId)
{
	auto itr = mMapProduct.find(insId);
	if (itr != mMapProduct.end())
		return itr->second;
	return NULL;
}

void ShopModule::DelProduct(uint32 insId)
{
	auto itr = mMapProduct.find(insId);
	if (itr != mMapProduct.end()) {
		delete itr->second;
		mMapProduct.erase(itr);
	}
}

void ShopModule::DoAddProduct(Player* aPlr, uint32 productId)
{
	NetSellProductReq req;
	req.productId = productId;
	Session* ssn = aPlr->getSession();
	if (ssn) {
		ssn->sendPacketToDB(req);
	}
}

void ShopModule::OnSellProduct(Player* aPlr, Product* product)
{
	LuaEngine::Call("shop", "OnSellProduct", aPlr->getUserId(), product->mInsId);
}

bool ShopModule::Initialize()
{
	return true;
}

bool ShopModule::Update(float time, float delay)
{
	return true;
}

bool ShopModule::Destroy()
{
	return true;
}

bool ShopModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool ShopModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;

}

bool ShopModule::onLoad(Player* player, Dictionary& bytes)
{
	return true;
}

bool ShopModule::onSave(Player* player, Dictionary& bytes)
{
	return true;
}

