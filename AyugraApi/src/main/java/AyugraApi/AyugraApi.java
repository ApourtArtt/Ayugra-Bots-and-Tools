package AyugraApi;

import java.io.RandomAccessFile;


public class AyugraApi {
    public AyugraApi(int AyugraId, AyugraCallback Callback)
    {
        try
        {
            started = false;
            pipe = new RandomAccessFile("\\\\.\\pipe\\AyugraPacketApi_" + String.valueOf(AyugraId), "rw");
            callback = Callback;
            thread = new Thread(() -> { read(); });
        }
        catch (Exception e) { e.printStackTrace(); }
    }

    public void Start()
    {
        started = true;
        thread.start();
    }

    public void Stop()
    {
        started = false;
    }

    public void SendPacket(String packet)
    {
        try
        {
            pipe.write(packet.getBytes());
        }
        catch (Exception e) { e.printStackTrace(); }
    }

    private void read()
    {
        try
        {
            StringBuffer buffer = new StringBuffer();
            while(started)
            {
                int character = pipe.read();
                buffer.append((char)character);
                if (character == '\n' || character == 0)
                {
                    char type = buffer.charAt(0);
                    if (type == '>')
                    {
                        callback.onReceivedPacket(buffer.substring(1));
                    }
                    if (type == '<')
                    {
                        callback.onSentPacket(buffer.substring(1));
                    }
                    buffer.setLength(0);
                }
            }
        }
        catch (Exception e) { e.printStackTrace(); }
    }

    private boolean started;
    private RandomAccessFile pipe;
    private AyugraCallback callback;
    private Thread thread;
}
