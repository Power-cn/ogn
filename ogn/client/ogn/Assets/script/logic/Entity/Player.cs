using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class Player : Entity{
    protected int mAccId;
    protected int mUserId;
    public int AccId
    {
        get { return mAccId; }
        set { mAccId = value; }
    }
    public int UserId
    {
        get { return mUserId; }
        set { mUserId = value; }
    }

}
