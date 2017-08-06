using System.Collections;

using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Collections.Generic;
using System.IO;
using System;
using ComponentAce.Compression.Libs.zlib;

public class SocketEvent : EventTarget
{
	public static string CONNECT = "onConnect";
    public static string ACCEPT = "onAccept";
    public static string RECV = "onRecv";
    public static string SEND = "onSend";
    public static string EXIT = "onExit";
    public static string EXCEPTION = "onException";

    public byte[] data = null;
}

public class Header
{
    public virtual bool serialize(BinaryStream bitStream)
    {
        onSerializeHeader(bitStream);
        OnSerialize(bitStream);
        return true;
    }
    public virtual bool deSerialize(BinaryStream bitStream)
    {
        onDeserializeHeader(bitStream);
        OnDeserialize(bitStream);
        return true;
    }
    protected virtual bool onSerializeHeader(BinaryStream bitStream)
    {
        return true;
    }

    protected virtual bool onDeserializeHeader(BinaryStream bitStream)
    {
        return true;
    }

    protected virtual bool OnSerialize(BinaryStream bitStream)
    {
        return true;
    }

    protected virtual bool OnDeserialize(BinaryStream bitStream)
    {
        return true;
    }
}

public class Packet : Header
{
    public int mMsgId;
    public int mVersion;
    public Packet(int msgId)
    {
        mMsgId = msgId;
    }

    public int MsgID
    {
        get { return mMsgId; }
    }

    protected override bool onSerializeHeader(BinaryStream bitStream)
    {
        bitStream.Write(mMsgId);
        bitStream.Write(mVersion);
        return true;
    }

    protected override bool onDeserializeHeader(BinaryStream bitStream)
    {
        bitStream.Read(ref mMsgId);
        bitStream.Read(ref mVersion);
        return true;
    }
}

public class PacketHelper
{

    public static PacketHelper instance = new PacketHelper();
    public Object register = null;
    public PacketHelper()
    {
        m_dict = new Dictionary<int, string>();
        m_dict_helper = new Dictionary<int, PacketHandler>();
    }

    public void RegisterPacket(int MsgId, string type_name)
    {
        m_dict.Add(MsgId, type_name);
        string handler_name = type_name + "Handler";
        Type type_ = Type.GetType(handler_name);
        if (type_ != null)
        {
            object obj = type_.Assembly.CreateInstance(handler_name);
            if (obj != null)
            {
                PacketHandler helper = obj as PacketHandler;
                if (helper != null)
                    m_dict_helper.Add(MsgId, helper);
            }
        }
    }

    public Packet AllocPacket(int MsgId)
    {
        if (register == null)
        {
            Type type_ = Type.GetType("PacketRegister");
            if (type_ != null)
                register = type_.Assembly.CreateInstance("PacketRegister");
        }

        string type_name = null;
        if (m_dict.ContainsKey(MsgId))
            type_name = m_dict[MsgId];

        if (type_name != null)
        {
            object obj = Type.GetType(type_name).Assembly.CreateInstance(type_name);
            if (obj != null)
                return obj as Packet;
        }

        return null;
    }

    public PacketHandler FindPacket(int MsgId)
    {
        if (m_dict_helper.ContainsKey(MsgId))
            return m_dict_helper[MsgId];

        return null;
    }

    Dictionary<int, string> m_dict;
    Dictionary<int, PacketHandler> m_dict_helper;
}

public class PacketHandler
{
    public virtual void OnHandler(Packet packet) {}
}

public class SocketAngent : EventDispatcher
{
    public Network network;
    public Socket socket = null;
    public int offset = 0;
    public int packet = 0;
    public int position = 0;
    public byte[] recvPacket = new byte[4096];
    public byte[] countBytes = new byte[4];
    public IPEndPoint ipPoint;
    public Queue<BinaryStream> sendQueue = new Queue<BinaryStream>();
    public CircleBuffer buffer = new CircleBuffer();
    public bool sending = false;

