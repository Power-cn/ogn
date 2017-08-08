using UnityEngine;
using UnityEngine.UI;

using System.Collections;
using System;
using System.Collections.Generic;


public class GameStart : MonoBehaviour {

    public static GameStart gameStart = new GameStart();
    public static SocketClient socketClient;
    public static Network network = Network.Instance;
    public static SocketHandler sHandler = new SocketHandler();
    public static PlayerManager sPlrMgr = PlayerManager.Instance;
    public static ConfigManager sCfgMgr = ConfigManager.Instance;
    public static PacketHelper sPckMgr = PacketHelper.instance;
    public static UIManager uiMgr;

    public static GameObject root;
    public static GameObject roomItem;
    //public static Camera uiCamera;
    public static Camera mainCamera;
    public static Canvas canvas;
    public static Text textDebug;
    public static ConfigManager config;
    public static GameObject castSprite;
    public static float cell = 0.5f;
    public static Download download;
	// Use this for initialization

    public void SendPacket(Packet packet, SocketEntity socket)
    {
        byte[] send_data = new byte[Shared.MAX_PACKET_LENGTH];
        BinaryStream bit = new BinaryStream(send_data);
        packet.serialize(bit);
        Shared.XOR(bit.buffer, bit.wpos, ConstDef.sKey);
        socket.SendBuffer(bit.buffer, bit.wpos);
    }

    void onDownload()
    {

        config = ConfigManager.Instance;
        root = GameObject.Find("GameStart");
        //uiCamera = GameObject.Find("Canvas/UICamera").GetComponent<Camera>();
        uiMgr = gameObject.AddComponent<UIManager>();
        //canvas = GameObject.Find("Canvas").GetComponent<Canvas>();
        //uiMgr.openWindow("ui_login");

        //textDebug = uiCamera.transform.Find("Debug").GetComponent<Text>();

        //mainCamera = AssetManager.Instance.getGameObjectAsset("prefab/Main Camera").GetComponent<Camera>();
        //castSprite = AssetManager.Instance.getGameObjectAsset("prefab/CastSprite");
        //mainCamera.transform.parent = null;
        //castSprite.transform.parent = mainCamera.transform;
        //GameObject main_ui = AssetManager.Instance .getGameObjectAsset("prefab/main_ui");
        //roomItem = AssetManager.Instance .getAsset("prefab/roomItem");

        //main_ui.AddComponent<ViewMainUI>();
        //main.transform.parent = root.transform;
        //main_ui.transform.parent = uiCanvas.transform;

        //socketClient = Network.Connect("112.74.49.74", 13380);

        //ServerConfig cf = ConfigManager.Instance.getServerConfig("OGN");
       
    }

	void Start () {

        //download = gameObject.AddComponent<Download>();
        //download.func = onDownload;
        onDownload();
	}

	// Update is called once per frame
	void Update () {
        network.update();
        World.Instance.update();
        //Handles.DrawLine

        //for (int i = 0; i < 100; ++i )
        //    Handles.DrawLine(new Vector3(0, -i * cell, 0), new Vector3(100 * cell, -i * cell, 0));

        //for (int i = 0; i < 100; ++i)
        //    Handles.DrawLine(new Vector3(i * cell, 0, 0), new Vector3(i * cell, -100 * cell, 0));

	}

    public void Connect(string host, short port)
    {
        if (socketClient != null)
        {
            reConnect();
            socketClient.addEventListener(SocketEvent.CONNECT, this.onConnect);
            socketClient.addEventListener(SocketEvent.RECV, this.onRecv);
            socketClient.addEventListener(SocketEvent.EXIT, this.onExit);
            socketClient.addEventListener(SocketEvent.EXCEPTION, this.onException);
            return;
        }
        socketClient = network.Connect(host, port);
        socketClient.addEventListener(SocketEvent.CONNECT, this.onConnect);
        socketClient.addEventListener(SocketEvent.RECV, this.onRecv);
        socketClient.addEventListener(SocketEvent.EXIT, this.onExit);
        socketClient.addEventListener(SocketEvent.EXCEPTION, this.onException);

    }
    public void reConnect()
    {
        network.reConnect(socketClient);
    }

    protected int onConnect(EventTarget e)
    {
        Debug.Log("Connect Successful...");

        return 0;
    }

    protected int onExit(EventTarget e)
    {
        this.reConnect();

        Debug.Log("Disconnect...");

        return 0;
    }

    protected int onException(EventTarget e)
    {
        SocketEvent se = e as SocketEvent;
        this.reConnect();
        Debug.Log(System.Text.Encoding.Default.GetString(se.data));
        return 0;
    }

    protected int onRecv(EventTarget e)
    {
        SocketEvent se = e as SocketEvent;
        Shared.XOR(se.data, se.data.Length,  ConstDef.sKey);
        byte[] msgIdBytes = new byte[4];
        Array.Copy(se.data, 0, msgIdBytes, 0, 4);

        int MsgId = BitConverter.ToInt32(msgIdBytes, 0); ;
        MsgId = BinaryStream.ntohl(MsgId);

        //if (MsgId != (int)PACKET_ID_ENUM.ID_NetRegisterRes && MsgId != (int)PACKET_ID_ENUM.ID_NetLoginRes && PlayerManager.playerMgr.localPlayer == null)
        //    return 1;

        Packet packet = sPckMgr.AllocPacket(MsgId);
        if (packet == null) return 0;

        BinaryStream bit = new BinaryStream(se.data);
        packet.deSerialize(bit);
        sHandler.dispatchEvent(packet.MsgID, socketClient, packet);

        return 0;
    }

}
