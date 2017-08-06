using UnityEngine;
using System.Collections;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public partial class SocketHandler : EventDispatcher
{
    public SocketHandler()
    {

        this.addEventListener((int)PACKET_ID_ENUM.ID_NetLoginRes, this.onLogin);

        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetPlayerInfoNotify, this.onPlayerInfoNotify);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetCreateRoomRes, this.onNetCreateRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetEnterRoomRes, this.onNetEnterRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetLeaveRoomRes, this.onNetLeaveRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetDismissRoomRes, this.onNetDismissRoomRes);
    }

    public int onLogin(object l, object w)
    {
        SocketAngent socket = l as SocketAngent;
        NetLoginRes res = w as NetLoginRes;
        if (res.result == 0)
        {
            LocalPlayer localPlayer = PlayerManager.Instance.CreateLocalPlayer();
            localPlayer.Guid = res.guid;
            localPlayer.AccId = res.accountInfo.id;
            localPlayer.Name = res.accountInfo.user;
            localPlayer.client = GameStart.socketClient;

            Debug.Log("[" + res.accountInfo.user + "] login success");
            PlayerManager.Instance.AddPlayer(localPlayer);

            GameStart.uiMgr.closeWindow();
            //GameStart.uiMgr.openWindow("main_ui");
        }
        return 1;
    }

    //public int onPlayerInfoNotify(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetPlayerInfoNotify res = w as NetPlayerInfoNotify;

    //    int propertyCount = 0;
    //    res.playerInfoStream.Read(ref propertyCount);
    //    for (int i = 0; i < propertyCount; ++i)
    //    {
    //        string propertyName = "";
    //        res.playerInfoStream.Read(ref propertyName);
    //        BinaryStream bit = new BinaryStream();
    //        res.playerInfoStream.Read(bit);

    //        Property property = PlayerManager.playerMgr.localPlayer.getProperty(propertyName);
    //        if (property == null)
    //        {
    //            Type type_ = Type.GetType(propertyName);
    //            if (type_ != null)
    //                property = type_.Assembly.CreateInstance(propertyName) as Property;

    //            if (property != null)
    //                PlayerManager.playerMgr.localPlayer.addProperty(propertyName, property);
    //        }
           
    //        if (property != null)
    //        {
    //            property.deSerialize(bit);
    //        }

    //    }

    //    ViewMainUI viewMainUI = GameStart.uiMgr.getUIPlane("main_ui").GetComponent<ViewMainUI>();
    //    viewMainUI.RefreshInfo(PlayerManager.playerMgr.localPlayer);
    //    return 1;
    //}
}
