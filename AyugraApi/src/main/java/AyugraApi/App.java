package AyugraApi;

public class App 
{
    public static void main( String[] args )
    {
        int AyugraId = 0;
        AyugraApi api = new AyugraApi(AyugraId, new AyugraCallback(){
            @Override
            public void onSentPacket(String packet)
            {
                System.out.println("Sent packet : " + packet);
            }

            @Override
            public void onReceivedPacket(String packet)
            {
                System.out.println("Rcvd packet : " + packet);
            }
        });

        api.Start();
    }
}
