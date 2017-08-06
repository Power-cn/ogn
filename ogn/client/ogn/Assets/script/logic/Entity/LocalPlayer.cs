using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class LocalPlayer : Player {
    public SocketClient client;
    public string user;
    public string password;
    public LocalPlayer()
    {
    }

    public override void  SendPacket(Packet packet)
    {
        if (client == null) return;
        GameStart.gameStart.SendPacket(packet, client);
    }

}