    public void unCompression(byte[] input, int inCount, ref byte[] output, ref int outCount)
    {
        MemoryStream mOut = new MemoryStream(input, 0, inCount);
        ZInputStream zOut = new ZInputStream(mOut);
        
        int tatolCount = 0;
        int count = 0;
        byte[] bytes = new byte[1024];
        output = new byte[256];

        while (true)
        {
            count = zOut.read(bytes, 0, bytes.Length);
            if (count <= 0)
                break;
            if (output.Length < tatolCount + count)
            {
                byte[] newBytes = new byte[tatolCount + count];
                Array.Copy(output, 0, newBytes, 0, tatolCount);

                Array.Copy(bytes, 0, newBytes, tatolCount, count);
                output = newBytes;
            }
            else
            {
                Array.Copy(bytes, 0, output, tatolCount, count);
            }
            tatolCount += count;
        }
        outCount = tatolCount;
        mOut.Close();

        //Console.WriteLine("unCompression: in {0} out {1}", inCount, outCount);
    }

    public void compression(byte[] input, int inCount, ref byte[] output, ref int outCount)
    {
        MemoryStream mOut = new MemoryStream();
        ZOutputStream outZStream = new ZOutputStream(mOut, zlibConst.Z_DEFAULT_COMPRESSION);
        outZStream.Write(input, 0, inCount);
        outCount = (int)outZStream.TotalOut;
        output = mOut.GetBuffer();
        mOut.Close();

        //Console.WriteLine("compression: in {0} out {1}", inCount, outCount);
    }


    public void Send(Packet packet)
    {
        byte[] send_data = new byte[4096];

        BinaryStream bit = new BinaryStream(send_data);
        packet.serialize(bit);
        int length = bit.WriteIndex + 4;
        byte[] send_packet = new byte[length];

        this.Send(bit.buffer, bit.WriteIndex);
    }

    public void Send(byte[] data)
    {
        this.Send(data, data.Length);
    }

    public void Send(byte[] data, int length, int index = 0)
    {
        BinaryStream send_bit = new BinaryStream(length + 4);
        send_bit.Write(length + 4);
        send_bit.Write(data, 0, length);

        sendQueue.Enqueue(send_bit);

        postSend();
    }



    protected void OnSend(IAsyncResult ar)
    {
        try
        {
            int BytesSend = socket.EndSend(ar);
            if (BytesSend > 0)
                sending = false;

            postSend();

            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.SEND;
            if (network != null)
                network.dispatchEvent(this, e);
        }
        catch (SocketException ex)
        {
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXCEPTION;
            e.data = System.Text.Encoding.Default.GetBytes(ex.Message);
            if (network != null)
                network.dispatchEvent(this, e);
        }
    }

    public bool postSend()
    {
        if (sending)
            return false;
        if (sendQueue.Count <= 0)
        {
            sending = false;
            return false;
        }
        int sendCount = 0;
        Network.sPacketBuffer.Clear();
        while (true)
        {
            if (sendQueue.Count <= 0)
                break;
            BinaryStream packet = sendQueue.Peek();
            if (sendCount + packet.WriteIndex > BinaryStream.PACKET_MAX_LENGTH)
                break;
            Network.sPacketBuffer.Write(packet.buffer, 0, packet.WriteIndex);
            sendCount += packet.WriteIndex;
            sendQueue.Dequeue();
        }

        try
        {
            sending = true;
            this.socket.BeginSend(Network.sPacketBuffer.buffer, 0, Network.sPacketBuffer.WriteIndex, SocketFlags.None, new AsyncCallback(this.OnSend), this);
        }
        catch
        {
            sending = false;
            return false;
        }
        return true;
    }
}

public class SocketClient : SocketAngent
{

}



public class Network 
{
    public static BinaryStream sPacketBuffer = new BinaryStream();
    PacketHelper m_helper;
    public delegate void PacketHandler_CallBack(Packet pt);

    protected Queue<SocketEvent> eventList = new Queue<SocketEvent>();
    protected Queue<SocketAngent> clientList = new Queue<SocketAngent>();

    private EventWaitHandle handle = new EventWaitHandle(true, EventResetMode.AutoReset);

    public Network()
    {
        m_helper = new PacketHelper();
        //PacketRegister register = new PacketRegister(m_helper);
    }

