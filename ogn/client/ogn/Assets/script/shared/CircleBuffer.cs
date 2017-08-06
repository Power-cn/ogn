using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

public class CircleBuffer
{
    public CircleBuffer(int count = 4096)
    {
        this.count_ = count;
        this.buffer = new byte[count];
        readPosition = 0;
        writePosition = 0;
    }


    public bool Push(byte[] bytes, int offset, int count)
    {
        if (count <= 0)
            return false;

        if (count_ - validDataLength < count)
            return false;

        if (writePosition < readPosition)
        {
            Array.Copy(bytes, offset, this.buffer, writePosition, count);

            writePosition += count;
            writePosition = (writePosition >= count_) ? 0 : writePosition;
            return true;
        }

        if (count_ - writePosition >= count)
        {
            Array.Copy(bytes, offset, this.buffer, writePosition, count);
            writePosition += count;
            writePosition = (writePosition >= count_) ? 0 : writePosition;
            return true;
        }

        int size = count_ - writePosition;

        Array.Copy(bytes, offset, this.buffer, writePosition, size);
        writePosition = 0;

        Array.Copy(bytes, offset + size, this.buffer, writePosition, count - size);
        writePosition += count - size;
        writePosition = (writePosition >= count_) ? 0 : writePosition;
        return true;
    }

    public bool Pop(byte[] bytes, int offset, int count)
    {
        if (count <= 0)
            return false;

        if (bytes.Length - offset < count)
            return false;
        
        if (validDataLength < count)
            return false;

        if (writePosition > readPosition)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            readPosition += count;

            readPosition = (readPosition >= count_) ? 0 : readPosition;
            if (readPosition == writePosition)
                readPosition = writePosition = 0;

            return true;
        }

        if (count_ - readPosition >= count)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            readPosition += count;
            readPosition = (readPosition >= count_) ? 0 : readPosition;
            if (readPosition == writePosition)
                readPosition = writePosition = 0;

            return true;
        }

        int size = count_ - readPosition;

        Array.Copy(this.buffer, readPosition, bytes, offset, size);
        readPosition = 0;
        Array.Copy(this.buffer, readPosition, bytes, offset + size, count - size);
        readPosition += count - size;
        readPosition = (readPosition >= count_) ? 0 : readPosition;
        if (readPosition == writePosition)
            readPosition = writePosition = 0;

        return true;
    }

    public bool Read(byte[] bytes, int offset, int count)
    {
        if (count <= 0)
            return false;

        if (validDataLength < count)
            return false;

        if (writePosition > readPosition)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            return true;
        }

        if (count_ - readPosition >= count)
        {
            Array.Copy(this.buffer, readPosition, bytes, offset, count);
            return true;
        }

        int size = count_ - readPosition;
        Array.Copy(this.buffer, readPosition, bytes, offset, size);
        Array.Copy(this.buffer, 0, bytes, offset + size, count - size);
        return true;
    }

    public void Clear()
    {
        readPosition = 0;
        writePosition = 0;
    }

    public int writeLength
    {
        get 
        {
            if (readPosition > writePosition)
                return readPosition - writePosition;

            if (readPosition < writePosition)
                return count_ - writePosition;

            if (readPosition == 0)
                return count_;

            return 0;
        }
    }

    public int validDataLength
    {
        get {
            if (readPosition == writePosition)
            {
                return 0;
            }

            if (readPosition > writePosition)
            {
                return count_ - readPosition + writePosition;
            }

            return writePosition - readPosition;
        }
    }

    public byte[] buffer;
    public int count_;
    public int readPosition;
    public int writePosition;
}