    public void update()
    {
        if (eventList.Count > 0)
        {
            handle.WaitOne();
            SocketEvent e = eventList.Dequeue();
            SocketAngent a = clientList.Dequeue();
            handle.Set();
            a.dispatchEvent(e);
        }
    }

    public SocketClient Connect(string ip, int port)
    {
        SocketClient client = new SocketClient();
        client.network = this;
        client.socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IPAddress addr = IPAddress.Parse(ip);
        IPEndPoint ipe = new IPEndPoint(addr, port);
        client.ipPoint = ipe;
        IAsyncResult result = client.socket.BeginConnect(ipe, this.OnConnect, client);
        return client;
    }

    public void reConnect(SocketClient client)
    {
        client.socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        IAsyncResult result = client.socket.BeginConnect(client.ipPoint, this.OnConnect, client);
    }

    void OnConnect(IAsyncResult ar)
    {
        SocketClient client = ar.AsyncState as SocketClient;
        try
        {
            client.socket.EndConnect(ar);
            client.offset = 0;
            client.packet = 0;
            client.socket.BeginReceive(client.buffer.buffer, client.buffer.writePosition, client.buffer.writeLength, SocketFlags.None, new AsyncCallback(this.OnReceive), client);

            client.postSend();

            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.CONNECT;

            dispatchEvent(client, e);
        }
        catch (SocketException ex)
        {
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXCEPTION;
            e.data = System.Text.Encoding.Default.GetBytes(ex.Message);
            dispatchEvent(client, e);
        }
     
    }

    void OnDisconnect(IAsyncResult ar)
    {
        SocketClient client = ar.AsyncState as SocketClient;
        Socket socket = client.socket;
        try
        {
            socket.EndDisconnect(ar);
            SocketEvent e = new SocketEvent();
            e.name = SocketEvent.EXIT;
            dispatchEvent(client, e);
        }
        catch
        {

        }
    }

    public void dispatchEvent(SocketAngent angent, SocketEvent e)
    {
        handle.WaitOne();
        eventList.Enqueue(e);
        clientList.Enqueue(angent);
        handle.Set();
    }

    void OnReceive(IAsyncResult ar)
    {
        SocketAngent client = ar.AsyncState as SocketAngent;
        Socket socket = client.socket;
        try
        {
            int BytesRead = socket.EndReceive(ar);
            //if (BytesRead == 0)
            //    return;

            int count = 0;
            if (BytesRead <= 0)
            {
                socket.BeginDisconnect(true, new AsyncCallback(this.OnDisconnect), client);
                return;
            }
            client.buffer.writePosition = client.buffer.writePosition + BytesRead;
            while (true)
            {
                if (client.buffer.Read(client.countBytes, 0, sizeof(int)))
                {
                    count = System.BitConverter.ToInt32(client.countBytes, 0);  
                    count = BinaryStream.ntohl(count);
                    if (client.buffer.validDataLength >= count)
                    {
                        if (client.buffer.Pop(client.recvPacket, 0, count))
                        {
                            MemoryStream read_stream = new MemoryStream(client.recvPacket, sizeof(int), count - sizeof(int));
                            this.OnReceive(read_stream.ToArray(), client);

                        }
                        else
                            break;
                    }
                    else
                        break;
                }
                else
                    break;
            }
            if (client.buffer.writeLength <= 0)
            {
                return;
            }
            socket.BeginReceive(client.buffer.buffer, client.buffer.writePosition, client.buffer.writeLength, 0, new AsyncCallback(this.OnReceive), client);
        }
        catch (SocketException)
        {
            this.OnDisconnect(ar);
        }
    }

    void OnReceive(byte[] data, SocketAngent angent)
    {

        //MemoryStream mem = new MemoryStream(data);

        //byte[] outBytes = null;
        //int outCount = 0;
        //angent.unCompression(data, data.Length, ref outBytes, ref outCount);

        //MemoryStream read_mem = new MemoryStream(outBytes, 0, outCount);


        SocketEvent e = new SocketEvent();
        e.name = SocketEvent.RECV;
        e.data = data;

        dispatchEvent(angent, e);
    }

	void Update () 
    {
	
	}
}
